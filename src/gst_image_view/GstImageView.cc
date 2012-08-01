#include "GstImageView.h"

#include <base/logging.h>
#include <iostream>

#include <QtOpenGL/QGLWidget>

#ifdef USE_GST
    #include <QGst/Init>
    #include <QGst/Parse>
    #include <QGst/Pipeline>
    #include <QGst/ElementFactory>
    #include <QGst/Ui/VideoWidget>
    #include <QGst/Ui/GraphicsVideoSurface>
    #include <QGst/Ui/GraphicsVideoWidget>
#endif

GstImageView::GstImageView(QWidget *parent)
    : QWidget(parent), bgColor(QColor(Qt::darkGray)), progress_indicator_timeout(5000), use_gl(false), pipelineDescription("videotestsrc ! ximagesink") //qtglvideosink
{
    resize(500,500);
    imageItem = NULL;
    
    /* Configure progress indicator */
    progress_indicator = new ProgressIndicator;
    progress_indicator->setAnimationDelay(40);
    progress_indicator->setDisplayedWhenStopped(false);
    progress_indicator->resize(30,30);
    progress_indicator->show();
    
    /* Set up timer for progress indicator */
    progress_indicator_timer = new QTimer(this);
    progress_indicator_timer->setInterval(getProgressIndicatorTimeout());
    connect(progress_indicator_timer, SIGNAL(timeout()), progress_indicator, SLOT(startAnimation()));
    
#ifdef USE_GST
    QGst::PipelinePtr pipeline;
    QGst::ElementPtr videoSrc;
        /* GStreamer setup */
        QGst::init();

//         pipeline = QGst::Parse::launch(pipelineDescription).dynamicCast<QGst::Pipeline>();
        pipeline = QGst::Pipeline::create("pipeline"); // Create hard coded pipeline for debugging
        videoSrc = QGst::ElementFactory::make("videotestsrc");
#endif

    
    /* Setup video sink */
    scene = new QGraphicsScene;
    scene->setBackgroundBrush(getBackgroundColor());
    
    view = new QGraphicsView(scene, this);
    progress_indicator->setParent(view);
    
    if(use_gl) {
        // Indicator to use qtglvideosink (hardware rendering!) instead of qtvideosink
        view->setViewport(new QGLWidget); 
    }
    
    view->setAlignment(Qt::AlignCenter);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

#ifdef USE_GST    
    QGst::Ui::GraphicsVideoSurface *surface = NULL;
    QGst::Ui::GraphicsVideoWidget *widget = NULL;
    
    surface = new QGst::Ui::GraphicsVideoSurface(view); 
    widget = new QGst::Ui::GraphicsVideoWidget;
    widget->setSurface(surface);
    
    scene->addItem(widget);

    // Connect hard coded pipeline
    pipeline->add(videoSrc, surface->videoSink());
    videoSrc->link(surface->videoSink());
    
    LOG_INFO_S << "Video source name: " << surface->videoSink()->property("name").toString().toStdString();
    
    /* Try to start playing */
    if(!pipeline->setState(QGst::StatePlaying)) {
        // TODO proper fallback handling
        LOG_WARN("Could not play pipeline.");
    }
#else
    imageItem = new QGraphicsPixmapItem;
    scene->addItem(imageItem);
#endif
    
    //view->resize(400,400);
//     widget->resize(300,300);

    setItemPositions();
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
    delete progress_indicator;
    delete progress_indicator_timer;
}

const QColor& GstImageView::getBackgroundColor() const
{
    return bgColor;
}

void GstImageView::setBackgroundColor(const QColor & color)
{
    bgColor = color;
    scene->setBackgroundBrush(bgColor);
}

const QString GstImageView::getPipelineDescription() const
{
    return pipelineDescription;
}

void GstImageView::setPipelineDescription(QString descr)
{
    this->pipelineDescription = descr;
}

const int GstImageView::getProgressIndicatorTimeout() const
{
    return progress_indicator_timeout;
}

void GstImageView::setProgressIndicatorTimeout(int timeout)
{
    progress_indicator_timeout = timeout;
    progress_indicator_timer->setInterval(timeout);
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
            new QGraphicsEllipseItem(QRectF(center.x() - radius/2.0, center.y() - radius/2.0, radius, radius));
    addDrawItem(circlePtr, persistent);
}

void GstImageView::addLine(QLineF &line, bool persistent)
{
    //QLineF mappedLine;
    //mappedLine.setPoints(imageItem->mapFromItem(imageItem, line.p1()), 
    //                     imageItem->mapFromItem(imageItem, line.p2()));
                        
    QGraphicsLineItem *linePtr = new QGraphicsLineItem(line);
    //std::cout << "lineptr=" << linePtr << std::endl;
    
    //scene->addLine(mappedLine);
    //scene->addLine(line);
    addDrawItem(linePtr, persistent);

    //addDrawItem(linePtr);
}

void GstImageView::addText(QString text, TextLocation location, bool persistent)
{
    QGraphicsSimpleTextItem *textOverlay = new QGraphicsSimpleTextItem(text);
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
        Q_FOREACH(QGraphicsItem *item, persistentDrawItems) {
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
    view->rotate(deg);
    view->fitInView(imageItem, Qt::KeepAspectRatio);
    //view->fitInView(view->sceneRect(), Qt::KeepAspectRatio);
}

void GstImageView::setFrame(const base::samples::frame::Frame &frame)
{   
    progress_indicator_timer->start();
    progress_indicator->stopAnimation();
    
#ifdef USE_GST
    // TODO
#else
    clearOverlays();

    if(1 == frame_converter.copyFrameToQImageRGB888(image,frame)) {
        LOG_WARN("Frame size changed while converting frame to QImage (says converter)");
    }

    // Backup image size for detecting size change
    QSize oldSize = imageSize;
    imageSize = image.size();
    //std::cout << "Image size: x=" << image.width() <<", y=" << image.height() << std::endl;
    QPixmap pixmap = QPixmap::fromImage(image);
    pixmap.scaled(size(), Qt::KeepAspectRatio);

    //std::cout << "Pixmap size: x=" << pixmap.width() <<", y=" << pixmap.height() << std::endl;
    if(imageItem) {
        imageItem->setPixmap(pixmap);
    } else {
        LOG_WARN("imageItem undefined!");
    }
    
    //std::cout << "ImageItem BoundingRect: x=" << imageItem->boundingRect().width() <<", y=" << imageItem->boundingRect().height() << std::endl;
    addText(QString::fromStdString(frame.time.toString()), TOPRIGHT, 0);
    
    /* Resize and repositioning if frame size changes (and on start) */
    if(imageSize != oldSize) {
        LOG_INFO("image size changed. resize.");
        setVisible(false); setVisible(true); // For some reason the first frame is not scaled to fit the full space. Calling update or even repaint did not help but a simple resize or setVisible does.
        setItemPositions();
    }
    
    update();
#endif
}

/* PROTECTED METHODS ---------------------------------------------------------- */

void GstImageView::resizeEvent(QResizeEvent *event)
{ 
    QWidget::resizeEvent(event);
    view->resize(event->size());

//    std::cout << "resize event.\nview.width: " << view->width() << "\n"
//              << "view.height: " << view->height() << std::endl;
    
    view->fitInView(imageItem, Qt::KeepAspectRatio);
    setItemPositions();
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
    scene->addItem(item);
    update();
}

void GstImageView::update2()
{
    QWidget::update();
}

void GstImageView::setItemPositions()
{
//    std::cout << "setItemPositions" << std::endl;
    // Align to top-left corner of view (scene gets always fit in view)   
    progress_indicator->move(view->width() - progress_indicator->width(), 0);
}
