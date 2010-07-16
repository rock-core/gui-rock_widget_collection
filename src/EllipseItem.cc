/* 
 * File:   EllipseItem.cc
 * Author: blueck
 * 
 * Created on 24. Juni 2010, 14:42
 */

#include <QtGui/qpainter.h>

#include "EllipseItem.h"

EllipseItem::EllipseItem(int posX, int posY, int groupNr, QColor* color, int width, int height)
    : RectangleItem(posX, posY, groupNr, color, width, height)
{
}

void EllipseItem::draw(QPainter* painter)
{
    FillItem::draw(painter);
    painter->drawEllipse(posX, posY, width, height);
}


