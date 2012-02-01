#include "IndexSlider.h"
#include "SlideBarItem.h"
#include <iostream>

IndexSlider::IndexSlider() {
    
}

IndexSlider::IndexSlider(QGraphicsItem* parent, QPointF initPos) : Slider(parent, initPos, 0) {
    setLastIndex(((SlideBarItem*)parentItem())->getStartIndex()); // initialize variable to first index for safety reasons.
    setHeight(40); // default
}

void IndexSlider::setHeight(int height) {
    sliderHeight = height;
}

int IndexSlider::height() {
    return sliderHeight;
}

int IndexSlider::width() {
    return WIDTH;
}

void IndexSlider::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    painter->setRenderHint(QPainter::Antialiasing, false); // Force 1px slim line
    
    int x_pos = mapFromParent(pos()).x();
    int y_top = -(height()/2) - SlideBarItem::HEIGHT/2;
    int y_bottom = y_top + height();
    painter->drawLine(x_pos, y_top, x_pos, y_bottom);
}