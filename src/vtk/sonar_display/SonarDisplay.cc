
#include "SonarDisplay.h"
#include <stdexcept>
#include <QtCore/QtPlugin>

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkImageViewer.h>
#include <vtkRenderWindowInteractor.h>
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
  QVTKWidget(parent)
{
    this->resize(256,256);

    const int number_of_bins = 5;
    const int number_of_beams = 31;
    const float distance_resolution = 2;
    const float horizontal_angular_resolution = 30 * vtkMath::DegreesToRadians();
    const float vertical_angular_resolution = 12.0* vtkMath::DegreesToRadians();

    // Create the structured grid.
    vtkStructuredPoints *spoints = vtkStructuredPoints::New();
    spoints->SetScalarTypeToUnsignedChar();
    vtkStructuredGrid *sgrid = vtkStructuredGrid::New();
    sgrid->SetDimensions(number_of_beams,number_of_bins,2);
    spoints->SetDimensions(number_of_beams,number_of_bins,2);

    vtkFloatArray *array = vtkFloatArray::New();

    vtkPoints *points = vtkPoints::New();
    points->Allocate(number_of_bins*number_of_beams*2);
    double point[3];
    double radius;
    double theta;

    int offset =0;
    for(int k= 1;k>=-1;k-=2)
    {
        for (int j=0; j<number_of_bins; j++) 
        {
            radius = j* distance_resolution;
            point[2] = k*radius*sin(0.5*horizontal_angular_resolution);
            for (int i=0; i< number_of_beams; i++) 
            {
                theta = vertical_angular_resolution*i;
                point[0] = radius * cos(theta);
                point[1] = radius * sin(theta);
                points->InsertNextPoint(point);
                array->InsertNextValue(i+rand()%100);
            }
           // spoints->SetScalarComponentFromFloat(10,j,k,0,200);
        }
    }
    sgrid->SetPoints(points);
    sgrid->GetPointData()->SetScalars(array);

    // We create a simple pipeline to display the data.
    vtkDataSetMapper *isoMapper = vtkDataSetMapper::New();
    vtkDataSetMapper *isoMapper2 = vtkDataSetMapper::New();
    isoMapper->SetInput(sgrid);
    isoMapper2->SetInput(sgrid);
    isoMapper2->ScalarVisibilityOff();

     vtkLookupTable *lut = vtkLookupTable::New();
            lut->SetNumberOfColors(20);
            lut->SetHueRange(0.0,1.0);
            lut->Build();
    isoMapper->SetLookupTable(lut);
    isoMapper->SetScalarRange(0,255);

    //isoMapper->ScalarVisibilityOff();

    vtkActor *sgridActor = vtkActor::New();
    vtkActor *sgridActor2 = vtkActor::New();
    sgridActor->SetMapper(isoMapper);
    sgridActor2->SetMapper(isoMapper2);
    sgridActor2->GetProperty()->SetColor(0,0,0);
    sgridActor->GetProperty()->SetOpacity(0.9);
    sgridActor2->GetProperty()->SetRepresentationToWireframe();

    vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New();
    volumeProperty->ShadeOff();
    volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

    /*vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
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

    //setup window
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();

    //setup renderer
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    renderer->AddActor(sgridActor2);
    renderer->AddActor(sgridActor);
    renderer->ResetCamera();

    //renderer->AddVolume(volume);

    renderer->SetBackground(0.1,0.1,0.1);
    renderer->GetActiveCamera()->Elevation(60.0);
    renderer->GetActiveCamera()->Azimuth(30.0);
    renderer->GetActiveCamera()->Zoom(1.25);
    this->SetRenderWindow(renderWindow);
}


SonarDisplay::~SonarDisplay()
{
}


void SonarDisplay::addRawImage(const QString &mode, int pixel_size,  int width,  int height,const char* pbuffer)
{
}


