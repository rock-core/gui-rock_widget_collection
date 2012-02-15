#include "IndexSlider.h"
#include "SlideBarItem.h"
#include <iostream>

IndexSlider::IndexSlider() {
    
}

IndexSlider::IndexSlider(QGraphicsItem* parent, unsigned initIdx) : Slider(parent, initIdx) {
    setLastIndex(((SlideBarItem*)parentItem())->getStartIndex()); // initialize variable to first index for safety reasons.
    setShapeMode(BoundingRectShape);
    //setAcceptTouchEvents(true);
    setPixmap(QPixmap(":/timeline/test_index_slider"));
    setOffset(-0.5*width(), -0.5*height());
}

QRectF IndexSlider::boundingRect() const {
    // TODO why is this not working? should be a huge bounding box.
//    const int enlarger = 50;
//    return QRectF(pos().x() - width()/2.0 - enlarger / 2.0, pos().y() - height()/2.0,
//                  width() + enlarger, height());
    return QGraphicsPixmapItem::boundingRect();
}
