#include "GstImageView.h"

#include <base/logging.h>
#include <iostream>

#include <QtOpenGL/QGLWidget>

#include <QGst/Init>
#include <QGst/Parse>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Ui/VideoWidget>
#include <QGst/Ui/GraphicsVideoSurface>
#include <QGst/Ui/GraphicsVideoWidget>


GstImageView::GstImageView(QWidget *parent)
    : QWidget(parent)
{
    resize(500,500);
    imageItem = NULL;
    
    /* Default widget property values */
    /* TODO does this overwrite the user's widget property setting? */
    
    // TODO Currently using hard coded pipeline instead of text based pipeline description from property
    pipelineDescription = "videotestsrc ! ximagesink";//qtglvideosink"; 
    use_gl = false;
    use_gst = false;

    QGst::PipelinePtr pipeline;
    QGst::ElementPtr videoSrc;
    if(use_gst) {
        /* GStreamer setup */
        QGst::init();

//         pipeline = QGst::Parse::launch(pipelineDescription).dynamicCast<QGst::Pipeline>();
        pipeline = QGst::Pipeline::create("pipeline"); // Create hard coded pipeline for debugging
        videoSrc = QGst::ElementFactory::make("videotestsrc");
    }

    
    /* Setup video sink */
    scene = new QGraphicsScene;
    //scene->setBackgroundBrush(Qt::darkGray);
    
    view = new QGraphicsView(scene, this);
    
    if(use_gl) {
        // Indicator to use qtglvideosink (hardware rendering!) instead of qtvideosink
        view->setViewport(new QGLWidget); 
    }
    
    view->setAlignment(Qt::AlignCenter);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    QGst::Ui::GraphicsVideoSurface *surface = NULL;
    QGst::Ui::GraphicsVideoWidget *widget = NULL;
    
    if(use_gst) {
        surface = new QGst::Ui::GraphicsVideoSurface(view); 
        widget = new QGst::Ui::GraphicsVideoWidget;
        widget->setSurface(surface);
        
        scene->addItem(widget);

        // Connect hard coded pipeline
        pipeline->add(videoSrc, surface->videoSink());
        videoSrc->link(surface->videoSink());
        
        std::cout << "Video source name: " << surface->videoSink()->property("name").toString().toStdString() << std::endl;
        
        /* Try to start playing */
        if(!pipeline->setState(QGst::StatePlaying)) {
            // TODO proper fallback handling
            std::cout << "Could not play pipeline." << std::endl;
        }
    } else {
        imageItem = new QGraphicsPixmapItem;
        scene->addItem(imageItem);
    }
    
    view->resize(400,400);
//     widget->resize(300,300);
    update();
}

GstImageView::~GstImageView()
{
    Q_FOREACH(QGraphicsItem *item, persistentDrawItems + volatileDrawItems) {
        delete item;
    }
    
    delete imageItem;
    delete scene;
    delete view;
    // pipeline->setState(QGst::StateNull);
}


QString GstImageView::getPipelineDescription()
{
    return pipelineDescription;
}

void GstImageView::setPipelineDescription(QString descr)
{
    this->pipelineDescription = descr;
}

bool GstImageView::getUseGst()
{
    return this->use_gst;
}

void GstImageView::setUseGst(bool use_gst)
{
    this->use_gst = use_gst;
}

bool GstImageView::getUseGl()
{
    return this->use_gl;
}

void GstImageView::setUseGl(bool use_gl)
{
    this->use_gl = use_gl;
}







void GstImageView::addCircle(QPointF center, double radius, bool persistent)
{
    QGraphicsEllipseItem *circlePtr = 
            new QGraphicsEllipseItem(QRectF(center.x() - radius/2.0, center.y() - radius/2.0, radius, radius), 
                                     imageItem);
    addDrawItem(circlePtr, persistent);
}

void GstImageView::addLine(QLineF &line, bool persistent)
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

void GstImageView::addText(QString text, TextLocation location, bool persistent)
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

void GstImageView::clearOverlays(bool clear_persistent_items)
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

void GstImageView::rotate(int deg)
{
    std::cout << "rotating " << deg << " degrees." << std::endl;
    view->rotate(deg);
    view->fitInView(imageItem, Qt::KeepAspectRatio);
    //view->fitInView(view->sceneRect(), Qt::KeepAspectRatio);
}

void GstImageView::setFrame(const base::samples::frame::Frame &frame)
{   
    std::cout << "GstImageView: Got frame!" << std::endl;
    
    if(use_gst) {
        // TODO
    } else {
        clearOverlays();

        if(1 == frame_converter.copyFrameToQImageRGB888(image,frame)) {
            LOG_DEBUG("Frame size changed while converting frame to QImage (says converter)");
        }
        imageSize = image.size();
        //std::cout << "Image size: x=" << image.width() <<", y=" << image.height() << std::endl;
        QPixmap pixmap = QPixmap::fromImage(image);
        pixmap.scaled(size(), Qt::KeepAspectRatio);

        //std::cout << "Pixmap size: x=" << pixmap.width() <<", y=" << pixmap.height() << std::endl;
        if(imageItem) {
            imageItem->setPixmap(pixmap);
        } else {
            std::cout << "imageItem undefined!" << std::endl;
        }
        
        //std::cout << "ImageItem BoundingRect: x=" << imageItem->boundingRect().width() <<", y=" << imageItem->boundingRect().height() << std::endl;
        addText(QString::fromStdString(frame.time.toString()), TOPRIGHT, 0);
    }
    update();
}

void GstImageView::resizeEvent(QResizeEvent *event)
{ 
    QWidget::resizeEvent(event);
    std::cout << "ResizeEvent" << std::endl;
    
    view->resize(event->size());

    //imageItem->setPixmap(QPixmap::fromImage(image.scaled(event->size(), Qt::KeepAspectRatio)));
    
    view->fitInView(imageItem, Qt::KeepAspectRatio);
}


/* PRIVATE METHODS ---------------------------------------------------------- */

void GstImageView::addDrawItem(QGraphicsItem *item, bool persistent)
{   
    //std::cout << "addDrawItem: adding item: " << item << std::endl;
    
    if(persistent) {
        persistentDrawItems.push_back(item);
    } else {
        volatileDrawItems.push_back(item);
    }
    update();
}

void GstImageView::update2()
{
    std::cout << "GstImageView: update2()" << std::endl;
    QWidget::update();
}