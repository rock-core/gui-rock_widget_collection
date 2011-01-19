/* 
 * File:   CurveListItemWidget.cc
 * Author: blueck
 * 
 * Created on 28. Oktober 2010, 13:49
 */

#include "CurveListItemWidget.h"

CurveListItemWidget::CurveListItemWidget(QwtPlotCurve* curve, const QString& text, QListWidget * parent, int type) :
    QListWidgetItem(text, parent, type)
{
    this->curve = curve;
}

CurveListItemWidget::~CurveListItemWidget() { }

