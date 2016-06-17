#include "BoundarySlider.h"
#include "SlideBarItem.h"

#include <iostream>

#include <base/Logging.hpp>

BoundarySlider::BoundarySlider() : Slider() {

}

BoundarySlider::BoundarySlider(QGraphicsItem* parent, unsigned initIdx) : Slider(parent, initIdx) {
    setPixmap(QPixmap(":/timeline/triangle_slider"));
    
    /* Set item coordinate origin to the x_center and let the slider tip overlap the slidebar a little.
     * So the origin is a little above the y_center. */
    setOffset( -0.5 * width() - 1, 0.2 * height() );
}

Direction BoundarySlider::leftRight(BoundarySlider *otherMarker) {
    if(this->getInitIdx() > otherMarker->getInitIdx()) {
        return RIGHT;
    }
    else {
        return LEFT;
    }
}

void BoundarySlider::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    /* Sliding bounds in parent's coords */
    qreal leftBoundary = parentItem()->boundingRect().left();
    qreal rightBoundary = parentItem()->boundingRect().right();

    QPointF newPos = boundarySnapPos(event->pos(), getClickPosOffsetX(event), leftBoundary, rightBoundary);

    // Check for marker overlap with new position
    setPos(newPos);
    if( BoundarySlider *overlappingMarker = ((SlideBarItem*) parentItem())->markerOverlap(this) ) {
        /* Collision detected! Do not move further in this direction! */

        LOG_DEBUG_S << "Overlapping time markers!";

        qreal overlapLeftBoundary;
        qreal overlapRightBoundary;
        switch(leftRight(overlappingMarker)) {
        case LEFT:
            // the overlapping marker is right hand side
            overlapLeftBoundary = leftBoundary;
            overlapRightBoundary = overlappingMarker->pos().x();
            break;
        case RIGHT:
            // the overlapping marker is left hand side
            overlapLeftBoundary = overlappingMarker->pos().x();
            overlapRightBoundary = rightBoundary;
            break;
        default :
            std::cerr << "Unknown direction!" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        /* Update position */
        setPos(boundarySnapPos(event->pos(), getClickPosOffsetX(event), overlapLeftBoundary, overlapRightBoundary));

    } else {
        // No overlap, new position is safe to use.
    }
    unsigned index = ((SlideBarItem*)parentItem())->markerIndex(this);
    setLastIndex(index);
    
//    std::cout << "Logical marker position: " << index << std::endl;
    //std::cout << "Computed real position for that index: " << ((SlideBarItem*)parentItem())->markerPositionForIndex(index) << std::endl;
//    std::cout << "boundary slider pos.x: " << pos().x() << std::endl;
    parentItem()->update(); // updates index slider as well    
}
