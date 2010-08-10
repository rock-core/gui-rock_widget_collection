/*
 * File:   main.cpp
 * Author: blueck
 *
 * Created on 4. August 2010, 14:15
 */

#include <QtGui/QApplication>
#include <qwt-qt4/qwt_plot.h>
#include <qwt-qt4/qwt_plot_curve.h>
#include <QtGui/QPen>
#include <time.h>
#include <iostream>
#include <qwt-qt4/qwt_plot_grid.h>
#include "PlottingWidget.h"


int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);

    PlottingWidget widget(true);
//    double xPoints[100];
//    double yPoints[100];
//    for(int i=0;i<20;i++)
//    {
//        xPoints[i] = rand() % 100;
//        yPoints[i] = rand() % 100;
//    }
//    int id = widget.addData(xPoints, yPoints, 10);
//    widget.setDataStyle(id, QColor(0, 255, 0), 5);
    widget.setAxisTitles(QString("Time"), QString("Temp"));
    widget.setAxisBoundaries(0, 100, 0, 100);
    widget.addHorizontalBorderLine(QColor(255, 0, 0), 55);
    widget.setAutoscrolling(false);
    widget.setMouseZoomAxis(true, false);
//    widget.setAxisAutoScaleOn();
//    widget.zoomToVisibleRange(0, 40, 0, 40);
    widget.show();

//    QwtPlot *myPlot = new QwtPlot();
//    myPlot->setCanvasBackground(QColor(255, 255, 255));
//    // add curves
//    QwtPlotCurve *curve1 = new QwtPlotCurve("Curve Testing");
//    QwtPlotGrid* grid = new QwtPlotGrid();
////    curve1->setStyle(QwtPlotCurve::Dots);
//    grid->attach(myPlot);
//    QPen pen(QColor(255, 0, 0));
//    pen.setWidth(5);
//    curve1->setPen(pen);
//    double xPoints[100];
//    double yPoints[100];
//    srand(time(NULL));
//    for(int i=0;i<100;i++)
//    {
//        xPoints[i] = rand() % 100;
//        yPoints[i] = rand() % 100;
//        std::cout << xPoints[i] << ":" << yPoints[i] << std::endl;
//    }
//    curve1->setData(xPoints, yPoints, 100);
//    curve1->attach(myPlot);
//    myPlot->replot();
//
//    myPlot->show();
    
    // create and show your widgets here

    app.exec();
//    std::cout << "Sleep" << std::endl;
//    sleep(1000 * 5);
//    double xPoints1[100];
//    double yPoints1[100];
//    for(int i=0;i<20;i++)
//    {
//        xPoints1[i] = rand() % 100 + 100;
//        yPoints1[i] = rand() % 100 + 100;
//    }
//    id = widget.addData(xPoints1, yPoints1, 10);
//    widget.setDataStyle(id, QColor(255, 0, 0), 5);
    return 0;
}
