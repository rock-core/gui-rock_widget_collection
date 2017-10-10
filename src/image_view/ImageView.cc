/*!
 * \file ImageView.cc
 */

#include "ImageView.h"
#include "ContextMenuGraphicsWidget.h"

#include <base-logging/Logging.hpp>
#include <iostream>

#include <QtOpenGL/QGLWidget>
#include "GraphicsPointsItem.h"

#ifdef USE_GST
    #include <QGst/Init>
    #include <QGst/Parse>
    #include <QGst/Pipeline>
    #include <QGst/ElementFactory>
    #include <QGst/Ui/VideoWidget>
    #include <QGst/Ui/GraphicsVideoSurface>
    #include <QGst/Ui/GraphicsVideoWidget>
#endif

ImageView::ImageView(QWidget *parent)
      : QWidget(parent),
        bgColor(QColor(Qt::black)),
        use_progress_indicator(true),
        use_smooth_transformation(true),
        progress_indicator_timeout(2500),
        rgb_swapped(false),
        invert_color(false),
        progress_indicator_timer(this),
        context_menu(this),
        rotate_image_clockwise_act(this),
        save_image_act(this),
        save_image_overlay_act(this),
        activate_progress_indicator_act(this),
        rgb_swapped_act(this),
        invert_color_act(this)
#ifdef USE_GST
        ,
        pipelineDescription("videotestsrc ! ximagesink"), //qtglvideosink
        use_gl(false)
#endif
{
    setWindowTitle("ImageView");
    setMinimumSize(220,170);
    setContentsMargins(0,0,0,0);
    
    imageItem = NULL;
    
    /* Configure progress indicator */
    progress_indicator.setAnimationDelay(40);
    progress_indicator.setDisplayedWhenStopped(false);
    progress_indicator.resize(30,30);
    progress_indicator.hide();
    
    /* Set up timer for progress indicator */
    progress_indicator_timer.setInterval(getProgressIndicatorTimeout());
    connect(&progress_indicator_timer, SIGNAL(timeout()), this, SLOT(startProgressIndicator()));
    
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
    imageScene = new QGraphicsScene(this);
    imageScene->setBackgroundBrush(getBackgroundColor());
    imageScene->installEventFilter(this); // to get mouse press events to emit clicked image coordinate
    
    imageView = new QGraphicsView(imageScene); 
    imageView->setAlignment(Qt::AlignCenter);
    imageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    imageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    imageView->setFocusPolicy(Qt::NoFocus);
    imageView->setFrameStyle(QFrame::NoFrame);
    imageView->setContentsMargins(0,0,0,0);

#ifdef USE_GST
    if(use_gl) {
        // Indicator to use qtglvideosink (hardware rendering!) instead of qtvideosink
        imageView->setViewport(new QGLWidget); 
    }
    
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
    
    setSmoothTransformation(use_smooth_transformation);
    
    imageScene->addItem(imageItem);
#endif
    
    /* Setup outer graphicsscene and view. This scene contains fixed overlays like the 
     * image timestamp which are immune to rotation etc. i.e. they stay at the same place
     * nevertheless the inner scene gets rotated or scaled.
     */
    fixedOverlayScene = new QGraphicsScene(this);
    fixedOverlayScene->setBackgroundBrush(Qt::transparent);
    
    fixedOverlayView = new QGraphicsView(fixedOverlayScene, this);
    fixedOverlayView->setAlignment(Qt::AlignCenter);
    fixedOverlayView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fixedOverlayView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fixedOverlayView->setFocusPolicy(Qt::NoFocus);
    fixedOverlayView->setFrameStyle(QFrame::NoFrame);
    fixedOverlayView->setContentsMargins(0,0,0,0);
    
    /* Layout for outer scene to help positioning text overlay */
    overlay_grid = new QGraphicsGridLayout;
    
    /* XXX Transparent middle item with variable size (default). This snaps the border items to the border.
     * TODO That seems to eliminate the possibility of a middle widget with fixed size. 
     */
    QGraphicsWidget *label_mi = fixedOverlayScene->addWidget(new QLabel);
    QPalette label_mi_pal;
    label_mi_pal.setColor(QPalette::Window, QColor(Qt::transparent));
    //label_mi_pal.setColor(QPalette::Window, QColor(Qt::white));
    label_mi->setPalette(label_mi_pal);
    label_mi->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    overlay_grid->addItem(label_mi, 1, 1, Qt::AlignVCenter|Qt::AlignCenter);
    
    overlayWidget = new ContextMenuGraphicsWidget;//new QGraphicsWidget;
    connect(overlayWidget, SIGNAL(contextMenuRequest(QPoint)), this, SLOT(displayContextMenu(QPoint)));
    
    overlayWidget->setLayout(overlay_grid);
    overlayWidget->setZValue(12);
    fixedOverlayScene->addItem(overlayWidget);

    // Nest inner view in outer scene
    imageViewProxy = fixedOverlayScene->addWidget(imageView);
    
    progress_indicator.setParent(fixedOverlayView);
     
    setItemPositions();
    update();
}

ImageView::~ImageView()
{
}

QSize ImageView::sizeHint() const
{
    return QSize(500,500);
}

const QColor& ImageView::getBackgroundColor() const
{
    return bgColor;
}

void ImageView::setBackgroundColor(const QColor & color)
{
    bgColor = color;
    imageScene->setBackgroundBrush(bgColor);
}

bool ImageView::useProgressIndicator()
{
    return use_progress_indicator;
}
  

void ImageView::setUseProgressIndicator(bool use)
{
    use_progress_indicator = use;
    progress_indicator.hide();
    
    if(use) {
        progress_indicator_timer.start();
        // TODO Maybe unwanted behavior since the timer starts even if there is not a single frame yet.
        //      But it is nice if you trigger the use of the progress indicator while the replay is stopped.
    }
    else {
        // Disable possibly already running widget
        progress_indicator_timer.stop();
        progress_indicator.stopAnimation();
    }
}

bool ImageView::useSmoothTransformation()
{
    return use_smooth_transformation;
}

void ImageView::setSmoothTransformation(bool smooth)
{
    use_smooth_transformation = smooth;
    if(smooth)
        imageItem->setTransformationMode(Qt::SmoothTransformation);
    else
        imageItem->setTransformationMode(Qt::FastTransformation);
}

const int ImageView::getProgressIndicatorTimeout() const
{
    return progress_indicator_timeout;
}

void ImageView::setProgressIndicatorTimeout(int timeout)
{
    progress_indicator_timeout = timeout;
    progress_indicator_timer.setInterval(timeout);
}

#ifdef USE_GST
    const QString ImageView::getPipelineDescription() const
    {
        return pipelineDescription;
    }

    void ImageView::setPipelineDescription(QString descr)
    {
        this->pipelineDescription = descr;
    }

    bool ImageView::getUseGl()
    {
        return this->use_gl;
    }

    void ImageView::setUseGl(bool use_gl)
    {
        this->use_gl = use_gl;
    }
#endif /* USE_GST */

void ImageView::addCircle(QPointF &center, double radius, QColor &color, int width, bool persistent)
{
    QGraphicsEllipseItem *circlePtr = 
            new QGraphicsEllipseItem(QRectF(center.x() - radius/2.0, center.y() - radius/2.0, radius, radius));
    QPen pen;
    pen.setColor(color);
    pen.setWidth(width);
    circlePtr->setPen(pen);
    
    addDrawItem(imageScene, circlePtr, persistent);
}

void ImageView::addLine(QLineF &line, QColor &color, int width, bool persistent)
{                     
    QGraphicsLineItem *linePtr = new QGraphicsLineItem(line);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(width);
    linePtr->setPen(pen);

    addDrawItem(imageScene, linePtr, persistent);
}

void ImageView::addPoints(const QList<int> points_x,QList<int> points_y, QColor &color, int width, bool persistent)
{   
    QList<QPoint> points;
    QList<int>::const_iterator iter1 = points_x.begin();
    QList<int>::const_iterator iter2 = points_y.begin();
    for(;iter1 != points_x.end() && iter2 != points_y.end();++iter1,++iter2)
    {
        points.push_back(QPoint(*iter1,*iter2));
    }

    GraphicsPointsItem *pointsPtr = new GraphicsPointsItem(points);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(width);
    pointsPtr->setPen(pen);

    addDrawItem(imageScene, pointsPtr, persistent);
}

void ImageView::addPolygon(QPolygonF &polygon, QColor &color, int width, bool persistent)
{              
    LOG_DEBUG_S << "Received polygon. Polygon closed: " << polygon.isClosed();
    LOG_DEBUG_S << "The polygon has a size of " << polygon.size() << ".";
    QGraphicsPolygonItem *polygonPtr = new QGraphicsPolygonItem(polygon);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(width);
    polygonPtr->setPen(pen);

    addDrawItem(imageScene, polygonPtr, persistent);
}

void ImageView::addText(QString text, /*TextLocation*/ int location, QColor color, bool persistent)
{   
    /* Positioning */
    int row = 0;
    int col = 0;
    Qt::Alignment alignment = Qt::AlignLeft;
    
    switch(location) {
    case TOPLEFT :
        row = 0;
        col = 0;
        alignment = Qt::AlignTop|Qt::AlignLeft;
        break;
    case TOPRIGHT :
        row = 0;
        col = 2;
        alignment = Qt::AlignTop|Qt::AlignRight;
        break;
    case BOTTOMLEFT :
        row = 2;
        col = 0;
        alignment = Qt::AlignBottom|Qt::AlignLeft;
        break;
    case BOTTOMRIGHT :
        row = 2;
        col = 2;
        alignment = Qt::AlignBottom|Qt::AlignRight;
        break;
    default:
        LOG_WARN("Unsupported text location. Switching to TOPLEFT.");
        addText(text, TOPLEFT, persistent);
        return;
    }
    
    QLabel *label = NULL;
    QGraphicsWidget *textLabel = NULL;
    int margin = 1;
    
    // Avoid displaying empty labels, i.e. labels with margin only and no text
    if(text.isEmpty()) {
        margin = 0;
    }
    
    // Label for text location already exists?
    if(overlayMap.contains(location)) {
        // Alter existing label
        label = overlayMap[location];
        label->setText(text);
        label->adjustSize();
    } else {
        // Add new label to map
        label = new QLabel(text);
        label->setVisible(false);
        label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        
        overlayMap[location] = label;
        
        textLabel = fixedOverlayScene->addWidget(label);
        overlay_grid->addItem(textLabel, row, col, alignment);
        textLabel->setVisible(true); // TODO use label instead?
    }
    
    QPalette palette;    
    QColor labelBgColor(Qt::white);
    labelBgColor.setAlphaF(0.7);
    palette.setColor(QPalette::Window, labelBgColor);
    palette.setColor(QPalette::WindowText, color);
    palette.setColor(QPalette::Text, color); // <-- this one seems to be working but WindowText should be correct according to docu
        
    label->setPalette(palette);
    label->setMargin(margin);
       
    QFont font("Monospace");
    font.setPointSize(12);
    font.setStyleHint(QFont::TypeWriter);
    label->setFont(font);
    
    label->setMaximumSize(label->sizeHint());
    
//     std::cout << "label size hint: " << label->sizeHint().width() << "," << label->sizeHint().height() << " ('" << text.toStdString().data() << "')" << std::endl;
//     std::cout << "label size: " << label->width() << "," << label->height() << " ('" << text.toStdString().data() << "')" << std::endl;
//     std::cout << "label maximum size: " << label->maximumSize().width() << "," << label->maximumSize().height() << " ('" << text.toStdString().data() << "')" << std::endl;
//     std::cout << "---" << std::endl;
    
    overlay_grid->invalidate();
    //addDrawItem(NULL, textLabel, persistent); // TODO check correctness of deletion etc.
    update();
}

void ImageView::clearOverlays(bool clear_persistent_items)
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

void ImageView::rotate(int deg)
{
    imageView->rotate(deg);
    imageView->fitInView(imageItem, Qt::KeepAspectRatio);
    //view->fitInView(view->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::saveImage(QString path, bool overlay)
{
    QImage saveImage(imageScene->sceneRect().size().toSize(), image.format());
    saveImage.fill(0); // Painter cannnot handle null image
    
    /* Get destination path if not submitted */
    if(path.isEmpty()) {
        path = QFileDialog::getSaveFileName(this, "Save Image",
                                last_path,
                                "Images (*.png)");
        last_path = path;
    }
    
    /* Save original image or also the overlay? */
    if(overlay) {
        QPainter painter(&saveImage);
        
        // Render image and image overlays first
        imageScene->render(&painter);
        
        QSize oldSize = size();
        QSize newSize = imageScene->sceneRect().size().toSize();
        
        // Hide image view from outer scene. It is already painted.
        imageViewProxy->hide();
        
        fixedOverlayScene->setSceneRect(imageScene->sceneRect());
        overlayWidget->resize(newSize);
        
        // Render outer scene (text overlays) on top
        fixedOverlayScene->render(&painter);
        
        imageViewProxy->show();
        
        // restore old sizes
        resize(oldSize.width(),oldSize.height()+1);
        resize(oldSize.width(),oldSize.height()-1);
    } else {
        saveImage = image;
    }
    
    saveImage.save(path, "PNG", 80);
}

void ImageView::setFrame(const base::samples::DepthMap &frame)
{
    // This just copies/converts remission values from the DepthMap to a Frame
    // TODO For better visualization, the vertical and horizontal resolution of the DepthMap should be considered

    base::samples::frame::Frame bframe(frame.horizontal_size, frame.vertical_size, 8);
    uint8_t* ptr = bframe.getImagePtr();

    for(std::vector<float>::const_iterator r=frame.remissions.begin(); r!=frame.remissions.end(); ++r)
    {
        *ptr++ = *r * 255;
    }
    if(1 == frame_converter.copyFrameToQImageRGB888(image,bframe)) {
        LOG_WARN("Frame size changed while converting frame to QImage (says converter)");
    }
    processImage();
    refresh();
}

void ImageView::setFrame(const base::samples::DistanceImage &frame)
{  

    base::samples::frame::Frame bframe(frame.width,frame.height,16); 
    std::vector<uint16_t> v(frame.data.size());
    for(unsigned int i=0;i<v.size();i++){
        v[i] = frame.data[i];
    }

    bframe.setImage((uint8_t*)&v[0],frame.data.size()*2);
    if(1 == frame_converter.copyFrameToQImageRGB888(image,bframe)) {
        LOG_WARN("Frame size changed while converting frame to QImage (says converter)");
    }
    processImage();
    refresh();
}

void ImageView::setFrame(const base::samples::frame::Frame &frame)
{   
    if(1 == frame_converter.copyFrameToQImageRGB888(image,frame)) {
        LOG_WARN("Frame size changed while converting frame to QImage (says converter)");
    }
    processImage();
    refresh();
}

void ImageView::setRawImage(const QString &mode, int pixel_size, int width, int height,const char* pbuffer, const int size)
{
    frame_converter.copyFrameToQImageRGB888(image,mode, pixel_size, width, height,pbuffer,size);
    processImage();
    refresh();
}

void ImageView::setImage(const QImage &image)
{   
    this->image = image.copy();
    processImage();
    refresh();
}

void ImageView::processImage()
{
    if(getInvertColor())
        image.invertPixels();
    if(getRgbSwapped())
        image = image.rgbSwapped();
}

void ImageView::refresh()
{
    if(use_progress_indicator) {
        progress_indicator.hide();
        progress_indicator_timer.start();
        progress_indicator.stopAnimation();
    }
    
#ifdef USE_GST
    // TODO
#else
    clearOverlays();

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
    
    /* Resize and repositioning if frame size changes (and on start) */
    if(imageSize != oldSize) {
        LOG_INFO("image size changed. resize.");
        
        // Avoid dynamic scene growth
        imageScene->setSceneRect(0, 0, imageSize.width(), imageSize.height());
        
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

void ImageView::resizeEvent(QResizeEvent *event)
{ 
    QWidget::resizeEvent(event);
    
    //fixedOverlayScene->setSceneRect(fixedOverlayScene->itemsBoundingRect());
    QSize eventSize = event->size();
    fixedOverlayScene->setSceneRect(0, 0, eventSize.width(), eventSize.height());
    imageView->resize(eventSize);
    

//    std::cout << "resize event.\nview.width: " << view->width() << "\n"
//              << "view.height: " << view->height() << std::endl;
    
    imageView->fitInView(imageItem, Qt::KeepAspectRatio);
    
    
    fixedOverlayView->resize(eventSize);
    overlayWidget->resize(eventSize);
    setItemPositions();
    LOG_DEBUG_S << "scene dimensions:\n"
            << "fixedOverlayScene (w/h): (" << fixedOverlayScene->width() << "," << fixedOverlayScene->height() << ")\n"
            << "imageScene (w/h): (" << imageScene->width() << "," << imageScene->height() << ")\n";
}

void ImageView::contextMenuEvent ( QContextMenuEvent * event )
{
   context_menu.exec(event->globalPos());
}

bool ImageView::eventFilter(QObject* obj, QEvent* event)
{
    //std::cout << "Got an event: " << event->type() << std::endl;
    
    if (event->type() == QEvent::GraphicsSceneMousePress) {
         QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
         
         QPoint scenePos = mouseEvent->scenePos().toPoint();
         
         // Clicked on image scene?
         if(imageScene->sceneRect().contains(scenePos)) {
             LOG_INFO_S << "Clicked at image coordinate (w,h) (" << scenePos.x() << "," << scenePos.y() << ")";
             emit clickedImage(scenePos);
         } 
         
         return true;
     } else {
         // standard event processing
         return QObject::eventFilter(obj, event);
     }
}


/* PRIVATE SLOTS ------------------------------------------------------------ */

void ImageView::displayContextMenu(QPoint screenPos)
{
    LOG_DEBUG("Got context menu request signal");
    context_menu.exec(screenPos);
}

void ImageView::rotate_clockwise()
{
    rotate(90);
}

void ImageView::save_image()
{
    saveImage(QString(), false);
}

void ImageView::save_image_overlay()
{
    saveImage(QString(), true);
}

void ImageView::startProgressIndicator()
{
    progress_indicator.show();
    progress_indicator.startAnimation();
}


/* PRIVATE METHODS ---------------------------------------------------------- */

void ImageView::addDrawItem(QGraphicsScene* scene, QGraphicsItem *item, bool persistent)
{   
    //std::cout << "addDrawItem: adding item: " << item << std::endl;
    
    if(persistent) {
        persistentDrawItems.push_back(item);
    } else {
        volatileDrawItems.push_back(item);
    }
    if(scene) {// Text overlays are already in the scene and have therefore scene==null
        LOG_DEBUG("Adding item to scene");
        scene->addItem(item);
    }
    update();
}

void ImageView::update2()
{
    QWidget::update();
}

void ImageView::setItemPositions()
{
    // Align to center of view (scene gets always fit in view)   
    progress_indicator.move(imageView->width()/2 - progress_indicator.width()/2,
                             imageView->height()/2 - progress_indicator.height()/2);
}

int ImageView::getHeight() const
{
    return imageSize.height();
}

int ImageView::getWidth() const
{
    return imageSize.width();
}

void ImageView::setInvertColor(bool invert)
{
    if(invert_color == invert)
        return;
    invert_color=invert;
    image.invertPixels();
    refresh();
}
void ImageView::setRgbSwapped(bool swapped)
{
    if(rgb_swapped == swapped)
        return;
    rgb_swapped=swapped;
    image = image.rgbSwapped();
    refresh();
}

void ImageView::setupContextMenu()
{
    rgb_swapped_act.setText("RGB Swapped");
    rgb_swapped_act.setCheckable(true);
    rgb_swapped_act.setChecked(getRgbSwapped());
    connect(&rgb_swapped_act,SIGNAL(triggered(bool)),this,SLOT(setRgbSwapped(bool)));
    context_menu.addAction(&rgb_swapped_act);

    invert_color_act.setText("Invert Color");
    invert_color_act.setCheckable(true);
    invert_color_act.setChecked(getInvertColor());
    connect(&invert_color_act,SIGNAL(triggered(bool)),this,SLOT(setInvertColor(bool)));
    context_menu.addAction(&invert_color_act);

    rotate_image_clockwise_act.setText("Rotate 90 deg");
    connect(&rotate_image_clockwise_act,SIGNAL(triggered()),this,SLOT(rotate_clockwise()));
    context_menu.addAction(&rotate_image_clockwise_act);
    
    save_image_act.setText("Save");
    connect(&save_image_act,SIGNAL(triggered()),this,SLOT(save_image()));
    context_menu.addAction(&save_image_act);
    
    save_image_overlay_act.setText("Save with Overlay");
    connect(&save_image_overlay_act,SIGNAL(triggered()),this,SLOT(save_image_overlay()));
    context_menu.addAction(&save_image_overlay_act);
    
    activate_progress_indicator_act.setText("Show Progress-Indicator");
    activate_progress_indicator_act.setCheckable(true);
    activate_progress_indicator_act.setChecked(use_progress_indicator);
    connect(&activate_progress_indicator_act,SIGNAL(triggered(bool)),this,SLOT(setUseProgressIndicator(bool)));
    context_menu.addAction(&activate_progress_indicator_act);
}

