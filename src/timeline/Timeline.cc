#include "Timeline.h"
#include <iostream>
#include <algorithm>
#include <QPixmap>

#include <base-logging/Logging.hpp>


Timeline::Timeline(QWidget *parent) {
    setParent(parent);
    
    // Default property values
    bookmarkHeight = 6;
    marginTopBot = 5;
    marginLR = 5;
    bgColor = QColor(Qt::white);
    ordered_width = 400;
    minIndex = 0;
    steps = 10;
    stepSize = 1;
    
    reconfigure_slidebar = false;
    
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // Setup scene
    scene = new QGraphicsScene;
    scene->setBackgroundBrush(getBackgroundColor());
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Add items
    slidebar = new SlideBarItem(getMinIndex(), getSteps(), getStepSize());
    slidebar->setBookmarkHeight(getBookmarkHeight());
    slidebar->setWidth(ordered_width - getMarginLR()*2);
    slidebar->setHeight(ordered_width - height());
    
    scene->addItem(slidebar);
    
    startmarker = new BoundarySlider(slidebar, getMinIndex());
    slidebar->addTimeMarker(startmarker);
    
    endmarker = new BoundarySlider(slidebar, getMaxIndex());
    slidebar->addTimeMarker(endmarker);
    
    connect(slidebar->getIndexSlider(), SIGNAL(sliderReleased(Slider*, int)), this, SLOT(sliderReleased(Slider*, int)));
    connect(startmarker, SIGNAL(sliderReleased(Slider*, int)), this, SLOT(sliderReleased(Slider*, int)));
    connect(endmarker, SIGNAL(sliderReleased(Slider*, int)), this, SLOT(sliderReleased(Slider*, int)));
    connect(slidebar->getIndexSlider(), SIGNAL(sliderMoved(Slider*, int)), this, SLOT(sliderMoved(Slider*, int)));
    connect(startmarker, SIGNAL(sliderMoved(Slider*, int)), this, SLOT(sliderMoved(Slider*, int)));
    connect(endmarker, SIGNAL(sliderMoved(Slider*, int)), this, SLOT(sliderMoved(Slider*, int)));
    connect(slidebar->getIndexSlider(), SIGNAL(sliderClicked(Slider*)), this, SLOT(sliderClicked(Slider*)));
    connect(startmarker, SIGNAL(sliderClicked(Slider*)), this, SLOT(sliderClicked(Slider*)));
    connect(endmarker, SIGNAL(sliderClicked(Slider*)), this, SLOT(sliderClicked(Slider*)));
    connect(slidebar, SIGNAL(bookmarkClicked(int)), this, SLOT(bookmarkClickedSlot(int)));
    
    /* Timeout, at the moment only used for TestWidget */
    timer = new QTimer(this);
    timer->setInterval(25);
    connect(timer, SIGNAL(timeout()), this, SLOT(fireTimeout()));
    timer->start();
       
    updateScene();
}

int Timeline::getBookmarkHeight() const {
    return bookmarkHeight;
}

void Timeline::setBookmarkHeight(int height) {
    bookmarkHeight = height;
}

int Timeline::getMarginTopBot() {
    return marginTopBot;
}

void Timeline::setMarginTopBot(int margin) {
    marginTopBot = margin;
}

int Timeline::getMarginLR() {
    return marginLR;
}

void Timeline::setMarginLR(int margin) {
    marginLR = margin;
}

const QColor& Timeline::getBackgroundColor() const {
    return bgColor;
}

void Timeline::setBackgroundColor(const QColor & color) {
    bgColor = color;
}

int Timeline::getWidth() {
    return ordered_width;
}

void Timeline::setWidth(int width) {
    ordered_width = width;
}

int Timeline::getMinIndex() {
    return minIndex;
}

void Timeline::setMinIndex(int minIndex) {
    this->minIndex = minIndex;
    reconfigure_slidebar = true;
}

int Timeline::getSteps() {
    return steps;
}

void Timeline::setSteps(int steps) {
    this->steps = steps;
    reconfigure_slidebar = true;
}

int Timeline::getStepSize() {
    return stepSize;
}

void Timeline::setStepSize(int stepSize) {
    this->stepSize = stepSize;
    reconfigure_slidebar = true;
}

int Timeline::getSliderIndex() {
    return slidebar->getIndexSlider()->getLastIndex();
}

int Timeline::getStartMarkerIndex() {
    return startmarker->getLastIndex();
}

void Timeline::setStartMarkerIndex(int idx) {
    setSliderIndex(idx, startmarker);
    
}

int Timeline::getEndMarkerIndex() {
    return endmarker->getLastIndex();
}

void Timeline::setEndMarkerIndex(int idx) {
    setSliderIndex(idx, endmarker);
}

void Timeline::setSliderIndex(int idx) {
    setSliderIndex(idx, slidebar->getIndexSlider());
}

// This method is private!
void Timeline::setSliderIndex(int idx, Slider* slider) {
    // TODO check boundaries!!!
    slider->setLastIndex(idx);
    updateScene();
}

void Timeline::addBookmark(int idx) {
    if(hasBookmarkAtIndex(idx)) {
        return;
    }
    
    QVector<int> vec;
    vec << idx;
    addBookmarks(vec);
}

void Timeline::addBookmarks(QVector<int> bookmarks) {
    Q_FOREACH(int bookmark, bookmarks) {
        this->bookmarks << bookmark;
    }
    slidebar->updateBookmarks(this->bookmarks);
}

void Timeline::removeBookmark(int idx) {
    for(int i = 0; i < bookmarks.size(); i++) {
        if (bookmarks.at(i) == idx) {
            bookmarks.remove(i);    
            emit bookmarkRemoved(idx);
            slidebar->updateBookmarks(bookmarks);
            break;
        }
    }
}

void Timeline::removeAllBookmarks() {
    bookmarks.clear();
    slidebar->updateBookmarks(bookmarks);
}

bool Timeline::hasBookmarkAtIndex(int idx) {
    for(int i = 0; i < bookmarks.size(); i++) {
        if (bookmarks.at(i) == idx) {
            return true;
        }
    }
    return false;
}

void Timeline::resizeEvent(QResizeEvent * event) {
    QSize size = event->size();
    updateScene(size);
}

void Timeline::updateScene() {
    updateScene(size());
}

void Timeline::updateScene(QSizeF newSize) {
    if(reconfigure_slidebar) {
        reconfigureSlidebar();
    }
    
    scene->setBackgroundBrush(getBackgroundColor());
    scene->setSceneRect(0, 0, newSize.width(), newSize.height());
    
    slidebar->setWidth(newSize.width() - getMarginLR()*2);
    slidebar->setHeight(newSize.height() - getMarginTopBot()*2);
    slidebar->setPos(getMarginLR(),getMarginTopBot());

    Q_FOREACH(Slider *slider, slidebar->getAllSliders()) {
        slider->setPos(slidebar->markerPositionForIndex(slider->getLastIndex()),newSize.height()*0.5-getMarginTopBot());
    }
    update();
}

void Timeline::bookmarkClickedSlot(int idx) {
    LOG_DEBUG("BOOKMARK clicked at index %d", idx);
    emit bookmarkClicked(idx);
}

void Timeline::fireTimeout() {
    emit timeout();
}

void Timeline::sliderReleased(Slider* slider, int idx) {
    Slider* indexSlider = slidebar->getIndexSlider();
    
    if(slider == indexSlider) {
        emit indexSliderReleased(idx);
        LOG_DEBUG("INDEX SLIDER released at index %d", idx);
    } else if (slider == startmarker) {
        emit startMarkerReleased(idx);
        LOG_DEBUG("START MARKER released at index %d", idx);
    } else if (slider == endmarker) {
        emit endMarkerReleased(idx);
        LOG_DEBUG("END MARKER released at index %d", idx);
    } else {
        LOG_WARN("UNKNOWN SLIDER released at index %d", idx);
    }
}

void Timeline::sliderMoved(Slider* slider, int idx) {

    Slider* indexSlider = slidebar->getIndexSlider();
    if(slider == indexSlider) {
        emit indexSliderMoved(idx);
    } else if (slider == startmarker) {
        emit startMarkerMoved(idx);
    } else if (slider == endmarker) {
        emit endMarkerMoved(idx);
    } else {
        LOG_WARN("UNKNOWN SLIDER released at index %d", idx);
    }
}
void Timeline::sliderClicked(Slider* slider) {

    Slider* indexSlider = slidebar->getIndexSlider();
    if(slider == indexSlider) {
        emit indexSliderClicked();
    } else if (slider == startmarker) {
        emit startMarkerClicked();
    } else if (slider == endmarker) {
        emit endMarkerClicked();
    } else {
        LOG_WARN("UNKNOWN SLIDER clicked");
    }
}
void Timeline::reconfigureSlidebar() {
    reconfigure_slidebar = false;
    slidebar->reconfigure(getMinIndex(), getSteps(), getStepSize());
    setStartMarkerIndex(getMinIndex());
    setEndMarkerIndex(getMaxIndex());
}

int Timeline::getMaxIndex() {
    return getMinIndex() + getSteps() * getStepSize();
}
