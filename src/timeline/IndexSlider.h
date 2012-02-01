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
    
    void setHeight(int height);
    int height();
    int width();
    
protected:
     void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
     
 private:
     int sliderHeight;
};

#endif	/* INDEXSLIDER_H */

