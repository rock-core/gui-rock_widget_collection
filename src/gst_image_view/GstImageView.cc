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
    frame = NULL;
    
    /* Default widget property values */
    pipelineDescription = "videotestsrc ! ximagesink";//qtglvideosink";
    
    /* GStreamer setup */
    QGst::init();
    
    //QGst::PipelinePtr pipeline = QGst::Parse::launch(pipelineDescription).dynamicCast<QGst::Pipeline>();
    QGst::PipelinePtr pipeline = QGst::Pipeline::create("pipeline"); // Create hard coded pipeline for debugging
    QGst::ElementPtr videoSrc = QGst::ElementFactory::make("videotestsrc");
    
    
    /* Setup video sink */
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsView *view = new QGraphicsView(scene, this);
//     view->setViewport(new QGLWidget); // indicator to use qtglvideosink (hardware rendering!)
    
    QGst::Ui::GraphicsVideoSurface *surface = new QGst::Ui::GraphicsVideoSurface(view); 
    QGst::Ui::GraphicsVideoWidget *widget = new QGst::Ui::GraphicsVideoWidget;
    widget->setSurface(surface);
    
    scene->addItem(widget);
//     view->show();
    view->resize(400,400);
    widget->resize(300,300);
    
    pipeline->add(videoSrc, surface->videoSink());
    videoSrc->link(surface->videoSink());
    
    std::cout << "Video source name: " << surface->videoSink()->property("name").toString().toStdString() << std::endl;
    
    /* Try to start playing */
    if(!pipeline->setState(QGst::StatePlaying)) {
        // TODO proper fallback handling
        /* There seems to be an opengl problem. Fall back to software rendering using qtvideosink. */
        std::cout << "Could not play pipeline with opengl video sink. Trying to fall back to qtvideosink." << std::endl;
    }

}

GstImageView::~GstImageView()
{
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

void GstImageView::setFrame(const base::samples::frame::Frame &frame)
{   
    std::cout << "GstImageView: Got frame!" << std::endl;
    
    // TODO store frame in a pointer?
    // this->frame = frame;
    
    // ------
    
//     clearOverlays();
   
//     if(1 == frame_converter.copyFrameToQImageRGB888(image,frame)) {
//         LOG_DEBUG("Frame size changed while converting frame to QImage (says converter)");
//     }

//     imageSize = image.size();
//     //std::cout << "Image size: x=" << image.width() <<", y=" << image.height() << std::endl;
//     QPixmap pixmap = QPixmap::fromImage(image);
//     pixmap.scaled(size(), Qt::KeepAspectRatio);
//     
//     //std::cout << "Pixmap size: x=" << pixmap.width() <<", y=" << pixmap.height() << std::endl;
//     imageItem->setPixmap(pixmap);
//     //std::cout << "ImageItem BoundingRect: x=" << imageItem->boundingRect().width() <<", y=" << imageItem->boundingRect().height() << std::endl;
//     addText(QString::fromStdString(frame.time.toString()), TOPRIGHT, 0);
//     update();
}

void GstImageView::update2()
{
    std::cout << "GstImageView: update2()" << std::endl;
    QWidget::update();
}