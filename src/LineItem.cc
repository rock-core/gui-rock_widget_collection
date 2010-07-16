/* 
 * File:   LineItem.cc
 * Author: blueck
 * 
 * Created on 24. Juni 2010, 14:24
 */

#include <QtGui/qpainter.h>

#include "LineItem.h"

LineItem::LineItem(int posX, int posY, int groupNr, QColor* color, int endX, int endY)
    : DrawItem(posX, posY, groupNr, color)
{
    this->endX = endX;
    this->endY = endY;
}

void LineItem::draw(QPainter* painter)
{
    addPenStyle(painter);
    painter->drawLine(posX, posY, endX, endY);
}


