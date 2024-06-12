#ifndef GRAPHICSPOINTSITEM_H__
#define GRAPHICSPOINTSITEM_H__

#include <QtWidgets>
#include <QtCore>
#include <QGraphicsItem>


// Library representing points which can be added to GraphicScene
// The main propose is :wa
class GraphicsPointsItem : public QGraphicsItem
{
    public:
        GraphicsPointsItem(const QList<QPoint> &points);
        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void setPen(const QPen &pen);

    private:
        QRectF calcBoundingRect() const;

    private:
        QList<QPoint> points;
        QPen pen;
};

#endif
