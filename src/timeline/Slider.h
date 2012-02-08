#ifndef SLIDER_H
#define	SLIDER_H

#include <QtGui>
#include <QGraphicsItem>

class Slider : public QGraphicsPixmapItem
{
public:

    Slider();
    Slider(QGraphicsItem* parent, QPointF initPos);
    
    const QPointF getInitPos();
    void setInitPos(QPointF initPos);
    unsigned getLastIndex();
    void setLastIndex(unsigned index);
    QPointF boundarySnapPos(QPointF eventPos, qreal clickPosOffsetX, qreal leftBoundary, qreal rightBoundary);
    qreal getClickPosOffsetX(QGraphicsSceneMouseEvent *event);
    virtual int height() const;
    virtual int width() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    
    QPointF initPos;
    unsigned lastIndex;
};

#endif	/* SLIDER_H */

