
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

#include <vtkMath.h>
#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkPolyData.h>
#include <vtkXYPlotActor.h>


//Q_EXPORT_PLUGIN2(SonarDisplay, SonarDisplay)

SonarDisplay::SonarDisplay(QWidget *parent):
  QVTKWidget(parent)
{
    this->resize(256,256);

    //setup sphere
    vtkSmartPointer<vtkSphereSource> sphereSource = 
        vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = 
        vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> sphereActor = 
        vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);

    //setup window
    vtkSmartPointer<vtkRenderWindow> renderWindow = 
        vtkSmartPointer<vtkRenderWindow>::New();

    //setup renderer
    vtkSmartPointer<vtkRenderer> renderer = 
        vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    renderer->AddActor(sphereActor);
    renderer->ResetCamera();


    vtkSmartPointer<vtkXYPlotActor> plot = 
        vtkSmartPointer<vtkXYPlotActor>::New();
    plot->ExchangeAxesOff();
    plot->SetLabelFormat( "%g" );
    plot->SetXTitle( "Level" );
    plot->SetYTitle( "Frequency" );
    plot->SetXValuesToIndex();

    for (unsigned int i = 0 ; i < 2 ; i++)
    {
        vtkSmartPointer<vtkDoubleArray> array_s = 
            vtkSmartPointer<vtkDoubleArray>::New();
        vtkSmartPointer<vtkFieldData> field = 
            vtkSmartPointer<vtkFieldData>::New();
        vtkSmartPointer<vtkDataObject> data = 
            vtkSmartPointer<vtkDataObject>::New();

        for (int b = 0; b < 30; b++)   /// Assuming an array of 30 elements
        {
            double val = vtkMath::Random(0.0,2.0);
            array_s->InsertValue(b,val);
        }
        field->AddArray(array_s);
        data->SetFieldData(field);
        plot->AddDataObjectInput(data);    
    }

    plot->SetPlotColor(0,1,0,0);
    plot->SetPlotColor(1,0,1,0);


    renderer->AddActor(plot);
    this->SetRenderWindow(renderWindow);
}


SonarDisplay::~SonarDisplay()
{
}


void SonarDisplay::addRawImage(const QString &mode, int pixel_size,  int width,  int height,const char* pbuffer)
{
}


