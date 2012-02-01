#ifndef SLIDER_H
#define	SLIDER_H

#include <QtGui>
#include <QGraphicsItem>

class Slider : public QGraphicsPixmapItem
{
public:

    Slider();
    Slider(QGraphicsItem* parent, QPointF initPos, double pointing_offset);
    
    const QPointF getInitPos();
    void setInitPos(QPointF initPos);
    void setPointingOffset(double pointing_offset);
    unsigned getLastIndex();
    void setLastIndex(unsigned index);
    QPointF boundarySnapPos(QPointF eventPos, qreal clickPosOffsetX, qreal leftBoundary, qreal rightBoundary);
    qreal getClickPosOffsetX(QGraphicsSceneMouseEvent *event);
    virtual int height();
    virtual int width();
    
    /* 
     * Indicates where the graphical pointer of the slider is located. in this setup 
     * the pointer has to be in the middle of the object because the left and right 
     * case are using the same offset.
     */
    double getPointingOffset();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    
    QPointF initPos;
    double pointing_offset;
    unsigned lastIndex;
};

#endif	/* SLIDER_H */

