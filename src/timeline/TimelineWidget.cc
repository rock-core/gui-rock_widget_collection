#include "TimelineWidget.h"
#include <iostream>
#include <algorithm>
#include <QPixmap>


TimelineWidget::TimelineWidget(qreal width, unsigned startIndex, unsigned steps, unsigned stepSize, QWidget *parent) {
    setParent(parent);
    
    // Default property values
    bookmarkHeight = 6;
    indexSliderHeight = 40;
    marginTopBot = 5;
    marginLR = 5;
    bgColor = QColor(Qt::white);
    
    // Setup scene
    ordered_width = width;
    scene = new QGraphicsScene;
    scene->setBackgroundBrush(getBackgroundColor());
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // Add items
    slidebar = new SlideBarItem(startIndex, steps, stepSize);
    slidebar->setBookmarkHeight(getBookmarkHeight());
    slidebar->setOrderedWidth(ordered_width - getMarginLR()*2);
    
    //slidebar->setPos(-(slidebar->getWidth() / 2), 0);
    //slidebar->setPos(0,0);
    scene->addItem(slidebar);
    
    startmarker = new BoundarySlider(slidebar, QPointF(0, SlideBarItem::HEIGHT - 1));
   // startmarker->setLastIndex(1);
    slidebar->addTimeMarker(startmarker);
    
    endmarker = new BoundarySlider(slidebar, QPointF(250, SlideBarItem::HEIGHT - 1));
    //endmarker->setLastIndex(4);
    slidebar->addTimeMarker(endmarker);
    
    timer = new QTimer(this);
    timer->setInterval(25);
    connect(timer, SIGNAL(timeout()), this, SLOT(fireTimeout()));
    timer->start();
    
    
    updateScene();
    
}

int TimelineWidget::getBookmarkHeight() const {
    return bookmarkHeight;
}

void TimelineWidget::setBookmarkHeight(int height) {
    bookmarkHeight = height;
}

int TimelineWidget::getIndexSliderHeight() const {
    return indexSliderHeight;
}

void TimelineWidget::setIndexSliderHeight(int height) {
    indexSliderHeight = height;
}

int TimelineWidget::getMarginTopBot() {
    return marginTopBot;
}

void TimelineWidget::setMarginTopBot(int margin) {
    marginTopBot = margin;
}

int TimelineWidget::getMarginLR() {
    return marginLR;
}

void TimelineWidget::setMarginLR(int margin) {
    marginLR = margin;
}

const QColor& TimelineWidget::getBackgroundColor() const {
    return bgColor;
}

void TimelineWidget::setBackgroundColor(const QColor & color) {
    bgColor = color;
}

int TimelineWidget::getSliderIndex() {
    return slidebar->markerIndex(slidebar->getIndexSlider());
}
int TimelineWidget::getStartMarkerIndex() {
    return slidebar->markerIndex(startmarker);
}
int TimelineWidget::getEndMarkerIndex() {
    return slidebar->markerIndex(endmarker);
}

void TimelineWidget::setSliderIndex(int idx) {
    std::cout << "Setting slider index to " << idx << std::endl;
    setSliderIndex(idx, slidebar->getIndexSlider());
}

// This method is private!
void TimelineWidget::setSliderIndex(int idx, Slider* slider) {
    // TODO check boundaries!!!
    slider->setLastIndex(idx);
    //startmarker->setLastIndex(idx); // TODO ONLY FOR DEBUGGING!!! REMOVE!!!
    updateScene();
}

void TimelineWidget::addBookmark(int idx) {
    QVector<int> vec;
    vec << idx;
    addBookmarks(vec);
}

void TimelineWidget::addBookmarks(QVector<int> bookmarks) {
    Q_FOREACH(int bookmark, bookmarks) {
        this->bookmarks << bookmark;
    }
    slidebar->updateBookmarks(this->bookmarks);
}

void TimelineWidget::removeBookmark(int idx) {
    for(int i = 0; i < bookmarks.size(); i++) {
        // std::cout << "idx = " << idx << std::endl;
        // std::cout << "bookmarks.at(i) = " << bookmarks.at(i) << std::endl;
        if (bookmarks.at(i) == idx) {
            // std::cout << "Removing " << idx << std::endl;
            bookmarks.remove(i);    
            emit bookmarkRemoved(idx);
            slidebar->updateBookmarks(bookmarks);
            break;
        }
    }
}

void TimelineWidget::resizeEvent(QResizeEvent * event) {
    // TODO Update all items in the scene
    
    QSize size = event->size();
    
    std::cout << "ResizeEvent!" << std::endl;
    std::cout << "TimelineWidget newsize: H=" << size.height() << ", W=" << size.width() << std::endl;
    
    QSizeF sceneSize = sceneRect().size();
    std::cout << "Current scenerect size: H=" << sceneSize.height() << ", W=" << sceneSize.width() << std::endl;
    
    updateScene(size);
}

void TimelineWidget::updateScene() {
    updateScene(size());
}

void TimelineWidget::updateScene(QSizeF newSize) {
    scene->setBackgroundBrush(getBackgroundColor());
    qreal sceneHeight = std::max(getBookmarkHeight(), getIndexSliderHeight()) + getMarginTopBot()*2;
//    qreal sceneWidth = this->width();
    qreal sceneWidth = newSize.width() + getMarginLR()*2;//ordered_width;//std::max((double)SlideBarItem::DESIGNED_WIDTH, (double)width) + PADDING;
    //scene = new QGraphicsScene(-sceneWidth/2.0, -sceneHeight/2.0, sceneWidth, sceneHeight); // TODO fit correct size
    setMaximumHeight(sceneHeight);
    setMinimumHeight(sceneHeight);
//    setMinimumWidth(sceneWidth);
    scene->setSceneRect(-getMarginLR(), -sceneHeight/2.0, sceneWidth, sceneHeight);
    
    slidebar->setOrderedWidth(newSize.width() - getMarginLR()*2);
    std::cout << "updateScene(): updating slider positions" << std::endl;
    Q_FOREACH(Slider *slider, slidebar->getAllSliders()) {
        //bs.setPos(bs.boundarySnapPos(slidebar->markerPositionForIndex(bs.g)))
        unsigned index = slidebar->markerIndex(slider);
        std::cout << "Logical marker position: " << index << ", old pos: " << slider->getLastIndex() << std::endl;
        
//// **** TODO Tried to remove pointing offset and do boundary safe positioning but there seems to be a coordinate system mismatch. ****
//        slider->setPos(slider->boundarySnapPos(QPointF(slidebar->markerPositionForIndex(slider->getLastIndex()), slider->pos().y()),
//                                                    0,
//                                                    slidebar->boundingRect().left(),
//                                                    slidebar->boundingRect().right())
//                                                );
        std::cout << "old slider xPos: " << slider->pos().x() << ", new pos: " << slidebar->markerPositionForIndex(slider->getLastIndex()) << std::endl;
        slider->setPos(slidebar->markerPositionForIndex(slider->getLastIndex()), slider->pos().y());
    }

    std::cout << "this->height(): " << this->height() << std::endl;
    std::cout << "this->width(): " << this->width() << std::endl;
    std::cout << "this->viewport()->size().width()" << this->viewport()->size().width() << std::endl;
    
    update();

}

void TimelineWidget::fireTimeout() {
    emit timeout();
}