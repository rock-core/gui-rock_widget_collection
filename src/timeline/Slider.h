#ifndef SLIDER_H
#define	SLIDER_H

#include <QtGui>
#include <QGraphicsItem>

class Slider : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    
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
    
signals:
    void sliderMoved(Slider* slider, int idx);
    void sliderReleased(Slider* slider, int idx);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    
    QPointF initPos;
    unsigned lastIndex;
};

#endif	/* SLIDER_H */

