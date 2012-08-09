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
    
    setupContextMenu();
    
#ifdef USE_GST
    QGst::PipelinePtr pipeline;
    QGst::ElementPtr videoSrc;
        /* GStreamer setup */
        QGst::init();

//         pipeline = QGst::Parse::launch(pipelineDescription).dynamicCast<QGst::Pipeline>();
        pipeline = QGst::Pipeline::create("pipeline"); // Create hard coded pipeline for debugging
        videoSrc = QGst::ElementFactory::make("videotestsrc");
#endif

    /* Setup inner graphicsscene and view. This scene contains the image and image related overlays. */
    imageScene = new QGraphicsScene;
    imageScene->setBackgroundBrush(getBackgroundColor());
    
    imageView = new QGraphicsView(imageScene);
    
    if(use_gl) {
        // Indicator to use qtglvideosink (hardware rendering!) instead of qtvideosink
        imageView->setViewport(new QGLWidget); 
    }
    
    imageView->setAlignment(Qt::AlignCenter);
    imageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    imageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    imageView->setFocusPolicy(Qt::NoFocus);

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
    imageScene->addItem(imageItem);
#endif
    
    /* Setup outer graphicsscene and view. This scene contains fixed overlays like the 
     * image timestamp which are immune to rotation etc. i.e. they stay at the same place
     * nevertheless the inner scene gets rotated or scaled.
     */
    fixedOverlayScene = new QGraphicsScene;
    fixedOverlayScene->setBackgroundBrush(getBackgroundColor());
    
    fixedOverlayView = new QGraphicsView(fixedOverlayScene, this);
    fixedOverlayView->setAlignment(Qt::AlignCenter);
    fixedOverlayView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fixedOverlayView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fixedOverlayView->setFocusPolicy(Qt::NoFocus);
    
    // Nest inner view in outer scene
    imageViewProxy = fixedOverlayScene->addWidget(imageView);
    
    progress_indicator->setParent(fixedOverlayView);
    
    
    setItemPositions();
    update();
}

GstImageView::~GstImageView()
{
    Q_FOREACH(QGraphicsItem *item, persistentDrawItems + volatileDrawItems) {
        delete item;
    }
    
    delete imageItem;
    delete imageScene;
    delete imageView;
    delete imageViewProxy;
    delete fixedOverlayScene;
    delete fixedOverlayView;
    // pipeline->setState(QGst::StateNull);
    delete progress_indicator;
    delete progress_indicator_timer;
    
    delete contextMenu;
    delete rotate_image_clockwise_act;
    delete rotate_image_counterclockwise_act;
    delete rotate_image_180_act;
    delete save_image_act;
    delete save_image_overlay_act;
}

const QColor& GstImageView::getBackgroundColor() const
{
    return bgColor;
}

void GstImageView::setBackgroundColor(const QColor & color)
{
    bgColor = color;
    imageScene->setBackgroundBrush(bgColor);
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
    addDrawItem(imageScene, circlePtr, persistent);
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
    addDrawItem(imageScene, linePtr, persistent);

    //addDrawItem(linePtr);
}

void GstImageView::addText(QString text, TextLocation location, bool persistent)
{
    QGraphicsSimpleTextItem *textOverlay = new QGraphicsSimpleTextItem(text);
    textOverlay->show();
    textOverlay->setZValue(10);
    textOverlay->setBrush(Qt::black);

    QFont font;
    font.setPointSize(12);
    //std::cout << "text overlay pixel size: " << height()/20 << " (desired), " << font.pixelSize() << " (real)" << std::endl;
    textOverlay->setFont(font);
    
    /* Create transparent text background */
    Qt::GlobalColor bgColor = Qt::white;
    qreal opacity = 0.7;
    int padding = 0; //5
    
    QColor penColor(bgColor);
    penColor.setAlphaF(opacity);
    QPen pen;
    pen.setColor(penColor);
    pen.setWidth(padding);
    
    QGraphicsRectItem *textBackground = new QGraphicsRectItem(textOverlay->boundingRect());
    textBackground->setBrush(bgColor);
    textBackground->setPen(pen); // increase margin. like cellpadding in HTML tables.
    textBackground->setOpacity(opacity);
    
    textOverlay->stackBefore(textBackground);
/*    
    switch(location) {
    case TOPLEFT :

        break;
    default:
        LOG_WARN("Unsupported text location. Switching to TOPLEFT.");
        delete textOverlay;
        addText(text, TOPLEFT, persistent);
        return;
    }
*/    
    //addDrawItem(imageScene, textOverlay, persistent);
    addDrawItem(fixedOverlayScene, textOverlay, persistent);
    addDrawItem(fixedOverlayScene, textBackground, persistent);
}

void GstImageView::clearOverlays(bool clear_persistent_items)
{
    if(clear_persistent_items) {
        Q_FOREACH(QGraphicsItem *item, persistentDrawItems) {
            item->scene()->removeItem(item);
        }
        persistentDrawItems.clear();
    }
    
    Q_FOREACH(QGraphicsItem *item, volatileDrawItems) {
        item->scene()->removeItem(item);
    }
    volatileDrawItems.clear();
}

void GstImageView::rotate(int deg)
{
    imageView->rotate(deg);
    imageView->fitInView(imageItem, Qt::KeepAspectRatio);
    //view->fitInView(view->sceneRect(), Qt::KeepAspectRatio);
}

void GstImageView::saveImage(QString path, bool overlay)
{
    QImage saveImage(imageScene->sceneRect().size().toSize(), image.format());
    saveImage.fill(0); // Painter cannnot handle null image
    
    /* Get destination path if not submitted */
    if(path.isEmpty()) {
        path = QFileDialog::getSaveFileName(this, "Save Image",
                                path+"rock_image.png",
                                "Images (*.png)");
    }
    
    /* Save original image or also the overlay? */
    if(overlay) {
        QPainter painter(&saveImage);
        imageScene->render(&painter);
    } else {
        saveImage = image;
    }
    
    saveImage.save(path, "PNG", 80);
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
        setItemPositions();
        
        /* For some reason the first frame is not scaled to fit the full space. 
         * Calling update or even repaint did not help but a simple resize or 
         * setVisible does. On Debian, you have to call both ???
         */
        setVisible(false); setVisible(true); 
        resize(width(),height()-1); 
        resize(width(),height()+1);
    }
    
    update();
#endif
}

/* PROTECTED METHODS ---------------------------------------------------------- */

void GstImageView::resizeEvent(QResizeEvent *event)
{ 
    QWidget::resizeEvent(event);
    fixedOverlayView->resize(event->size());
    imageView->resize(event->size());

//    std::cout << "resize event.\nview.width: " << view->width() << "\n"
//              << "view.height: " << view->height() << std::endl;
    
    imageView->fitInView(imageItem, Qt::KeepAspectRatio);
    setItemPositions();
}

void GstImageView::contextMenuEvent ( QContextMenuEvent * event )
{
   contextMenu->exec(event->globalPos());
}

/* PRIVATE SLOTS ------------------------------------------------------------ */

void GstImageView::rotate_clockwise()
{
    rotate(90);
}

void GstImageView::rotate_counterclockwise()
{
    rotate(270);
}

void GstImageView::rotate_180()
{
    rotate(180);
}

void GstImageView::save_image()
{
    saveImage(QString(), false);
}

void GstImageView::save_image_overlay()
{
    saveImage(QString(), true);
}

/* PRIVATE METHODS ---------------------------------------------------------- */

void GstImageView::addDrawItem(QGraphicsScene* scene, QGraphicsItem *item, bool persistent)
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
    progress_indicator->move(imageView->width() - progress_indicator->width(), 0);
}

void GstImageView::setupContextMenu()
{
    contextMenu = new QMenu(this);
    
    rotate_image_clockwise_act = new QAction("Rotate Image by 90 deg. clockwise",this);
    connect(rotate_image_clockwise_act,SIGNAL(triggered()),this,SLOT(rotate_clockwise()));
    contextMenu->addAction(rotate_image_clockwise_act);
    
    rotate_image_counterclockwise_act = new QAction("Rotate Image by 90 deg. counter-clockwise",this);
    connect(rotate_image_counterclockwise_act,SIGNAL(triggered()),this,SLOT(rotate_counterclockwise()));
    contextMenu->addAction(rotate_image_counterclockwise_act);
    
    rotate_image_180_act = new QAction("Rotate Image by 180 deg.",this);
    connect(rotate_image_180_act,SIGNAL(triggered()),this,SLOT(rotate_180()));
    contextMenu->addAction(rotate_image_180_act);
    
    save_image_act = new QAction("Save image", this);
    connect(save_image_act,SIGNAL(triggered()),this,SLOT(save_image()));
    contextMenu->addAction(save_image_act);
    
    save_image_overlay_act = new QAction("Save image with overlay", this);
    connect(save_image_overlay_act,SIGNAL(triggered()),this,SLOT(save_image_overlay()));
    contextMenu->addAction(save_image_overlay_act);
}

