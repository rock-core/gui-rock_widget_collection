/* 
 * File:   DrawItem.cc
 * Author: blueck
 * 
 * Created on 24. Juni 2010, 13:10
 */

#include "DrawItem.h"

DrawItem::DrawItem(int posX, int posY, int groupNr, QColor* color)
{
    this->groupNr = groupNr;
    this->posX = posX;
    this->posY = posY;
    this->color = color;
    this->lineWidth = 0;
    this->penCapStyle = Qt::SquareCap;
    this->penStyle = Qt::SolidLine;

}

void DrawItem::addPenStyle(QPainter* painter)
{
    QPen pen;
    pen.setCapStyle(penCapStyle);
    pen.setStyle(penStyle);
    pen.setWidth(lineWidth);
    pen.setColor(*color);
    painter->setPen(pen);
}


