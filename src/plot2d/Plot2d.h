
#ifndef PLOT2D_H
#define	PLOT2D_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtDesigner/QDesignerExportWidget>
#include <QtGui/QWidget>

#include <iostream>
#include <vector>
#include "qcustomplot.h"

class QDESIGNER_WIDGET_EXPORT Plot2d : public QCustomPlot
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Alexander Duda")

public:
    Plot2d(QWidget *parent = NULL);
    virtual ~Plot2d();

public slots:
    QObject *getGraph(int index)const{return graph(index);}
    QObject *getLegend()const{return legend;}
    QObject *addGraph(QObject *key_axis,QObject* value_axis)
        {return QCustomPlot::addGraph(dynamic_cast<QCPAxis*>(key_axis),dynamic_cast<QCPAxis*>(value_axis));}
    void removeGraph(int index);
    QObject *getXAxis()const{return xAxis;}
    QObject *getXAxis2()const{return xAxis2;}
    QObject *getYAxis()const{return yAxis;}
    QObject *getYAxis2()const{return yAxis2;}

    void clearData()
    {
        for (int i = 0; i < graphCount(); ++i)
            graph(i)->clearData();
    }

    void setZoomAble(bool enabled, int yAxisIndex = 0)
    {
        if(yAxisIndex == 0)
            setRangeDragAxes(xAxis, yAxis);
        else
            setRangeDragAxes(xAxis, yAxis2);

        if(enabled)
            setRangeDrag(Qt::Horizontal | Qt::Vertical);
        else setRangeDrag(0);
    }
    void setRangeAble(bool enabled, int yAxisIndex = 0)
    {
        if(yAxisIndex == 0)
            setRangeZoomAxes(xAxis, yAxis);
        else
            setRangeZoomAxes(xAxis, yAxis2);

        if(enabled)
            setRangeZoom(Qt::Horizontal | Qt::Vertical);
        else setRangeZoom(0);
    }
    
private:
    bool auto_scroll;
};

#endif	

