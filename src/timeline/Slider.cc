#include "Slider.h"
#include "SlideBarItem.h"
#include <iostream>
#include <QGraphicsSceneMouseEvent>

Slider::Slider() {

}

Slider::Slider(QGraphicsItem* parent, unsigned initIdx) {
    setParentItem(parent);
    this->initIdx = initIdx; 
}

unsigned Slider::getInitIdx() {
    return initIdx;
}

QPointF Slider::boundarySnapPos(QPointF eventPos, qreal clickPosOffsetX, qreal leftBoundary, qreal rightBoundary) {
    
    QPointF newPos;

    // Restricting the sliding area. The left and right boundaries of the slider bar
    // cannot be surpassed.
    if(mapToParent(eventPos).x() - clickPosOffsetX < leftBoundary) {
        // Stepped over left boundary.
        newPos = QPointF(leftBoundary, pos().y());
    } else if (mapToParent(eventPos).x() - clickPosOffsetX > rightBoundary) {
        // Stepped over right boundary.
        newPos = QPointF(rightBoundary, pos().y());
    } else {
        // Slider position is within boundaries.
        newPos = QPointF(mapToParent(eventPos).x() - clickPosOffsetX /*- getPointingOffset()*/,
               pos().y() );
    }
    return newPos;
}

qreal Slider::getClickPosOffsetX(QGraphicsSceneMouseEvent *event) {
    // Offset of clicked x coordinate of time marker item w.r.t. item origin.x
    return mapToParent(event->buttonDownPos((Qt::LeftButton))).x() - pos().x(); 
}

int Slider::height() const {
    return pixmap().height();
}

int Slider::width() const {
    return pixmap().width();
}

unsigned Slider::getLastIndex() {
    return lastIndex;
}

void Slider::setLastIndex(unsigned index) {
    if(index != lastIndex)
        emit sliderMoved(this, index);
    this->lastIndex = index;
}

void Slider::mousePressEvent(QGraphicsSceneMouseEvent *event) 
{
    sliderClicked(this);
}

void Slider::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    /* Sliding bounds in parent's coords */
    qreal leftBoundary = parentItem()->boundingRect().left();
    qreal rightBoundary = parentItem()->boundingRect().right();

    QPointF newPos = boundarySnapPos(event->pos(), getClickPosOffsetX(event), leftBoundary, rightBoundary);
    setPos(newPos);
    
    unsigned index = ((SlideBarItem*)parentItem())->markerIndex(this);

    setLastIndex(index);
    
//    std::cout << "Logical marker position: " << index << std::endl;
//    std::cout << "Computed real position for that index: " << ((SlideBarItem*)parentItem())->markerPositionForIndex(index) << std::endl;
    update();
}

void Slider::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    emit sliderReleased(this, getLastIndex());
}
