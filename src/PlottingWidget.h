/* 
 * File:   PlottingWidget.h
 * Author: blueck
 *
 * Created on 5. August 2010, 10:25
 */

#ifndef PLOTTINGWIDGET_H
#define	PLOTTINGWIDGET_H

#include <qwt-qt4/qwt_plot.h>
#include <qwt-qt4/qwt_plot_curve.h>
#include <qwt-qt4/qwt_plot_grid.h>
#include <qwt-qt4/qwt_plot_zoomer.h>
#include <qwt-qt4/qwt_plot_marker.h>
#include <QtGui/QWidget>
#include <QtGui/QBoxLayout>
#include <QtGui/QPen>
#include <QtGui/QWheelEvent>
#include <vector>
#include <QTimer>
#include <iostream>
#include <qwt-qt4/qwt_slider.h>

enum PlotStyle {Dot, Line};

class PlottingWidget : public QwtPlot
{

    Q_OBJECT

public:
    PlottingWidget(bool drawGrid);
    virtual ~PlottingWidget();

    bool isDrawGrid() { return drawGrid; };
    void setDrawGrid(bool drawGrid);
    void setDataStyle(int dataId, QColor color, int width, PlotStyle plotStyle);
    void setAxisTitles(QString xAxisTitle, QString yAxisTitle);
    void setAxisBoundaries(double xLower, double xUpper, double yLower, double yUpper);
    void setAxisAutoScaleOn();
    QwtPlotCurve* getPlotCurveForData(int dataId);
    void setAutoscrolling(bool enabled);
    bool getAutoscrolling() {return autoscrolling;};
    void zoomToVisibleRange(double xLower, double xUpper, double yLower, double yUpper);
    void wheelEvent(QWheelEvent* event);
    void setMouseZoomAxis(bool xAxis, bool yAxis) {zoomXAxis = xAxis, zoomYAxis = yAxis;};
    void addHorizontalBorderLine(QColor color, double value);
public slots:
    int addData(double* xPoints, double* yPoints, int length);
    void addDataToExisting(int dataId, double* xPoints, double* yPoints, int length);
    void removeData(int dataId);
    void testData();
private:
    std::vector<QwtPlotCurve*> allCurves;
    std::vector<double> minPoints;
    QwtPlotGrid grid;
    int currentId;
    bool drawGrid;
    QTimer timer;
    bool autoscrolling;
    bool zoomXAxis;
    bool zoomYAxis;
    QwtPlotZoomer zoomer;
    QwtSlider* xSlider;
};

#endif	/* PLOTTINGWIDGET_H */

