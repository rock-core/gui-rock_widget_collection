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
#include "CSVImporter.h"
#include "PlotWidget.h"


int main(int argc, char *argv[]) {


    QApplication app(argc, argv);

    PlotWidget widget(NULL);
//    widget.setAutoscrolling(false);
//    widget.setAxisTitles(QString("Time (s)"), QString("Temperature"));
    // uncomment to set boundaries for the axis
//    widget.setAxisBoundaries(0, -20, 30);
    //widget.setAutoscaling(true);
//    widget.setAxisBoundaries(1, -2, 4);
//    widget.addBorderLine(2.5);
//    widget.setAxisBoundaries(QwtPlot::yRight, -4.0, 5);
//    widget.setAxisBoundaries(QwtPlot::xTop, 0, 100);
    // uncomment to show another axis
//    widget.setAxisShown(QwtPlot::xTop, false);
//    int id = widget.addData(10, 2);
//    int id = 5;
//    widget.addData(15, 4, id);
//    widget.addData(11, 3, id);
//    widget.setDataStyle(id, QPen(QColor(255, 0, 0)));
    // uncomment if you have any data
//    int dataId = widget.addData(xPoints, yPoints, size);
//    widget.addData(xPoints, yPoints, size, dataId);
    widget.show();


    return app.exec();
}
