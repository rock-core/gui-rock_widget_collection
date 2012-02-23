#ifndef SLIDEBARITEM_H
#define SLIDEBARITEM_H

#include <QGraphicsItem>
#include "BoundarySlider.h"
#include "IndexSlider.h"

class SlideBarItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    
public:

    static const int HEIGHT = 3;

    SlideBarItem(unsigned startIndex, unsigned steps, unsigned stepSize, QGraphicsItem *parent = 0);
    void reconfigure(unsigned startIndex, unsigned steps, unsigned stepSize);
    
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    unsigned getStartIndex() const;
    qreal getBookmarkHeight() const;
    void setBookmarkHeight(qreal height);
    void setIndexSliderHeight(int height);
    void addTimeMarker(BoundarySlider *marker);
    const QList<BoundarySlider*>& getTimeMarkers();
    const QList<Slider*>& getAllSliders();
    BoundarySlider* markerOverlap(BoundarySlider *marker);
    bool markerContainsPoint(BoundarySlider *marker, const QPointF &point);
    unsigned markerIndex(Slider *marker);
    int markerPositionForIndex(unsigned index);
    IndexSlider* getIndexSlider();
    void updateBookmarks(QVector<int> & bookmarks);
    
    void setWidth(int width);
    void setHeight(int width);
    int getWidth()const;
    int getHeight()const;
    
signals:
    void bookmarkClicked(int idx);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    
private:
    qreal height;
    qreal width;
    qreal bookmark_height;
    unsigned startIndex, steps, stepSize;
    QColor color;
    QList<BoundarySlider*> timeMarkers;
    QList<Slider*> allSliders;
    QVector<int> bookmarks;
    IndexSlider *slider;
};

#endif // SLIDEBARITEM_H
