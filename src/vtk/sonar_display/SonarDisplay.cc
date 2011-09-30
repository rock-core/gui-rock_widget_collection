
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

#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkMath.h>
#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkPolyData.h>
#include <vtkXYPlotActor.h>

#include <vtkStructuredGrid.h>
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

    const int number_of_bins = 100;
    const int number_of_beams = 121;
    const float distance_resolution = 0.1;
    const float horizontal_angular_resolution = 30 * vtkMath::DegreesToRadians();
    const float vertical_angular_resolution = 3.0* vtkMath::DegreesToRadians();

    // Create the structured grid.
    vtkStructuredPoints *spoints = vtkStructuredPoints::New();
    spoints->SetScalarTypeToUnsignedChar();
    vtkStructuredGrid *sgrid = vtkStructuredGrid::New();
    sgrid->SetDimensions(number_of_beams,number_of_bins,2);
    spoints->SetDimensions(number_of_beams,number_of_bins,2);

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
            }
            spoints->SetScalarComponentFromFloat(10,j,k,0,200);
        }
    }
    sgrid->SetPoints(points);
    
    //sgrid->GetPointData()->SetVectors(vectors);
    //vectors->Delete();




    // We create a simple pipeline to display the data.
    vtkDataSetMapper *isoMapper = vtkDataSetMapper::New();
    isoMapper->SetInput(sgrid);
    isoMapper->ScalarVisibilityOff();

    //   vtkPolyDataMapper *sgridMapper = vtkPolyDataMapper::New();
    //   sgridMapper->SetInputConnection(isoMapper->GetOutputPort());
    vtkActor *sgridActor = vtkActor::New();
    //   sgridActor->SetMapper(sgridMapper);
    sgridActor->SetMapper(isoMapper);
    sgridActor->GetProperty()->SetColor(0,0,0);
    sgridActor->GetProperty()->SetRepresentationToWireframe();

    vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New();
    volumeProperty->ShadeOff();
    volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint(0.0,0.0);
    compositeOpacity->AddPoint(80.0,1.0);
    compositeOpacity->AddPoint(80.1,0.0);
    compositeOpacity->AddPoint(255.0,0.0);
    volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

    vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0.0  ,0.0,0.0,1.0);
    color->AddRGBPoint(40.0  ,1.0,0.0,0.0);
    color->AddRGBPoint(255.0,1.0,1.0,1.0);
    volumeProperty->SetColor(color);

    vtkVolumeRayCastCompositeFunction *compositeFunction = vtkVolumeRayCastCompositeFunction::New();
    vtkVolumeRayCastMapper *volumeMapper = vtkVolumeRayCastMapper::New();

    volumeMapper->SetVolumeRayCastFunction(compositeFunction);
    volumeMapper->SetInput(sgrid);

    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    //setup window
    vtkSmartPointer<vtkRenderWindow> renderWindow = 
        vtkSmartPointer<vtkRenderWindow>::New();

    //setup renderer
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    renderer->AddActor(sgridActor);
    renderer->ResetCamera();

    renderer->AddVolume(volume);

    renderer->SetBackground(1,1,1);
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


