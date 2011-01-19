/* 
 * File:   CurveListItemWidget.h
 * Author: blueck
 *
 * Created on 28. Oktober 2010, 13:49
 */

#ifndef CURVELISTITEMWIDGET_H
#define	CURVELISTITEMWIDGET_H

#include <QListWidgetItem>

#include <qwt_plot_curve.h>

/**
 * Class which extends QListWidget item, to make it easier to select curves
 * within a QListWidget.
 */
class CurveListItemWidget : public QListWidgetItem
{

public:
    /**
     * Creates a CurveListWidgetItem
     * @param curve the curve to put in the item
     * @param text the text to display in thw QListWidget
     * @param parent parent of the item
     * @param type the type of the item
     */
    CurveListItemWidget(QwtPlotCurve* curve, const QString& text, QListWidget * parent = 0, int type = Type);

    /**
     * Destructor
     */
    virtual ~CurveListItemWidget();

    /**
     * Returns the curve stored in the item
     * @return the curve stored
     */
    QwtPlotCurve* getCurve() { return curve; };

protected:
    /** The curve stored in the item*/
    QwtPlotCurve* curve;

private:

};

#endif	/* CURVELISTITEMWIDGET_H */

