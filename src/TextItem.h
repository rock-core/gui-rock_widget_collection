/* 
 * File:   TextItem.h
 * Author: blueck
 *
 * Created on 24. Juni 2010, 13:14
 */

#include "DrawItem.h"
#include <QtGui/QFont>

#ifndef TEXTITEM_H
#define	TEXTITEM_H

/**
 * DrawItem that draws text.<br>
 * the standard font used is QFont("Serif", 24, QFont::Normal)
 */
class TextItem : public DrawItem
{
public:
    /**
     * Creates a Textitem
     * @param posX the x start position
     * @param posY the y start position
     * @param groupNr the group number
     * @param color the color of teh text
     * @param text the text to display
     */
    TextItem(int posX, int posY, int groupNr, QColor* color, char* text);

    /**
     * Destructor
     */
    virtual ~TextItem() {};

    /**
     * Returns the text of the item
     * @return the text of the item
     */
    char* getText() {return text;};

    /**
     * Returns the font used
     * @return the font used.
     */
    QFont* getFont(){return font;};

    /**
     * Returns DrawType#Text
     * @return DrawType#Text
     */
    DrawType getType(){return Text;};

    /**
     * Sets the text of the item
     * @param text the text of the item
     */
    void setText(char* text) {this->text = text;};

    /**
     * Sets the font of the item
     * @param font the font of the item
     */
    void setFont(QFont* font) {this->font = font;};

    /**
     * @see DrawItem#draw
     */
    void draw(QPainter* painter);
private:
    /** The text of the item*/
    char* text;
    /** The font of the item*/
    QFont* font;

};

#endif	/* TEXTITEM_H */

