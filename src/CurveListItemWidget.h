/* 
 * File:   CurveListItemWidget.h
 * Author: blueck
 *
 * Created on 28. Oktober 2010, 13:49
 */

#ifndef CURVELISTITEMWIDGET_H
#define	CURVELISTITEMWIDGET_H

#include <QtGui/QListWidgetItem>

#include <qwt-qt4/qwt_plot_curve.h>

class CurveListItemWidget : public QListWidgetItem
{

public:
    CurveListItemWidget(QwtPlotCurve* curve, const QString& text, QListWidget * parent = 0, int type = Type);
    virtual ~CurveListItemWidget();
    QwtPlotCurve* getCurve() { return curve; };

protected:
    QwtPlotCurve* curve;

private:

};

#endif	/* CURVELISTITEMWIDGET_H */

