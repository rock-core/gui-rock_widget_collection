#include "NewImageView.h"

#include <base/logging.h>
#include <iostream>
#include <cstdlib>

NewImageView::NewImageView(QWidget *parent)
    : QWidget(parent)
{
    //setMinimumSize(500,400);
    
    view = new QGraphicsView(this);
    view->setAlignment(Qt::AlignCenter);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //view->resize(500,400);
    
    scene = new QGraphicsScene;
    scene->setBackgroundBrush(Qt::darkGray);
    view->setScene(scene);

    // Load default image; TODO for debugging, remove
//    const char* path = ":/artificial_horizon/icon.png";
//    image.load(path);
//    if(image.isNull()) {
//        LOG_ERROR("Could not load image from %s", path);
//        exit(EXIT_FAILURE);
//    }
                    
    imageItem = new QGraphicsPixmapItem;//(QPixmap::fromImage(image));
    scene->addItem(imageItem);
    

    
    
    //view->fitInView(imageItem, Qt::KeepAspectRatio);

    resize(500,400);
    update();
}

NewImageView::~NewImageView()
{
    
    
    Q_FOREACH(QGraphicsItem *item, persistentDrawItems + volatileDrawItems) {
        delete item;
    }
    
    delete imageItem;
    delete scene;
    delete view;
//    delete imageLabel;
//    delete scrollArea;
}

void NewImageView::addCircle(QPointF center, double radius, bool persistent)
{
    QGraphicsEllipseItem *circlePtr = 
            new QGraphicsEllipseItem(QRectF(center.x() - radius/2.0, center.y() - radius/2.0, radius, radius), 
                                     imageItem);
    addDrawItem(circlePtr, persistent);
}

void NewImageView::addLine(QLineF &line, bool persistent)
{
    //QLineF mappedLine;
    //mappedLine.setPoints(imageItem->mapFromItem(imageItem, line.p1()), 
    //                     imageItem->mapFromItem(imageItem, line.p2()));
                        
    QGraphicsLineItem *linePtr = new QGraphicsLineItem(line, imageItem);
    //std::cout << "lineptr=" << linePtr << std::endl;
    
    //scene->addLine(mappedLine);
    //scene->addLine(line);
    addDrawItem(linePtr, persistent);

    //addDrawItem(linePtr);
}

void NewImageView::addText(QString text, TextLocation location, bool persistent)
{
    QGraphicsSimpleTextItem *textOverlay = new QGraphicsSimpleTextItem(text, imageItem);
    switch(location) {
    case TOPLEFT :
        textOverlay->show();
        textOverlay->setZValue(10);
        textOverlay->setBrush(Qt::red);
        break;
    default:
        LOG_WARN("Unsupported text location. Switching to TOPLEFT.");
        delete textOverlay;
        addText(text, TOPLEFT, persistent);
        return;
    }
    
    addDrawItem(textOverlay, persistent);
}

void NewImageView::clearOverlays(bool clear_persistent_items)
{
    if(clear_persistent_items) {
        Q_FOREACH(QGraphicsItem *item, persistentDrawItems + volatileDrawItems) {
            scene->removeItem(item);
        }
        persistentDrawItems.clear();
    }
    
    Q_FOREACH(QGraphicsItem *item, volatileDrawItems) {
        scene->removeItem(item);
    }
    volatileDrawItems.clear();
}

void NewImageView::rotate(int deg)
{
    std::cout << "rotating " << deg << " degrees." << std::endl;
    view->rotate(deg);
    view->fitInView(imageItem, Qt::KeepAspectRatio);
    //view->fitInView(view->sceneRect(), Qt::KeepAspectRatio);
}

void NewImageView::setFrame(const base::samples::frame::Frame &frame)
{
    //std::cout << "setFrame!" << std::endl;
    
    clearOverlays();
    
    if(1 == frame_converter.copyFrameToQImageRGB888(image,frame)) {
        LOG_DEBUG("Frame size changed while converting frame to QImage (says converter)");
    }
    imageSize = image.size();
    //std::cout << "Image size: x=" << image.width() <<", y=" << image.height() << std::endl;
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap.scaled(size(), Qt::KeepAspectRatio);
    
    //std::cout << "Pixmap size: x=" << pixmap.width() <<", y=" << pixmap.height() << std::endl;
    imageItem->setPixmap(pixmap);
    //std::cout << "ImageItem BoundingRect: x=" << imageItem->boundingRect().width() <<", y=" << imageItem->boundingRect().height() << std::endl;
    addText(QString::fromStdString(frame.time.toString()), TOPRIGHT, 0);
    update();
}

void NewImageView::update2()
{
    QWidget::update();
}

//void NewImageView::paintEvent(QPaintEvent *event)
//{
//    std::cout << "PaintEvent!" << std::endl;
//    QPainter painter(this);
//    painter.set
//    painter.drawText(QRectF(20,20,80,80), Qt::AlignRight, "Right aligned text");
//    //update();
//}

void NewImageView::resizeEvent(QResizeEvent *event)
{ 
    QWidget::resizeEvent(event);
    std::cout << "ResizeEvent" << std::endl;
//    int width = event->size().width();
//    int height = heightForWidth(width);
    
//    view->resize(width, height);
    
    view->resize(event->size());

    //imageItem->setPixmap(QPixmap::fromImage(image.scaled(event->size(), Qt::KeepAspectRatio)));
    
    view->fitInView(imageItem, Qt::KeepAspectRatio);
//    QTransform transform = imageItem->transform();
//    Q_FOREACH(QGraphicsItem *item, drawItems) {
//        item->setTransform(transform); 
//    }
    
    //view->fitInView(view->sceneRect(), Qt::KeepAspectRatio);
   
}

//QSize NewImageView::fittingSize(QSize& proposed_size)
//{   
//    // TODO: IMPLEMENT!!!
//    QSize fittingSize;
//    int viewport_width = view->viewport()->width();
//    
//    int width = proposed_size.width() < viewport_width ? proposed_size.width() : viewport_width;
//    return fittingSize;
//}

//int NewImageView::heightForWidth(int width)
//{
//    // For keeping aspect ratio
//    double factor = (double) imageSize.width() / (double) imageSize.height();
//    int height = (double) size().width() / factor;
//    return height;
//}


/* PRIVATE METHODS ---------------------------------------------------------- */
void NewImageView::addDrawItem(QGraphicsItem *item, bool persistent)
{   
    //std::cout << "addDrawItem: adding item: " << item << std::endl;
    
    if(persistent) {
        persistentDrawItems.push_back(item);
    } else {
        volatileDrawItems.push_back(item);
    }
    update();
}