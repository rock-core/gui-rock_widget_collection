#ifndef BOUNDARY_SLIDER_H
#define BOUNDARY_SLIDER_H

#include "Slider.h"

enum Direction {
    LEFT,
    RIGHT
};

class BoundarySlider : public Slider
{
public:

    BoundarySlider();
    BoundarySlider(QGraphicsItem* parent, unsigned initIdx);

    Direction leftRight(BoundarySlider *otherMarker);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

};

#endif // BOUNDARY_SLIDER_H
