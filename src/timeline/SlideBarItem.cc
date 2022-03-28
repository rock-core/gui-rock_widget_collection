#include "SlideBarItem.h"

#include <QtWidgets>
#include <QGraphicsSceneMouseEvent>
#include <QStyle>

#include <iostream>

#include <base-logging/Logging.hpp>

SlideBarItem::SlideBarItem(unsigned startIndex, unsigned steps, unsigned stepSize, QGraphicsItem *parent) :
    QGraphicsItem(parent),height(10),width(100), color(QColor(Qt::gray)) 
{
    reconfigure(startIndex, steps, stepSize);
    slider = new IndexSlider(this, startIndex);
    allSliders.append(slider);
}

void SlideBarItem::reconfigure(unsigned startIndex, unsigned steps, unsigned stepSize) {
    LOG_DEBUG("Reconfiguring slidebar: startIndex=%d, steps=%d, stepSize=%d\n", startIndex, steps, stepSize);
    this->startIndex = startIndex;
    this->steps = steps;
    this->stepSize = stepSize;
}

QRectF SlideBarItem::boundingRect() const {
    return QRectF(0, 0, getWidth(), getHeight());
}

void SlideBarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(color));
    int center = getHeight() * 0.5;
    painter->fillRect(0, center, getWidth(), HEIGHT, color);
    Q_FOREACH(unsigned bookmark, bookmarks) {
        int x_pos = markerPositionForIndex(bookmark);
        int y_top = center-(getBookmarkHeight()/2)+SlideBarItem::HEIGHT/2;
        int y_bottom = y_top + getBookmarkHeight();
        painter->drawLine(x_pos, y_top, x_pos, y_bottom);
    }
}

unsigned SlideBarItem::getStartIndex() const {
    return startIndex;
}


qreal SlideBarItem::getBookmarkHeight() const {
    return bookmark_height;
}

void SlideBarItem::setBookmarkHeight(qreal bookmark_height) {
    this->bookmark_height = bookmark_height;
}

void SlideBarItem::setWidth(int width) {
    this->width = width;
}

void SlideBarItem::setHeight(int height) {
    this->height  = height;
}

int SlideBarItem::getWidth()const {
    return width;
}

int SlideBarItem::getHeight() const{
    return height;
}

void SlideBarItem::addTimeMarker(BoundarySlider* marker) {
    timeMarkers.append(marker);
    allSliders.append(marker);
    marker->setLastIndex(markerIndex(marker));
}

const QList<BoundarySlider*>& SlideBarItem::getTimeMarkers() {
    return timeMarkers;
}

const QList<Slider*>& SlideBarItem::getAllSliders() {
    return allSliders;
}

BoundarySlider* SlideBarItem::markerOverlap(BoundarySlider *marker) {
    Q_FOREACH(BoundarySlider* otherMarker, timeMarkers) {
        if(otherMarker == marker) {
            continue;
        }

        bool overlap = false;

        switch(marker->leftRight(otherMarker)) {
        case RIGHT :
            // marker is right hand side
            if(marker->pos().x() < otherMarker->pos().x()) {
                overlap = true;
            }
            break;
        case LEFT :
            // marker is left hand side or right on top
            if(marker->pos().x() >= otherMarker->pos().x()) {
                overlap = true;
            }
            break;
        default :
            std::cerr << "Unknown direction!" << std::endl;
            exit(EXIT_FAILURE);
        }

        if(marker->collidesWithItem(otherMarker) || overlap) {
            return otherMarker;
        }
    }
    return NULL;
}

bool SlideBarItem::markerContainsPoint(BoundarySlider *marker, const QPointF &point) {
    Q_FOREACH(BoundarySlider* otherMarker, timeMarkers) {
        if(otherMarker == marker) {
            continue;
        }
        if(otherMarker->contains(point)) {
            return true;
        }
    }
    return false;
}

unsigned SlideBarItem::markerIndex(Slider *marker) {
    //std::cout << "slidebar pos.x()" << pos().x() << std::endl;
    //std::cout << "marker pos.x()" << marker->pos().x()
    return (unsigned) QStyle::sliderValueFromPosition(startIndex, steps*stepSize, marker->pos().x(), getWidth());
}

int SlideBarItem::markerPositionForIndex(unsigned index) {
    return QStyle::sliderPositionFromValue(startIndex, steps*stepSize, index, getWidth());
}

IndexSlider* SlideBarItem::getIndexSlider() {
    return slider;
}

void SlideBarItem::updateBookmarks(QVector<int> & bookmarks) {
    this->bookmarks = bookmarks;
    update();
}

void SlideBarItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // Check if bookmark was clicked
    Q_FOREACH(int bookmark, bookmarks) {
        // Define click region for bookmarks
        QRectF clickRegion(markerPositionForIndex(bookmark)-2, pos().y() - (getBookmarkHeight()/2.0), 5, getBookmarkHeight());
        if(clickRegion.contains(event->pos())) {
            // Bookmark clicked!
            emit bookmarkClicked(bookmark);
            break;
        }
    }
}
