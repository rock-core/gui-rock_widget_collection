/* 
 * File:   PlotWidget.cc
 * Author: blueck
 * 
 * Created on 17. August 2010, 12:24
 */

#include "PlotWidget.h"
#include <QtPlugin>

Q_EXPORT_PLUGIN2(PlotWidget,PlotWidget)

PlotWidget::PlotWidget(QWidget* parent) : QWidget(parent),
        xBottomSlider(NULL), yLeftSlider(NULL, Qt::Vertical), plot(this),
        zoomer(plot.canvas(), true), markers(100), curves(100), initialRect(-1, -1, -1, -1)
{
    plotMarkerId = 0;
    curveId = 0;
    layout.addWidget(&yLeftSlider, 0, 0);
    layout.addWidget(&plot, 0, 1);
    layout.addWidget(&xBottomSlider, 1, 1);
    this->setLayout(&layout);
    minXBottom = INT_MAX;
    maxXBottom = INT_MIN;
    minYLeft = INT_MAX;
    maxYLeft = INT_MIN;
    autoScale = true;
    isZoomed = false;
    QObject::connect(&xBottomSlider, SIGNAL(valueChanged(double)), this, SLOT(xBottomSliderValueChanged(double)));
    QObject::connect(&yLeftSlider, SIGNAL(valueChanged(double)), this, SLOT(yLeftSliderValueChanged(double)));
    QObject::connect(&zoomer, SIGNAL(zoomed(const QwtDoubleRect&)), this, SLOT(zoomed(const QwtDoubleRect&)));
    QObject::connect(&plot, SIGNAL(mouseZoomed(const QwtDoubleRect&)), this, SLOT(zoomed(const QwtDoubleRect&)));
}

PlotWidget::~PlotWidget()
{
    for(int i=0;i<markers.size();i++)
    {
        delete(markers[i]);
    }
    markers.clear();
    for(int i=0;i<curves.size();i++)
    {
        delete(curves[i]);
    }
    curves.clear();
}

QwtPlot::Axis PlotWidget::getAxisForInt(int axis)
{
  switch(axis)
    {
    case X_BOTTOM:
      return QwtPlot::xBottom;
    case Y_LEFT:
      return QwtPlot::yLeft;
    case X_TOP:
      return QwtPlot::xTop;
    case Y_RIGHT:
      return QwtPlot::yRight;
    }
  return QwtPlot::xBottom;
}

void PlotWidget::setDrawGrid(bool drawGrid, bool enableX, bool enableY)
{
    if(drawGrid)
    {
        grid.enableX(enableX);
        grid.enableY(enableY);
        grid.attach(&plot);
    }
    else
    {
        grid.detach();
    }
}

int PlotWidget::addBorderLine(double value, Qt::Orientation orientation, QPen pen)
{
    QwtPlotMarker* marker = new QwtPlotMarker();
    if(orientation == Qt::Horizontal)
    {
        marker->setLineStyle(QwtPlotMarker::HLine);
        marker->setYValue(value);
    }
    else
    {
        marker->setLineStyle(QwtPlotMarker::VLine);
        marker->setXValue(value);
    }
    marker->setLinePen(pen);
    marker->attach(&plot);
    markers[plotMarkerId] = marker;
    return plotMarkerId++;
}

void PlotWidget::enableSlider(int axisId, bool enable)
{
  QwtPlot::Axis axis = getAxisForInt(axisId);
    if(axis == QwtPlot::xBottom)
    {
        xBottomSlider.setVisible(enable);
    }
    else
    {
        yLeftSlider.setVisible(enable);
    }
}

void PlotWidget::zoomed(const QwtDoubleRect& rect)
{
    if(rect == zoomer.zoomBase())
    {
        isZoomed = false;
        // set the slider positions to its original values
        xBottomSlider.blockSignals(true);
        yLeftSlider.blockSignals(true);
        setSliderValues();
        xBottomSlider.setValue(rect.bottomLeft().x());
        yLeftSlider.setValue(rect.topLeft().y());
        xBottomSlider.blockSignals(false);
        yLeftSlider.blockSignals(false);
        xBottomSlider.setEnabled(true);
        yLeftSlider.setEnabled(true);
        // if the initial rect is not the same as the zoombase
        // zoom back to the original width.
        // if we dont do this one cant zoom the rect in parts
        // of the plot which are not visible at the beginning, e.g.
        // out of the zoombase of the zoomer
        if(initialRect.width() != -1)
        {
            zoomer.zoom(initialRect);
        }
    }
    else
    {
        isZoomed = true;
        zoomXSpan = rect.bottomRight().x() - rect.bottomLeft().x();
        zoomYSpan = rect.bottomLeft().y() - rect.topLeft().y();

        xBottomSlider.blockSignals(true);
        yLeftSlider.blockSignals(true);
        xBottomSlider.setValue(rect.bottomLeft().x());
        yLeftSlider.setValue(rect.topLeft().y());
        xBottomSlider.blockSignals(false);
        yLeftSlider.blockSignals(false);
    }
}

void PlotWidget::enableBorderLine(int borderLineId, bool enable)
{
    QwtPlotMarker* marker = markers[borderLineId];
    if(marker != NULL)
    {
        if(enable)
        {
            marker->attach(&plot);
        }
        else
        {
            marker->detach();
        }
    }
}


void PlotWidget::setBorderLineStyle(int borderLineId, QPen pen)
{
    QwtPlotMarker* marker = markers[borderLineId];
    if(marker != NULL)
    {
        marker->setLinePen(pen);
    }
}

void PlotWidget::setAxisAutoScale(int axisId, bool enable)
{
  QwtPlot::Axis axis = getAxisForInt(axisId);
    if(enable)
    {
        plot.setAxisAutoScale(axis);
        initialRect.setRect(-1, -1, -1, -1);
    }
    else
    {
        double lower = plot.axisScaleDiv(axis)->lowerBound();
        double upper = plot.axisScaleDiv(axis)->upperBound();
        setAxisBoundaries(axis, lower, upper);
    }
}

void PlotWidget::setAxisTitles(QString xAxisTitle, QString yAxisTitle)
{
    plot.setAxisTitle(QwtPlot::yLeft, yAxisTitle);
    plot.setAxisTitle(QwtPlot::xBottom, xAxisTitle);
}

void PlotWidget::setAxisBoundaries(int axisId, double lower, double upper, double step)
{
  QwtPlot::Axis axis = getAxisForInt(axisId);
    plot.enableAxis(axis);
    plot.setAxisScale(axis, lower, upper, step);
    plot.replot();
    if(axis == QwtPlot::xBottom)
    {
        xSpan = upper - lower;
    }
    else if(axis == QwtPlot::yLeft)
    {
        ySpan = upper -lower;
    }
    setZoomBase();
    autoScale = false;
}

void PlotWidget::setSliderValues()
{
    xBottomSlider.setScalePosition(QwtSlider::BottomScale);
    xBottomSlider.setRange(minXBottom, maxXBottom);
    yLeftSlider.setScalePosition(QwtSlider::LeftScale);
    yLeftSlider.setRange(minYLeft, maxYLeft);
}

void PlotWidget::setZoomBase()
{
    double xLower = plot.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
    double width = plot.axisScaleDiv(QwtPlot::xBottom)->upperBound() - plot.axisScaleDiv(QwtPlot::xBottom)->lowerBound();
    double height = plot.axisScaleDiv(QwtPlot::yLeft)->upperBound() - plot.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
    double yLower = plot.axisScaleDiv(QwtPlot::yLeft)->lowerBound();
    initialRect.setRect(xLower, yLower, width, height);
    zoomer.setZoomBase(QwtDoubleRect(minXBottom, maxYLeft, maxXBottom + zoomXSpan, maxYLeft + zoomYSpan));
}

void PlotWidget::xBottomSliderValueChanged(double newValue)
{
    double value = xBottomSlider.value();
    if(!isZoomed)
    {
        plot.setAxisScale(QwtPlot::xBottom, value, value + xSpan);
    }
    else
    {
        plot.setAxisScale(QwtPlot::xBottom, value, value + zoomXSpan);
    }
    plot.replot();
}

void PlotWidget::yLeftSliderValueChanged(double newValue)
{
    double value = yLeftSlider.value();
    if(!isZoomed)
    {
        plot.setAxisScale(QwtPlot::yLeft, value, value + ySpan);
    }
    else
    {
        plot.setAxisScale(QwtPlot::yLeft, value, value + zoomYSpan);
    }
    plot.replot();
}

void PlotWidget::setAxisShown(int axisId, bool enable)
{
  QwtPlot::Axis axis = getAxisForInt(axisId);
    plot.enableAxis(axis, enable);
}

void PlotWidget::setAutoscrolling(bool enable)
{
    this->autoscrolling = enable;
    xBottomSlider.setEnabled(!enable);
    yLeftSlider.setEnabled(!enable);
    zoomer.setEnabled(!enable);
    plot.setMouseWheelZoomAxis(!enable, !enable);
}

int PlotWidget::addData(const QList<double>& xPoints, const QList<double>& yPoints, int dataId,
        int xAxisId, int yAxisId)
{
  addData(xPoints.toVector().data(), yPoints.toVector().data(), xPoints.size(), dataId, xAxisId, yAxisId);
}

int PlotWidget::addData(double xPoint, double yPoint, int dataId,
        int xAxisId, int yAxisId)
{
  double xPoints[1];
  double yPoints[1];
  xPoints[0] = xPoint;
  yPoints[0] = yPoint;
  addData(xPoints, yPoints, 1, dataId, xAxisId, yAxisId);
}

int PlotWidget::addData(double* xPoints, double* yPoints, int length, int dataId,
        int xAxisId, int yAxisId)
{
  QwtPlot::Axis xAxis = getAxisForInt(xAxisId);
  QwtPlot::Axis yAxis = getAxisForInt(yAxisId);
    // new data
    if(dataId < 0 || curves[dataId] == NULL)
    {
        QwtPlotCurve* curve = new QwtPlotCurve();
        curve->setStyle(QwtPlotCurve::Dots);
        QPen pen;
        pen.setWidth(5);
        curve->setPen(pen);
        curve->setData(xPoints, yPoints, length);
        curve->attach(&plot);
        curve->setAxis(xAxis, yAxis);
        for(int i=0;i<length;i++)
        {
            setMinMaxPoints(xPoints[i], yPoints[i]);
        }
        plot.replot();
        curves[curveId] = curve;
        dataId = curveId++;
    }
    // existing data
    else
    {
        QwtPlotCurve* curve = curves[dataId];
        QwtArrayData& currentData  = (QwtArrayData&)curve->data();
        QVector<double> xVector = currentData.xData();
        QVector<double> yVector = currentData.yData();
        for(int i=0;i<length;i++)
        {
            setMinMaxPoints(xPoints[i], yPoints[i]);
            xVector.push_back(xPoints[i]);
            yVector.push_back(yPoints[i]);
        }
        curve->setData(xVector, yVector);
        curve->setAxis(xAxis, yAxis);
        // if autoscrolling is set to true
        // rescale the axis
        if(autoscrolling)
        {
            // set the max to the last value and add 5% off the total span
            double finalMaxX = maxXBottom*1.05;
            double finalMaxY = maxYLeft * 1.05;
            plot.setAxisScale(QwtPlot::yLeft, finalMaxY - ySpan, finalMaxY);
            plot.setAxisScale(QwtPlot::xBottom, finalMaxX - xSpan, finalMaxX);
        }
        plot.replot();
    }
    setSliderValues();
    if(autoScale)
    {
        setAxisBoundaries(QwtPlot::yLeft, minYLeft, maxYLeft*1.05);
        setAxisBoundaries(QwtPlot::xBottom, minXBottom, maxXBottom*1.05);
    }
    setZoomBase();
    return dataId;
}

void PlotWidget::setMinMaxPoints(double xPoint, double yPoint)
{
    if(xPoint < minXBottom)
    {
        minXBottom = xPoint;
    }
    else if(xPoint > maxXBottom)
    {
        maxXBottom = xPoint;
    }
    if(yPoint < minYLeft)
    {
        minYLeft = yPoint;
    }
    else if(yPoint > maxYLeft)
    {
        maxYLeft = yPoint;
    }
}

void PlotWidget::enableData(int dataId, bool enable)
{
    QwtPlotCurve* curve = curves[dataId];
    if(curve == NULL)
    {
        if(enable)
        {
            curve->attach(&plot);
        }
        else
        {
            curve->detach();
        }
    }
}

void PlotWidget::setDataStyle(int dataId, QPen pen, int curveStyle)
{
    QwtPlotCurve* curve = curves[dataId];
    if(curve != NULL)
    {
        curve->setPen(pen);
	switch(curveStyle)
	  {
	  case NO_CURVE:
	    curve->setStyle(QwtPlotCurve::NoCurve);
	    break;
	  case LINES:
	    curve->setStyle(QwtPlotCurve::Lines);
	    break;
	  case STICKS:
	    curve->setStyle(QwtPlotCurve::Sticks);
	    break;
	  case STEPS:
	    curve->setStyle(QwtPlotCurve::Steps);
	    break;
	  case DOTS:
	    curve->setStyle(QwtPlotCurve::Dots);
	    break;
	  default:
	    curve->setStyle(QwtPlotCurve::UserCurve);
	  }
    }
}


void PlotWidget::exportPlotAsImage()
{
    QtExporter::exportWidgetAsImagWithDialog(&plot);
}
