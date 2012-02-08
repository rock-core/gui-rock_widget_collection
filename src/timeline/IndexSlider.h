#ifndef INDEXSLIDER_H
#define	INDEXSLIDER_H

#include <QGraphicsItem>
#include "Slider.h"



class IndexSlider : public Slider
{
public:
    
    static const int WIDTH = 1;

    IndexSlider();
    IndexSlider(QGraphicsItem* parent, QPointF initPos);
    
//    int height() const;
//    int width() const;
    QRectF boundingRect() const;
    
protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
     
 private:
     int sliderHeight;
};

#endif	/* INDEXSLIDER_H */

