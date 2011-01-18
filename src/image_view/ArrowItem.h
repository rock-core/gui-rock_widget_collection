/* 
 * File:   ArrowItem.h
 * Author: blueck
 *
 * Created on 30. Juni 2010, 10:41
 */

#include "math.h"
#include "FillItem.h"
#include <iostream>

#ifndef ARROWITEM_H
#define	ARROWITEM_H

/**
 * Arrow shape. This also gives an xample of how to create items that are
 * not composed of multiple shapes. The arrow is composed of a line and a polygon
 * which resides at the end of the line.
 * The endX and endY positions are
 * the end positions of the line. the arrowhead will reside at 10 + (2*lineWidth) further
 * along the line.
 */
class ArrowItem : public FillItem
{
public:
    /**
     * Creates an arrowitem
     * @param posX the starting xposition of the line
     * @param posY the starting y position of the line
     * @param groupNr the group number
     * @param color the color of the line and the border of the arrow
     * @param endX the x end position of the line NOT the arrow
     * @param endY the y end position of the line NOT the arrow
     */
    ArrowItem(int posX, int posY, int groupNr, const QColor &color, int endX, int endY);
    /**
     * Destructor
     */
    virtual ~ArrowItem();
    /**
     * Returns DrawTYpe#Arrow
     * @return DrawTYpe#Arrow
     */
    DrawType getType() {return Arrow;};

    /**
     * @see DrawItem#draw
     */
    void draw(QPainter* painter);
private:
    /** the end position of the line*/
    int endX;
    /**  the y position of the line*/
    int endY;
};

#endif	/* ARROWITEM_H */

