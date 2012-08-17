#include "GraphicsPointsItem.h"
#include <limits>

GraphicsPointsItem::GraphicsPointsItem(const QList<QPoint> &points):points(points)
{
    bounding_rect = calcBoundingRect();
}

QRectF GraphicsPointsItem::calcBoundingRect() const
{
    QPoint left_top(std::numeric_limits<qreal>::max(),
                    std::numeric_limits<qreal>::max());
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
    return QRect(left_top,right_bottom);
}

QRectF GraphicsPointsItem::boundingRect() const
{
    return bounding_rect;
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

