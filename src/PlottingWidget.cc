/* 
 * File:   PlottingWidget.cpp
 * Author: blueck
 * 
 * Created on 5. August 2010, 10:25
 */

#include "PlottingWidget.h"

PlottingWidget::PlottingWidget(bool drawGrid) : QwtPlot(), allCurves(100), zoomer(canvas(), true), minPoints(100)
{
    currentId = 0;
    setAutoscrolling(false);
    zoomXAxis = true;
    zoomYAxis = true;
    setDrawGrid(drawGrid);
    timer.setInterval(1000);
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(testData()));
    timer.start();
}


PlottingWidget::~PlottingWidget()
{
    for(unsigned int i=0;i<allCurves.size();i++)
    {
        delete(allCurves[i]);
    }
    allCurves.clear();
}

void PlottingWidget::setAutoscrolling(bool enabled)
{
    this->autoscrolling = enabled;
    zoomer.setEnabled(!enabled);
}

int PlottingWidget::addData(double* xPoints, double* yPoints, int length)
{
    QwtPlotCurve *curve = new QwtPlotCurve("Curve");
    curve->setStyle(QwtPlotCurve::Dots);
    QPen pen;
    pen.setWidth(5);
    curve->setPen(pen);
    double min = 0;
    for(int i=0;i<length;i++)
    {
        if(i == 0)
        {
            min = xPoints[i];
        }
        else
        {
            if(min > xPoints[i])
            {
                min = xPoints[i];
            }
        }
    }
    minPoints[currentId] = min;
    curve->setData(xPoints, yPoints, length);
    curve->attach(this);
    replot();
    allCurves[currentId] = curve;
    return currentId++;
}

void PlottingWidget::addDataToExisting(int dataId, double* xPoints, double* yPoints, int length)
{
    QwtPlotCurve* curve = allCurves[dataId];
    QwtArrayData* currentData = (QwtArrayData*)curve->data().copy();
    QVector<double> xVector = currentData->xData();
    QVector<double> yVector = currentData->yData();
    double min = minPoints[dataId];
    for(int i=0;i<length;i++)
    {
        if(xPoints[i] < min)
        {
            min = xPoints[i];
        }
        xVector.push_back(xPoints[i]);
        yVector.push_back(yPoints[i]);
        if(autoscrolling)
        {
            // scrolling x position
            if(xPoints[i] > axisScaleDiv(xBottom)->upperBound())
            {
                double adjustment = xPoints[i] - axisScaleDiv(xBottom)->upperBound();
                this->setAxisScale(xBottom, axisScaleDiv(xBottom)->lowerBound() + adjustment, xPoints[i]);
            }
            // scrolling y position
            if(yPoints[i] > axisScaleDiv(yLeft)->upperBound())
            {
                double adjustment = yPoints[i] - axisScaleDiv(yLeft)->upperBound();
                this->setAxisScale(yLeft, axisScaleDiv(yLeft)->lowerBound() + adjustment, yPoints[i]);
            }
        }
    }
    minPoints[dataId] = min;
    curve->setData(xVector, yVector);
    replot();
}

void PlottingWidget::removeData(int dataId)
{
    QwtPlotCurve* curve = allCurves[dataId];
    curve->detach();
    delete(curve);
}

void PlottingWidget::setAxisBoundaries(double xLower, double xUpper, double yLower, double yUpper)
{
    this->setAxisScale(xBottom, xLower, xUpper);
    this->setAxisScale(yLeft, yLower, yUpper);
    replot();
    zoomer.setZoomBase(QwtDoubleRect(xLower, xUpper, yLower, yUpper));
}

void PlottingWidget::setAxisAutoScaleOn()
{
    this->setAxisAutoScale(xBottom);
    this->setAxisAutoScale(yLeft);
}

void PlottingWidget::setDrawGrid(bool drawGrid)
{
    this->drawGrid = drawGrid;
    if(drawGrid)
    {
        grid.attach(this);
    }
    else
    {
        grid.detach();
    }
}

QwtPlotCurve* PlottingWidget::getPlotCurveForData(int dataId)
{
    return allCurves[dataId];
}

void PlottingWidget::setDataStyle(int dataId, QColor color, int width, PlotStyle plotStyle)
{
    QwtPlotCurve* curve = allCurves[dataId];
    if(curve != NULL)
    {
        QPen pen = curve->pen();
        pen.setWidth(width);
        pen.setColor(color);
        curve->setPen(pen);
        if(plotStyle == Line)
        {
            curve->setStyle(QwtPlotCurve::Lines);
        }
        else
        {
            curve->setStyle(QwtPlotCurve::Dots);
        }
    }
    replot();
}

void PlottingWidget::addHorizontalBorderLine(QColor color, double value)
{
    QwtPlotMarker* marker = new QwtPlotMarker();
    marker->setLineStyle(QwtPlotMarker::HLine);
    marker->setYValue(value);
    QPen pen = marker->linePen();
    pen.setColor(color);
    pen.setStyle(Qt::DashLine);
    marker->setLinePen(pen);
    marker->attach(this);
}

void PlottingWidget::setAxisTitles(QString xAxisTitle, QString yAxisTitle)
{
    setAxisTitle(yLeft, yAxisTitle);
    setAxisTitle(xBottom, xAxisTitle);
}

void PlottingWidget::zoomToVisibleRange(double xLower, double xUpper, double yLower, double yUpper)
{
    setAxisBoundaries(xLower, xUpper, yLower, yUpper);
    autoscrolling = false;
}

void PlottingWidget::wheelEvent(QWheelEvent* event)
{
//    std::cout << event->x() << ":" << invTransform(xBottom, event->x()) << std::endl;
    if(autoscrolling)
    {
        return;
    }
    // mouse wheel away from the user, or up
    if(event->delta() > 0)
    {
        // zoom in 10% or a factor given by the user
        if(zoomXAxis)
        {
            double adjustment = axisScaleDiv(xBottom)->upperBound() * 0.05;
            if(axisScaleDiv(xBottom)->lowerBound() + adjustment <  axisScaleDiv(xBottom)->upperBound() - adjustment)
            {
                this->setAxisScale(xBottom, axisScaleDiv(xBottom)->lowerBound() + adjustment, axisScaleDiv(xBottom)->upperBound() - adjustment);
            }
        }
        if(zoomYAxis)
        {
            double adjustment = axisScaleDiv(yLeft)->upperBound() * 0.05;
            this->setAxisScale(yLeft, axisScaleDiv(yLeft)->lowerBound() + adjustment, axisScaleDiv(yLeft)->upperBound() - adjustment);
        }
    }
    else
    {
        // zoom out 10% or a factor given by the user
        if(zoomXAxis)
        {
            double adjustment = axisScaleDiv(xBottom)->upperBound() * 0.05;
            double newXBottom = axisScaleDiv(xBottom)->lowerBound() - adjustment;
            double minPoint = 0;
            for(int i=0;i<minPoints.size();i++)
            {
                if(i == 0)
                {
                    minPoint = minPoints[i];
                }
                else
                {
                    if(minPoints[i] < minPoint)
                    {
                        minPoint = minPoints[i];
                    }
                }
            }
            if(newXBottom < minPoint)
            {
                newXBottom = minPoint;
            }
            this->setAxisScale(xBottom, newXBottom, axisScaleDiv(xBottom)->upperBound() + adjustment);
        }
        if(zoomYAxis)
        {
            double adjustment = axisScaleDiv(yLeft)->upperBound() * 0.05;
            this->setAxisScale(yLeft, axisScaleDiv(yLeft)->lowerBound() - adjustment, axisScaleDiv(yLeft)->upperBound() + adjustment);
        }
    }
    replot();
}


void PlottingWidget::testData()
{
    int numberOfPoints = 1;
    // see here for scrolling in x direction everything else can be done accordingly
    double xPoints[numberOfPoints];
    double yPoints[numberOfPoints];
    for(int i=0;i<numberOfPoints;i++)
    {
        xPoints[i] = rand() % 100;// + (currentId * 100);
        yPoints[i] = rand() % 100;
    }
    if(currentId == 0)
    {
        int id = addData(xPoints, yPoints, numberOfPoints);
        setDataStyle(id, QColor(0, 0, 255), 5, Dot);
    }
    else
    {
        addDataToExisting(0, xPoints, yPoints, 1);
    }
//    std:: cout << "Lower:" << this->axisScaleDiv(xBottom)->lowerBound() << "|" << this->axisScaleDiv(xBottom)->upperBound() << std::endl;
//    this->axisScaleDiv(xBottom)->setInterval(100, this->axisScaleDiv(xBottom)->upperBound());
//    if(currentId > 7)
//    {
//        this->setAxisScale(xBottom, 0 + ((currentId-7)*100), 1000 + ((currentId-7)*100));
//    }
//    else
//    {
//        this->setAxisScale(xBottom, 0 , 1000);
//    }
    currentId++;
//    setDataStyle(id, QColor((currentId % 2 == 0 ? 0 : 255), 255, 0), 5);
    // example to get a specific look at a specific value
//    this->setAxisScale(xBottom, 20, 40);
//    this->setAxisScale(yLeft, 20, 40);
}

