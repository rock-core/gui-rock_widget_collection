
#ifndef PLOT2D_H
#define	PLOT2D_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtUiPlugin/QDesignerExportWidget>

#include <QVTKWidget.h>
#include <vtkSmartPointer.h>

#include <iostream>
#include <vector>

#include <vtkXYPlotActor.h>
#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkTextActor.h>
#include <vtkRenderer.h>

typedef vtkSmartPointer<vtkDoubleArray> array_t;
typedef vtkSmartPointer<vtkFieldData> field_t;

class QDESIGNER_WIDGET_EXPORT Plot2d : public QVTKWidget 
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Alexander Duda")

public:
    Plot2d(QWidget *parent = NULL);
    virtual ~Plot2d();

public slots:
    void setTitle(QString title);
    void setXTitle(QString label);
    void setYTitle(QString label);

    void setStyle(int id, QString style);
    void setColor(int id, int r, int g,int b);
    void setPlotLabel(int id, QString label);
    void setNumberOfXLabels(int i);
    void setNumberOfYLabels(int i);
    void showLegend(bool on);
    void autoScroll(bool on);

    void showPlotPoints(bool on);
    void showPlotLines(bool on);

    //not supported by vtk 5.2
    //void showReferenceXLine(bool on);
    //void showReferenceYLine(bool on);
    //void setReferenceXValue(double val);
    //void setReferenceYValue(double val);

    void setXValues(int id);
    void setXRange(double min,double max);
    void setYRange(double min,double max);

    void addPoint(int id, double x,double y);
    void addPoints(int id, const QList<double> &x, const QList<double> &y);

    void clear(int id);

    void render();

    bool isIdValid(int id);
    void scrollTo(double x, double y);
    
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);

protected:
    void addPlot(int id);
    field_t getField(int id);

private:
    vtkSmartPointer<vtkXYPlotActor> plot; 
    std::vector<field_t> field_data;    
    bool auto_scroll;
    vtkSmartPointer<vtkTextActor> text;
    vtkSmartPointer<vtkRenderer> renderer;
};

#endif	

