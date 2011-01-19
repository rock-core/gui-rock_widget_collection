/* 
 * File:   ExtendedPlotZoomer.cc
 * Author: blueck
 * 
 * Created on 29. Dezember 2010, 11:17
 */

#include <QtCore/qstring.h>

#include "ExtendedPlotZoomer.h"

ExtendedPlotZoomer::ExtendedPlotZoomer(QwtPlotCanvas* canvas, bool doReplot) : QwtPlotZoomer(canvas, doReplot)
{
    dataManager = DataManager::getInstance();
}


ExtendedPlotZoomer::~ExtendedPlotZoomer(){ }

QwtText ExtendedPlotZoomer::trackerText(const QwtDoublePoint &pos) const
{
    QString text;
    QString formatter;
    formatter.sprintf("%.%df", dataManager->getZoomerDecimalSize());
    const char* format =  formatter.toStdString().c_str();
    switch(rubberBand())
    {
        case HLineRubberBand:
            text.sprintf(format, pos.y());
            break;
        case VLineRubberBand:
            text.sprintf(format, pos.x());
            break;
        default:
            text.sprintf(format, pos.x(), pos.y());
    }
    return QwtText(text);
}


