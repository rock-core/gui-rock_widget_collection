
#include "SonarDisplay.h"
#include <stdexcept>
#include <QtCore/QtPlugin>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkImageViewer.h>
#include <vtkRenderer.h>
#include <vtkLookupTable.h>
#include <vtkCellData.h>
#include <vtkMath.h>
#include <vtkFieldData.h>
#include <vtkPolyData.h>
#include <vtkXYPlotActor.h>
#include <vtkStructuredGrid.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkDataSetMapper.h>
#include <vtkStructuredPoints.h>
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkProperty.h"
#include <vtkPicker.h>
#include <vtkPropPicker.h>
#include "vtkCamera.h"

void KeyEvent::Execute (vtkObject *caller, unsigned long eventId, void *callData)
{
    vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
    vtkPropPicker *picker = vtkPropPicker::SafeDownCast(interactor->GetPicker());
    if(picker->GetActor())
    {
        int id =  p_sonar_display->getActorID(picker->GetActor());
        vtkStructuredGridGeometryFilter *filter = p_sonar_display->getFilter(id);
        int extent[6];
        filter->GetExtent(extent);

        switch(interactor->GetKeyCode())
        {
        case 'p':
            p_sonar_display->setPlotData(id);
            break;
        case '1':
            p_sonar_display->reset();
            break;

        case '2':
            if (p_sonar_display->isPlotVisible())
                p_sonar_display->setPlotVisible(false);
            else
            {
                p_sonar_display->setPlotVisible(true);
                p_sonar_display->setPlotData(id);
            }
            break;

        case '8':
            filter->SetExtent(0,1,0,2000,1,1);
            filter->Modified();
            break;
        case '9':
            filter->SetExtent(0,0,0,2000,0,1);
            filter->Modified();
            break;
        case '0':
            filter->SetExtent(0,1,bin_number,bin_number,0,1);
            filter->Modified();
            break;
        case '+':
            if(extent[1] == 1 && extent[5] == 1)
            {
                ++bin_number;
                filter->SetExtent(0,1,bin_number,bin_number,0,1);
                filter->Modified();
            }
            break;
        case '-':
            if(extent[1] == 1 && extent[5] == 1)
            {
                --bin_number;
                filter->SetExtent(0,1,bin_number,bin_number,0,1);
                filter->Modified();
            }
            break;
        }
        p_sonar_display->GetRenderWindow()->Render();
    }
    else
    {
        switch(interactor->GetKeyCode())
        {
        case '1':
            p_sonar_display->reset();
            break;

        }
    }
}

SonarDisplay::SonarDisplay(QWidget *parent):
    QVTKWidget(parent),
    number_of_beams(120), //121
    number_of_bins(500),
    start_bearing(0),
    end_bearing(2*M_PI),
    horizontal_resolution(3.0/180.0*M_PI),
    vertical_resolution(30.0/180.0*M_PI),
    distance_resolution(0.1),
    paramerter_auto(false),
    last_index(0),
    key_event(this),
    sonar_wireframe(vtkSmartPointer<vtkStructuredGrid>::New()),
    render_window(vtkSmartPointer<vtkRenderWindow>::New()),
    renderer(vtkSmartPointer<vtkRenderer>::New()),
    plot_actor(vtkSmartPointer<vtkXYPlotActor>::New())
{
    this->resize(256,256);
    render_window->AddRenderer(renderer);
    this->SetRenderWindow(render_window);
    render_window->GetInteractor()->AddObserver(vtkCommand::KeyReleaseEvent,&key_event,0.0);

    setUpSonar(number_of_beams,number_of_bins,horizontal_resolution,distance_resolution,vertical_resolution);


    //register events
}

int SonarDisplay::getActorID(const vtkActor *actor)
{
    std::vector<vtkSmartPointer<vtkActor> >::iterator iter = sonar_actor.begin();
    for(int i=0;iter != sonar_actor.end();++iter,++i)
        if((*iter).GetPointer() == actor)
            return i;

    return -1;
}

vtkStructuredGridGeometryFilter* SonarDisplay::getFilter(int id)
{
    return sonar_filter[id];
}
vtkUnsignedCharArray* SonarDisplay::getData(int id)
{
    return sonar_data[id];
}

void SonarDisplay::setPlotData(int id)
{
    if(!isPlotVisible())
        return;
    vtkSmartPointer<vtkFieldData> field = vtkSmartPointer<vtkFieldData>::New();
    vtkSmartPointer<vtkDataObject> data = vtkSmartPointer<vtkDataObject>::New();
    field->AddArray(sonar_data[id]);
    data->SetFieldData(field);
#if VTK_MAJOR_VERSION < 6
    plot_actor->RemoveAllInputs();
#else
    plot_actor->RemoveAllDataSetInputConnections();
#endif
    plot_actor->AddDataObjectInput(data);
}
bool SonarDisplay::isPlotVisible()
{
    return plot_actor->GetVisibility();
}

void SonarDisplay::setPlotVisible(bool val)
{
    plot_actor->SetVisibility(val);
}


void SonarDisplay::reset()
{
    std::vector<vtkSmartPointer<vtkStructuredGridGeometryFilter> >::iterator iter = sonar_filter.begin();
    while(iter != sonar_filter.end())
    {
        (*iter)->SetExtent(0,1,0,number_of_bins,1,1);
        (*iter)->Modified();
        ++iter;
    }

    renderer->SetBackground(0.1,0.1,0.1);
    renderer->GetActiveCamera()->SetPosition(0,0,1);
    //renderer->GetActiveCamera()->Elevation(0.0);
    //renderer->GetActiveCamera()->Azimuth(0.0);
    //renderer->GetActiveCamera()->Dolly(0.0);
    //renderer->GetActiveCamera()->SetViewUp(0,1,0);
    renderer->GetActiveCamera()->Yaw(0.0);
    renderer->GetActiveCamera()->Roll(0.0);
    renderer->GetActiveCamera()->Pitch(0.0);
    renderer->GetActiveCamera()->SetFocalPoint(0,0,0);
    renderer->ResetCamera();
    renderer->GetActiveCamera()->ParallelProjectionOn();
    renderer->GetActiveCamera()->Zoom(1.40);
    render_window->Render();
}


void SonarDisplay::setUpSonar(int number_of_beams, int number_of_bins,
        float horizontal_resolution, float distance_resolution,
        float vertical_resolution)
{
    //check some paramerters
    if(number_of_beams < 10 || number_of_bins < 10 || horizontal_resolution < 0||
       horizontal_resolution > M_PI*0.3 || distance_resolution < 0.01 || vertical_resolution > M_PI*0.45)
        throw std::runtime_error("invalid configuration");

    renderer->RemoveAllViewProps();
    sonar_wireframe->ReleaseData();
    sonar_wireframe->RemoveAllObservers();
    sonar_actor.clear();
    sonar_mapper.clear();
    sonar_filter.clear();
    sonar_grid.clear();
    sonar_data.clear();

    sonar_wireframe = vtkSmartPointer<vtkStructuredGrid>::New();

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
    // every 2 meter and every 15 degrees 
    setUpStructuredGrid(sonar_wireframe,25,number_of_bins*distance_resolution*0.5,15.0/180.0*M_PI,2,vertical_resolution);


    //set up pipeline (mapper + actors)
    vtkLookupTable *lut = vtkLookupTable::New();
    lut->SetNumberOfColors(255);
    lut->SetHueRange(0.55,1);
    lut->SetValueRange(1,1.0);
    lut->SetSaturationRange(1,1.0);
    lut->SetTableRange(0,255);
    lut->Build();
    iter = sonar_grid.begin();
    for(;iter != sonar_grid.end(); ++iter)
    {
        vtkSmartPointer<vtkStructuredGridGeometryFilter> filter = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
#if VTK_MAJOR_VERSION < 6
        filter->SetInputConnection((*iter)->GetProducerPort());
#else
        filter->SetInputData(*iter);
#endif
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(filter->GetOutputPort());
        mapper->SetLookupTable(lut);
        mapper->UseLookupTableScalarRangeOn(); 
        mapper->SetColorModeToMapScalars();
        // set_mapper->SetScalarRange(0,255);

        vtkSmartPointer<vtkActor> sonar_grid_actor = vtkSmartPointer<vtkActor>::New();
        sonar_grid_actor->SetMapper(mapper);
        //  sonar_grid_actor->GetProperty()->SetOpacity(0.80);

        sonar_filter.push_back(filter);
        sonar_mapper.push_back(mapper);
        sonar_actor.push_back(sonar_grid_actor);

        renderer->AddActor(sonar_grid_actor);
    }
    lut->Delete();

    //wireframe
    vtkDataSetMapper *set_mapper = vtkDataSetMapper::New();
#if VTK_MAJOR_VERSION < 6
    set_mapper->SetInputConnection(sonar_wireframe->GetProducerPort());
#else
    set_mapper->SetInputData(sonar_wireframe);
#endif
    set_mapper->ScalarVisibilityOff();

    vtkSmartPointer<vtkActor> wireframe_actor = vtkSmartPointer<vtkActor>::New();
    wireframe_actor->SetMapper(set_mapper);
    wireframe_actor->GetProperty()->SetColor(0.5,0.5,0.5);
    wireframe_actor->GetProperty()->SetRepresentationToWireframe();
    wireframe_actor->GetProperty()->SetLineWidth(1.2);
    wireframe_actor->PickableOff();
    wireframe_actor->SetPosition(0,0,0.1);
    set_mapper->Delete();

    renderer->AddActor(wireframe_actor);

    //plotting
    plot_actor->ExchangeAxesOff();
    plot_actor->SetXValuesToValue();
    plot_actor->SetWidth(0.9);
    plot_actor->SetXTitle("");
    plot_actor->SetYTitle("");
    plot_actor->SetHeight(0.50);
    plot_actor->SetPosition(0.04,0);
    plot_actor->PlotPointsOn();
    plot_actor->PlotLinesOn();
    plot_actor->SetVisibility(false);
    renderer->AddActor(plot_actor);

    reset();
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
    {
        std::cout << "SonarDisplay: Bearing is out of range!" << std::endl;
        return;
    }

    if(number_of_bins > this->number_of_bins)
    {
        std::cout << "SonarDisplay: Bin is out of range!" << std::endl;
        return;
    }


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

    if(last_index%3 && last_index != index && abs((long long)(*oldest)->GetMTime() - (long long)sonar_data[end_index]->GetMTime()) > 10e4)
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
