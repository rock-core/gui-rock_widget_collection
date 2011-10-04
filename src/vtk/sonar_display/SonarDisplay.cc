
#include "SonarDisplay.h"
#include <stdexcept>
#include <QtCore/QtPlugin>

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkImageViewer.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <vtkLookupTable.h>

#include <vtkCellData.h>

#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkUnstructuredGridVolumeRayCastMapper.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkMath.h>
#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkPolyData.h>
#include <vtkXYPlotActor.h>

#include <vtkStructuredGrid.h>
#include <vtkDataSetTriangleFilter.h>
#include <vtkMath.h>
#include <vtkHedgeHog.h>
#include <vtkContourFilter.h>
#include <vtkStructuredGridOutlineFilter.h>
#include <vtkStructuredGridToPolyDataFilter.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkContourFilter.h>
#include <vtkDataSetMapper.h>
#include <vtkStructuredPoints.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkUnstructuredGridBunykRayCastFunction.h>
#include <vtkFloatArray.h>
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkProperty.h"

#include "vtkCamera.h"


//Q_EXPORT_PLUGIN2(SonarDisplay, SonarDisplay)

SonarDisplay::SonarDisplay(QWidget *parent):
  QVTKWidget(parent),
  number_of_beams(120), //121
  number_of_bins(300),
  start_bearing(0),
  end_bearing(2*M_PI),
  horizontal_resolution(3.0*vtkMath::DegreesToRadians()),
  vertical_resolution(30.0*vtkMath::DegreesToRadians()),
  distance_resolution(0.05),
  paramerter_auto(false),
  last_index(0),
  number_of_writes(0),
  sonar_wireframe(vtkSmartPointer<vtkStructuredGrid>::New()),
  render_window(vtkSmartPointer<vtkRenderWindow>::New())
{
    this->resize(256,256);
    setUpSonar(number_of_beams,number_of_bins,horizontal_resolution,distance_resolution,vertical_resolution);

    //setup renderer
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    render_window->AddRenderer(renderer);
    
    //set up pipeline (mapper + actors)
    //volume grid 

    vtkLookupTable *lut = vtkLookupTable::New();
            lut->SetNumberOfColors(255);
            lut->SetHueRange(0.55,1);
            lut->SetValueRange(1,1.0);
            lut->SetSaturationRange(1,1.0);
            lut->SetTableRange(0,255);
            lut->Build();
    std::vector<vtkSmartPointer<vtkStructuredGrid> >::iterator iter = sonar_grid.begin();
    for(;iter != sonar_grid.end(); ++iter)
    {
        //vtkDataSetMapper *set_mapper = vtkDataSetMapper::New();
        vtkStructuredGridGeometryFilter *filter = vtkStructuredGridGeometryFilter::New();
        filter->SetExtent(0,1,0,2000,0,0);
        filter->SetInputConnection((*iter)->GetProducerPort());
        vtkPolyDataMapper *set_mapper = vtkPolyDataMapper::New();
        set_mapper->SetInputConnection(filter->GetOutputPort());
        set_mapper->SetLookupTable(lut);
        set_mapper->UseLookupTableScalarRangeOn(); 
        set_mapper->SetColorModeToMapScalars();
       // set_mapper->SetScalarRange(0,255);

        vtkSmartPointer<vtkActor> sonar_grid_actor = vtkSmartPointer<vtkActor>::New();
        sonar_grid_actor->SetMapper(set_mapper);
      //  sonar_grid_actor->GetProperty()->SetOpacity(0.80);

        renderer->AddActor(sonar_grid_actor);
        set_mapper->Delete();
        filter->Delete();
    }
    lut->Delete();

    //wireframe
    vtkDataSetMapper *set_mapper = vtkDataSetMapper::New();
    set_mapper = vtkDataSetMapper::New();
    set_mapper->SetInputConnection(sonar_wireframe->GetProducerPort());
    set_mapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> wireframe_actor = vtkSmartPointer<vtkActor>::New();
    wireframe_actor->SetMapper(set_mapper);
    wireframe_actor->GetProperty()->SetColor(0.5,0.5,0.5);
    wireframe_actor->GetProperty()->SetRepresentationToWireframe();
    wireframe_actor->GetProperty()->SetLineWidth(1.1);
    set_mapper->Delete();

    renderer->AddActor(wireframe_actor);
    renderer->ResetCamera();
    renderer->SetBackground(0.1,0.1,0.1);
    renderer->GetActiveCamera()->Elevation(60.0);
    renderer->GetActiveCamera()->Azimuth(30.0);
    renderer->GetActiveCamera()->Zoom(1.25);
    this->SetRenderWindow(render_window);
}


void SonarDisplay::setUpSonar(int number_of_beams, int number_of_bins,
                               float horizontal_resolution, float distance_resolution,
                               float vertical_resolution)
{
    this->number_of_beams = number_of_beams;
    this->number_of_bins = number_of_bins;
   // this->start_bearing = start_bearing;
   // this->end_bearing = end_bearing;
    this->horizontal_resolution = horizontal_resolution;
    this->vertical_resolution = vertical_resolution;
    this->distance_resolution = distance_resolution;

    int missing = number_of_beams - sonar_data.size();
    for(;missing > 0 ;--missing)
    {
        sonar_data.push_back(vtkSmartPointer<vtkUnsignedCharArray>::New());
        sonar_grid.push_back(vtkSmartPointer<vtkStructuredGrid>::New());
    }

    std::vector<vtkSmartPointer<vtkStructuredGrid> >::iterator iter = sonar_grid.begin();
    std::vector<vtkSmartPointer<vtkUnsignedCharArray> >::iterator iter2 = sonar_data.begin();
    for(float val=0;iter != sonar_grid.end();val+=horizontal_resolution, ++iter,++iter2)
    {
        setUpStructuredGrid(*iter,2,number_of_bins+1,horizontal_resolution,distance_resolution,vertical_resolution,val);
        void *p = (*iter2)->WriteVoidPointer(0,number_of_bins);
        memset(p,0,number_of_bins);
        (*iter)->GetCellData()->SetScalars(*iter2);
        (*iter2)->Modified();
    }

    // Create wireframe
    setUpStructuredGrid(sonar_wireframe,25,8, 15.0*vtkMath::DegreesToRadians(),2,vertical_resolution);
}

void SonarDisplay::setUpStructuredGrid(vtkSmartPointer<vtkStructuredGrid> sgrid, int number_of_beams, int number_of_bins,
        float horizontal_resolution, float distance_resolution,
        float vertical_resolution,float start_angle) 
{
    double point[3];
    double radius;
    double theta;

    // Create the structured grid.
    sgrid->SetDimensions(number_of_beams,number_of_bins,2);

    vtkPoints *points = vtkPoints::New();
    points->Allocate(number_of_bins*number_of_beams*2);

    int k =-1;
    if (vertical_resolution == 0)
        k = 0;
    for(;k<=1;k+=2)
    {
        for (int j=1; j<=number_of_bins; j++) 
        {
            radius = j* distance_resolution;
            point[2] = k*radius*sin(0.5*vertical_resolution);
            for (int i=1; i<= number_of_beams; i++) 
            {
                theta = horizontal_resolution*i+start_angle;
                point[0] = radius * cos(theta);
                point[1] = radius * sin(theta);
                points->InsertNextPoint(point);
            }
        }
    }
    sgrid->SetPoints(points);
    points->Delete();
}

void SonarDisplay::setParameterToAuto(bool value)
{
    paramerter_auto = value;
}


void SonarDisplay::deleteBeams(int from, int to)
{
    for(int i = from;i <= to;++i)
    {
        void* p = sonar_data[i]->WriteVoidPointer(0,number_of_bins);
        memset(p,0,number_of_bins);
        sonar_data[i]->Modified();
    }
}

void SonarDisplay::addSonarBeam(float bearing,int number_of_bins,const char* pbuffer)
{
    //check parameter
    if(start_bearing > bearing || end_bearing < bearing)
        throw std::runtime_error("SonarDisplay: Bearing is out of range!");

    if(number_of_bins > this->number_of_bins)
        throw std::runtime_error("SonarDisplay: Bin is out of range!");


    //copy sonar data
    //get index of for the sonar beam
    int index = bearing/horizontal_resolution+0.5;
    if((unsigned int)index >= sonar_data.size())
        index = 0;

    int delta =normalizeDelata(index-last_index);
    bool scan_right;
    if(delta < 0)
        scan_right = false;
    else
        scan_right = true;

    int start_index;
    int end_index;

    if(index == last_index)
    {
        start_index = index;
        end_index = index;
    }
    else
    {
        if(scan_right)
        {
            start_index = last_index+1;
            end_index = index;
        }
        else
        {
            start_index = index;
            end_index = last_index-1;
            if(end_index < 0)
                end_index = number_of_beams-1;
        }
    }

    //find oldest 
    std::vector<vtkSmartPointer<vtkUnsignedCharArray> >::iterator iter2 = sonar_data.begin();
    std::vector<vtkSmartPointer<vtkUnsignedCharArray> >::iterator oldest = sonar_data.begin();
    for(;iter2 != sonar_data.end();++iter2)
    {
        if((*oldest)->GetMTime() > (*iter2)->GetMTime())
            oldest = iter2;
    }

    do 
    {
        if(start_index >= number_of_beams)
            start_index = 0;
        void* p = sonar_data[start_index]->WriteVoidPointer(0,number_of_bins);
        memcpy(p,pbuffer,number_of_bins);
        sonar_data[start_index]->Modified();
    }while(start_index != end_index && ++start_index);

    if(last_index%3 && last_index != index && abs((*oldest)->GetMTime() - sonar_data[end_index]->GetMTime()) > 10e4)
    {
        void* p  = (*oldest)->WriteVoidPointer(0,number_of_bins);
        memset(p,0,number_of_bins);
        (*oldest)->Modified();
    }

    last_index = index;
    render_window->Render();
}

int SonarDisplay::normalizeDelata(int delta)
{
    if(delta > 0.5*number_of_beams)
        return delta - number_of_beams;
    else if(delta < -0.5*number_of_beams)
        return delta + number_of_beams;
    return delta;
}


//  renderer->AddVolume(volume);
//
/*
   vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New();
   volumeProperty->ShadeOff();
   volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
   vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
   compositeOpacity->AddPoint(0.0,0.8);
   compositeOpacity->AddPoint(80.0,0.8);
   compositeOpacity->AddPoint(80.1,0.8);
   compositeOpacity->AddPoint(255.0,0.8);
   volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

   vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
   color->AddRGBPoint(0.0  ,0.0,0.0,1.0);
   color->AddRGBPoint(41.0  ,1.0,0.0,0.0);
   color->AddRGBPoint(100.0  ,1.0,1.0,0.0);
   color->AddRGBPoint(255.0,1.0,1.0,1.0);
   volumeProperty->SetColor(color);

//vtkVolumeRayCastCompositeFunction *compositeFunction = vtkVolumeRayCastCompositeFunction::New();
//vtkVolumeRayCastMapper *volumeMapper = vtkVolumeRayCastMapper::New();
//volumeMapper->SetVolumeRayCastFunction(compositeFunction);

vtkUnstructuredGridBunykRayCastFunction* func = vtkUnstructuredGridBunykRayCastFunction::New();
vtkUnstructuredGridVolumeRayCastMapper *volumeMapper = vtkUnstructuredGridVolumeRayCastMapper::New();
volumeMapper->SetRayCastFunction(func);

vtkDataSetTriangleFilter* filter2 = vtkDataSetTriangleFilter::New();
filter2->SetInput(sgrid);
volumeMapper->SetInputConnection(filter2->GetOutputPort());

vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
volume->SetMapper(volumeMapper);
volume->SetProperty(volumeProperty);*/
