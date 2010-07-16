/* 
 * File:   PolygonItem.cc
 * Author: blueck
 * 
 * Created on 29. Juni 2010, 12:06
 */

#include <QtGui/qpainter.h>


#include "PolygonItem.h"
#include "FillItem.h"

PolygonItem::PolygonItem(QColor* color, int groupNr, QPoint* points, int numberOfPoints)
    : PolylineItem(color, groupNr, points, numberOfPoints)
{
}

PolygonItem::~PolygonItem()
{
}

void PolygonItem::draw(QPainter* painter)
{
    FillItem::draw(painter);
    painter->drawPolygon(getAllPoints(), getNumberOfPoints());
}

