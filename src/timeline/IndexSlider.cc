#include "IndexSlider.h"
#include "SlideBarItem.h"
#include <iostream>

IndexSlider::IndexSlider() {
    
}

IndexSlider::IndexSlider(QGraphicsItem* parent, unsigned initIdx) : Slider(parent, initIdx) {
    setLastIndex(((SlideBarItem*)parentItem())->getStartIndex()); // initialize variable to first index for safety reasons.
    setShapeMode(BoundingRectShape);
    setPixmap(QPixmap(":/timeline/index_slider"));
    setOffset(-0.5*width(), -0.5*height());
}
