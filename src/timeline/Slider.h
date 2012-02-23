#ifndef SLIDER_H
#define	SLIDER_H

#include <QtGui>
#include <QGraphicsItem>

class Slider : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    
public:

    Slider();
    Slider(QGraphicsItem* parent, unsigned initIdx);

    unsigned getInitIdx();
    unsigned getLastIndex();
    void setLastIndex(unsigned index);
    QPointF boundarySnapPos(QPointF eventPos, qreal clickPosOffsetX, qreal leftBoundary, qreal rightBoundary);
    qreal getClickPosOffsetX(QGraphicsSceneMouseEvent *event);
    virtual int height() const;
    virtual int width() const;
    
signals:
    void sliderMoved(Slider* slider, int idx);
    void sliderClicked(Slider* slider);
    void sliderReleased(Slider* slider, int idx);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    
    unsigned initIdx;
    unsigned lastIndex;
};

#endif	/* SLIDER_H */

