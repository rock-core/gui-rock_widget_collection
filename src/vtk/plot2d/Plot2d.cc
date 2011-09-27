
#include "Plot2d.h"
#include <stdexcept>
#include <sstream>
#include <QtCore/QtPlugin>
#include <QMouseEvent>

#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkImageViewer.h>
#include <vtkInteractorStyleImage.h>
#include <vtkJPEGReader.h>

#include <vtkMath.h>
#include <vtkPolyData.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkXYPlotWidget.h>

const double c_norm = 1.0/255;

Plot2d::Plot2d(QWidget *parent):
  QVTKWidget(parent),auto_scroll(true)
{
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    SetRenderWindow(renderWindow);

    text = vtkSmartPointer<vtkTextActor>::New();
    text->SetInput("0/0");
    text->SetDisplayPosition(5,1);

    plot = vtkSmartPointer<vtkXYPlotActor>::New();
    plot->ExchangeAxesOff();
    plot->SetXValuesToValue();
    plot->SetWidth(0.9);
    plot->SetHeight(1.0);
    plot->SetPosition(0.04,0);
    plot->LegendOn();
    plot->PlotPointsOn();
    plot->PlotLinesOn();
    addPlot(0);

    renderer =  vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(plot);
    renderer->AddActor(text);
    renderer->ResetCamera();
    renderer->SetBackground(0.1,0.1,0.1);
    renderWindow->AddRenderer(renderer);

    vtkSmartPointer<vtkXYPlotWidget> widget = vtkSmartPointer<vtkXYPlotWidget>::New();
    widget->SetXYPlotActor(plot);
    GetRenderWindow()->GetInteractor()->SetInteractorStyle(widget);
}

Plot2d::~Plot2d(){} 
void Plot2d::setTitle(QString title){plot->SetTitle(title.toStdString().c_str());}
void Plot2d::setXTitle(QString title){plot->SetXTitle(title.toStdString().c_str());}
void Plot2d::setYTitle(QString title){plot->SetYTitle(title.toStdString().c_str());}
void Plot2d::setStyle(int id, QString style){}
void Plot2d::setColor(int id, int r, int g, int b){addPlot(id);plot->SetPlotColor(id,c_norm*r,c_norm*g,c_norm*b);}
void Plot2d::setXRange(double min, double max){plot->SetXRange(min,max);}
void Plot2d::setYRange(double min, double max){plot->SetYRange(min,max);}
void Plot2d::setPlotLabel(int id, QString label){addPlot(id);plot->SetPlotLabel(id,label.toStdString().c_str());}
void Plot2d::showLegend(bool on){if(on){plot->LegendOn();}else{plot->LegendOff();}}
void Plot2d::showPlotPoints(bool on){if(on){plot->PlotPointsOn();}else{plot->PlotPointsOff();}}
void Plot2d::showPlotLines(bool on){if(on){plot->PlotLinesOn();}else{plot->PlotLinesOff();}}
void Plot2d::setNumberOfXLabels(int i){plot->SetNumberOfXLabels(i);}
void Plot2d::setNumberOfYLabels(int i){plot->SetNumberOfYLabels(i);}
void Plot2d::autoScroll(bool on){auto_scroll = on;}

//not supported by vtk 5.2
//void Plot2d::showReferenceXLine(bool on){if(on){plot->ShowReferenceXLineOn();}else{plot->ShowReferenceXLineOff();}}
//void Plot2d::showReferenceYLine(bool on){if(on){plot->ShowReferenceYLineOn();}else{plot->ShowReferenceYLineOff();}}
//void Plot2d::setReferenceXValue(double val){plot->SetReferenceXValue(val);}
//void Plot2d::setReferenceYValue(double val){plot->SetReferenceYValue(val);}

void Plot2d::clear(int id)
{
    if(!isIdValid(id))
        return;
    getField(id)->Reset();
}

void Plot2d::scrollTo(double x, double y)
{
    double* xrange = plot->GetXRange();
    double width = (xrange[1]-xrange[0]);
    double* yrange = plot->GetYRange();
    double height = (yrange[1]-yrange[0]);
    setXRange(x-width*0.8,x+width*0.2);
    setYRange(y-height*0.8,y+height*0.2);
}

void Plot2d::addPoint(int id, double x,double y)
{
    field_t field = getField(id);
    array_t array = (vtkDoubleArray*)field->GetArray(0);
    array_t array2 = (vtkDoubleArray*)field->GetArray(1);
    array->InsertNextValue(x);
    array2->InsertNextValue(y);
    array->Modified();
    array2->Modified();
    if(auto_scroll)
        scrollTo(x,y);
}

void Plot2d::addPoints(int id, const QList<double> &x, const QList<double> &y)
{
    if(x.size() != y.size())
        std::runtime_error("addValues: Size mismatch. Both lists must have the same size.");

    if(x.size() == 0)
        return;
    
    field_t field = getField(id);
    array_t array = (vtkDoubleArray*)field->GetArray(0);
    array_t array2 = (vtkDoubleArray*)field->GetArray(1);

    QList<double>::const_iterator iter_x = x.constBegin();
    QList<double>::const_iterator iter_y = y.constBegin();
    for(;iter_x != x.constEnd();++iter_x,++iter_y)
    {
        array->InsertNextValue(*iter_x);
        array2->InsertNextValue(*iter_y);
    }
    array->Modified();
    array2->Modified();
    if(auto_scroll)
        scrollTo(*(--iter_x),*(--iter_y));
}

void Plot2d::setXValues(int id)
{
    if(isIdValid(id))
        plot->SetXValues(id);
    else
        plot->SetXValuesToIndex();
}

bool Plot2d::isIdValid(int id)
{
    if (id >=0 && (unsigned int)id < field_data.size())
        return true;
    return false;
}

void Plot2d::render()
{
//    data2->Update();
//    cachedImageDirty();
//    GetRenderWindow()->GetInteractor()->Render();
//    update();
}

/////////////////////////////////////////////////////////////////////////////////
//protected methods
/////////////////////////////////////////////////////////////////////////////////

void Plot2d::mouseMoveEvent(QMouseEvent *event)
{
    QVTKWidget::mouseMoveEvent(event);

    int x,y;
    double x2,y2;
    std::stringstream ss;

    GetRenderWindow()->GetInteractor()->GetLastEventPosition(x,y);
    plot->SetViewportCoordinate(x,y);
    plot->ViewportToPlotCoordinate(renderer);
    plot->GetViewportCoordinate(x2,y2);

    ss << x2 << " / " << y2 << "\n";
    text->SetInput(ss.str().c_str());
    text->Modified();
    update();
}

void Plot2d::addPlot(int id)
{
    if(id < 0)
        throw std::runtime_error("addPlot: Bad id. Id must be positive.");

    while((unsigned int)id >= field_data.size())
    {
        array_t array = vtkSmartPointer<vtkDoubleArray>::New();
        array_t array2 = vtkSmartPointer<vtkDoubleArray>::New();
        field_t field = vtkSmartPointer<vtkFieldData>::New();
        vtkSmartPointer<vtkDataObject> data = vtkSmartPointer<vtkDataObject>::New();

        field->AddArray(array);
        field->AddArray(array2);
        data->SetFieldData(field);
        plot->AddDataObjectInput(data);
        plot->SetDataObjectXComponent(id,0);
        plot->SetDataObjectYComponent(id,1);
        field_data.push_back(field);
    }
}

field_t Plot2d::getField(int id)
{
    addPlot(id);
    return field_data[id];
}

