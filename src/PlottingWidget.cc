/* 
 * File:   PlottingWidget.cpp
 * Author: blueck
 * 
 * Created on 5. August 2010, 10:25
 */

#include "PlottingWidget.h"

PlottingWidget::PlottingWidget(QWidget* parent)
    : QwtPlot(parent)
{
}


PlottingWidget::~PlottingWidget()
{
}

void PlottingWidget::wheelEvent(QWheelEvent* event)
{

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
            if(axisScaleDiv(yLeft)->lowerBound() + adjustment <  axisScaleDiv(yLeft)->upperBound() - adjustment)
            {
                this->setAxisScale(yLeft, axisScaleDiv(yLeft)->lowerBound() + adjustment, axisScaleDiv(yLeft)->upperBound() - adjustment);
            }
        }
    }
    else
    {
        // zoom out 10% or a factor given by the user
        if(zoomXAxis)
        {
            double adjustment = axisScaleDiv(xBottom)->upperBound() * 0.05;
            double newXBottom = axisScaleDiv(xBottom)->lowerBound() - adjustment;
            this->setAxisScale(xBottom, newXBottom, axisScaleDiv(xBottom)->upperBound() + adjustment);
        }
        if(zoomYAxis)
        {
            double adjustment = axisScaleDiv(yLeft)->upperBound() * 0.05;
            double newYBottom = axisScaleDiv(yLeft)->lowerBound() - adjustment;
            this->setAxisScale(yLeft, newYBottom, axisScaleDiv(yLeft)->upperBound() + adjustment);
        }
    }
    replot();
    double width = axisScaleDiv(xBottom)->upperBound() - axisScaleDiv(xBottom)->lowerBound();
    double height = axisScaleDiv(yLeft)->upperBound() - axisScaleDiv(yLeft)->lowerBound();
    emit mouseZoomed(QwtDoubleRect(axisScaleDiv(xBottom)->lowerBound(), axisScaleDiv(yLeft)->lowerBound(), width, height));
}

