/* 
 * File:   FillItem.cc
 * Author: blueck
 * 
 * Created on 29. Juni 2010, 14:25
 */

#include "FillItem.h"
#include <iostream>

FillItem::FillItem(int posX, int posY, int groupNr, QColor* color)
    : DrawItem(posX, posY, groupNr, color)
{
    drawBorder = true;
    interiorColor = NULL;
}

void FillItem::addBrushStyle(QPainter* painter)
{
    if(interiorColor != NULL)
    {
        painter->setBrush(*interiorColor);
    }
}

void FillItem::draw(QPainter* painter)
{
    if(!drawBorder)
    {
        painter->setPen(Qt::NoPen);
    }
    else
    {
        addPenStyle(painter);
    }
    addBrushStyle(painter);
}

