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
    widget.setAxisTitles(QString("Time (s)"), QString(""));
    // uncomment to set boundaries for the axis
//    widget.setAxisBoundaries(QwtPlot::xBottom, 0, 30);
//    widget.setAxisBoundaries(QwtPlot::yLeft, -2, 4);
//    widget.setAxisBoundaries(QwtPlot::yRight, -4.0, 5);
//    widget.setAxisBoundaries(QwtPlot::xTop, 0, 100);
    // uncomment to show another axis
//    widget.setAxisShown(QwtPlot::xTop, false);
    widget.setDrawGrid(true);
    widget.addBorderLine(3.5f);
    // uncomment if you have any data
//    int dataId = widget.addData(xPoints, yPoints, size);
//    widget.addData(xPoints, yPoints, size, dataId);
    widget.show();


    return app.exec();
}
