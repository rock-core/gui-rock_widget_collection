#ifndef INDEXSLIDER_H
#define	INDEXSLIDER_H

#include <QGraphicsItem>
#include "Slider.h"

class IndexSlider : public Slider
{
public:
    
    static const int WIDTH = 1;

    IndexSlider();
    IndexSlider(QGraphicsItem* parent, unsigned initIdx);
    
protected:
     
private:
    int sliderHeight;
};

#endif	/* INDEXSLIDER_H */

