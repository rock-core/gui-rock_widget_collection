
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
    void removeGraph(QObject *graph){removeGraph(dynamic_cast<QCPGraph*>(graph));} 
    QObject *getXAxis()const{return xAxis;}
    QObject *getXAxis2()const{return xAxis2;}
    QObject *getYAxis()const{return yAxis;}
    QObject *getYAxis2()const{return yAxis2;}

    void setZoomAble(bool enabled){if(enabled)setRangeDrag(Qt::Horizontal | Qt::Vertical); else setRangeDrag(0);}
    void setRangeAble(bool enabled){if(enabled)setRangeZoom(Qt::Horizontal | Qt::Vertical); else setRangeZoom(0);}
    
private:
    bool auto_scroll;
};

#endif	

