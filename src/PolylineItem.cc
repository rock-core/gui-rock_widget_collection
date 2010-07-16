/* 
 * File:   PolylineItem.cc
 * Author: blueck
 * 
 * Created on 29. Juni 2010, 11:32
 */

#include <QtGui/qpainter.h>


#include <QtCore/qvector.h>

#include "PolylineItem.h"

PolylineItem::PolylineItem(QColor* color, int groupNr, QPoint* points, int numberOfPoints)
    : FillItem(0, 0, groupNr, color)
{
    for(int i=0;i<numberOfPoints;i++)
    {
        this->points.push_back(points[i]);
    }
}

PolylineItem::~PolylineItem()
{
}

void PolylineItem::draw(QPainter* painter)
{
    addPenStyle(painter);
    painter->drawPolyline(getAllPoints(), getNumberOfPoints());
}

void PolylineItem::addPoint(QPoint point)
{
    points.push_back(point);
}

void PolylineItem::removeAllPoints()
{
    points.clear();
}

void PolylineItem::removePoint(QPoint point)
{
    points.removeAll(point);
}

int PolylineItem::getNumberOfPoints()
{
    return points.size();
}

QPoint* PolylineItem::getAllPoints()
{
    return points.toVector().data();
}

