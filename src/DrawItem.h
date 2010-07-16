/* 
 * File:   DrawItem.h
 * Author: blueck
 *
 * Created on 24. Juni 2010, 13:10
 */

#include <QtGui/QColor>
#include <QtGui/QPainter>

#ifndef DRAWITEM_H
#define	DRAWITEM_H

/**
 * Enumeration for DrawTypes
 */
enum DrawType {Text, Line, Ellipse, Rectangle, Polyline, Polygon, Arrow};

/**
 * Abstract Base class for all shapes. It defines basic members and
 * methods and the abstract getType() method which needs to be overwritten with
 * a type from the DrawType enum.
 * The abstract draw method needs to be overwritten in each item. So each item draws
 * itself.
 */
class DrawItem
{
public:
    /**
     * Generates a DrawItem
     * @param posX the starting x position
     * @param posY the starting y position
     * @param groupNr the group number
     * @param color the color
     */
    DrawItem(int posX, int posY, int groupNr, QColor* color);

    /**
     * Destructor
     */
    virtual ~DrawItem() {};

    /**
     * Returns the x position
     * @return the x position
     */
    int getPosX(){return posX;};

    /**
     * Returns the y poistion
     * @return the y position
     */
    int getPosY(){return posY;};

    /**
     * Returns the group number
     * @return the group number
     */
    int getGroupNr(){return groupNr;}

    /**
     * Returns the color
     * @return the color
     */
    QColor* getColor(){return color;};

    /**
     * virtual method wjich returns the type of teh item
     * @return the type of the item defined by #seeDrawType
     */
    virtual DrawType getType() = 0;

    /**
     * Setrs the x position
     * @param posX the x position
     */
    void setPosX(int posX) {this->posX = posX;};

    /**
     * Sets the y position
     * @param posY the y position
     */
    void setPosY(int posY) {this->posY = posY;};

    /**
     * Sets the group number
     * @param groupNr the group number
     */
    void setGroupNr(int groupNr) {this->groupNr = groupNr;};

    /**
     * Sets the color
     * @param color the color
     */
    void setColor(QColor* color) {this->color = color;};

    /**
     * Returns the width of lines drawn. the smallest visible width is zero
     * @return the width of lines drawn
     */
    int getLineWidth() {return lineWidth;};

    /**
     * Returns the pen style, usually this should be solid
     * @return  the Qt Pen style
     */
    Qt::PenStyle getPenStyle() {return penStyle;};

    /**
     * Return sthe pen cap style
     * @return the qt pen cap style
     */
    Qt::PenCapStyle getPenCapStyle() {return penCapStyle;};

    /**
     * Sets the width of the lines drawn, 0 is the smallest visible line
     * available
     * @param lineWidth the width of the line
     */
    void setLineWidth(int lineWidth) {this->lineWidth = lineWidth;};

    /**
     * Sets the pen style @see Qt::PenStyle
     * @param penStyle the penStyle
     */
    void setPenStyle(Qt::PenStyle penStyle) {this->penStyle = penStyle;};

    /**
     * Sets the pen cap style @see Qt::PenCapStyle
     * @param penCapStyle
     */
    void setPenCapStyle(Qt::PenCapStyle penCapStyle) {this->penCapStyle = penCapStyle;};

    /**
     * avstract method to draw the item
     * @param painter the painter with wich to draw the shape
     */
    virtual void draw(QPainter* painter) = 0;

protected:
    /**
     * Adds a pen to the given painter with the parameters given to the item
     * @param painter teh painter with which afterwards teh shape will be drawn
     */
    void addPenStyle(QPainter* painter);
    /** The x position*/
    int posX;
    /** The y position*/
    int posY;
    /** The group number*/
    int groupNr;
    /** The color*/
    QColor* color;
    /** the width of the shapes lines*/
    int lineWidth;
    /** The pen style of the lines*/
    Qt::PenStyle penStyle;
    /** the pen cap style of the item, irrelevant for lines which have no start or end point*/
    Qt::PenCapStyle penCapStyle;
};

#endif	/* DRAWITEM_H */

