#include "GraphicsPointsItem.h"
#include <limits>

#include <base/logging.h>

GraphicsPointsItem::GraphicsPointsItem(const QList<QPoint> &points):points(points)
{
}

QRectF GraphicsPointsItem::calcBoundingRect() const
{
    QPoint left_top(std::numeric_limits< int >::max(),
                    std::numeric_limits< int >::max());
    QPoint right_bottom(0,0);
    QList<QPoint>::const_iterator iter = points.begin();
    for(;iter != points.end();++iter)
    {
        if(iter->x() < left_top.x())
            left_top.setX(iter->x());
        if(iter->y() < left_top.y())
            left_top.setY(iter->y());
        if(iter->x() > right_bottom.x())
            right_bottom.setX(iter->x());
        if(iter->y() > right_bottom.y())
            right_bottom.setY(iter->y());
    }
//     LOG_DEBUG_S << "GraphicsPointsItem: calculated bounding rect to left_top: (" << left_top.x() << "," << left_top.y() << ")/(" <<
//                                                                         right_bottom.x() << ", " << right_bottom.y() << ") right bottom";
    
    QRect rect(left_top,right_bottom);
    
    // Adjust rect to pen width
    int penwidth = pen.width();
    LOG_DEBUG_S << "GraphicsPointsItem: pen width: " << penwidth;
    rect = QRect(left_top.x()-penwidth/2, left_top.y()-penwidth/2,
                 rect.width()+penwidth, rect.height()+penwidth);
    
//     LOG_DEBUG_S << "GraphicsPointsItem: corrected bounding rect to left_top: (" << left_top.x() << "," << left_top.y() << ")/(" <<
//                                                                         right_bottom.x() << ", " << right_bottom.y() << ") right bottom";
    
    return rect;
}

QRectF GraphicsPointsItem::boundingRect() const
{
    return calcBoundingRect();
}

void GraphicsPointsItem::setPen(const QPen &pen)
{
    this->pen = pen;
}

void GraphicsPointsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen);
    QList<QPoint>::iterator iter = points.begin();
    for(;iter != points.end();++iter)
        painter->drawPoint(*iter);
}

