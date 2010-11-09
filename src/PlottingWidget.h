#ifndef PLOTTINGWIDGET_H
#define	PLOTTINGWIDGET_H

#include <qwt_plot.h>
#include <QWheelEvent>
#include <qwt_scale_div.h>
#include <iostream>

/**
 * Plotting widget which extends qwtPlot to add mouse wheel
 * functionality. When scrolling the mouse wheel a signal will be emitted
 * which contains the new dimension the plot shall be scrolled to.
 * A mouseZoomed signal will be emitted containing the new
 * dimensions the plot shall be scrolled to. Note that the given rectangle
 * will not contain pixel data, instead it will contain x and y axis
 * data in the coordinate system the plot is currently showing.
 * <br>
 * Currently only zooming of the yLeft and xBottom axis is supported.
 *
 *  <h1>History</h1>
 * 2010-08-18 Initial revision
 *
 * <h1>Known Issues</h1>
 *
 * <h1>To-Do</h1>
 * Support for all axis
 *
 * @author Bjoern Lueck <blueck@dfki.de>
 * @version 0.1
 */
class PlottingWidget : public QwtPlot
{

    Q_OBJECT

public:
    /**
     * Creates the widget
     * @param parent the parent of the widget
     */
    PlottingWidget(QWidget* parent);

    /**
     * Destroys the widget
     */
    virtual ~PlottingWidget();

    /**
     * Event which will be called when the mouse whell is stirred
     * @param event the event that occured
     */
    void wheelEvent(QWheelEvent* event);

    /**
     * Sets which axis can be zoomed via the mouse wheel
     * @param xAxis whether the x axis can be zoomed, defaults to true
     * @param yAxis whether the y axis can be zoomed, defaults to true
     */
    void setMouseWheelZoomAxis(bool xAxis=true, bool yAxis=true) {zoomXAxis = xAxis, zoomYAxis = yAxis;};
 signals:
    /**
     * Signal which will be emitted when the mouse wheel was zoomed and the
     * axis can be zoomed.
     * @param rect the rect to ehich shall be zoomed
     */
    void mouseZoomed(const QwtDoubleRect& rect);
private:
    /** whether the x axis shall be zoomed*/
    bool zoomXAxis;
    /** whether the y axis shall be zoomed*/
    bool zoomYAxis;
};

#endif	/* PLOTTINGWIDGET_H */

