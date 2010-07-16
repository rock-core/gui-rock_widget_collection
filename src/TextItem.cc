/* 
 * File:   TextItem.cc
 * Author: blueck
 * 
 * Created on 24. Juni 2010, 13:14
 */

#include "TextItem.h"

TextItem::TextItem(int posX, int posY, int groupNr, QColor* color, char* text) : DrawItem(posX, posY, groupNr, color)
{
    this->text = text;
    font = new QFont("Serif", 24, QFont::Normal);
}

void TextItem::draw(QPainter* painter)
{
    painter->setFont(*font);
    painter->drawText(QRect(posX, posY, 200, 200), Qt::TextWordWrap, text);
}
