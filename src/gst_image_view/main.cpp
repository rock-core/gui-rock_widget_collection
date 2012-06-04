// Experimenting with GST rendering on a QWidget

#include <iostream>
#include <stdlib.h>

#include <QtCore>
#include <QtGui>
#include <QtOpenGL/QGLWidget>

#include <QGst/Init>
#include <QGst/Parse>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Ui/VideoWidget>
#include <QGst/Ui/GraphicsVideoSurface>
#include <QGst/Ui/GraphicsVideoWidget>

#include "GstImageView.h"

int main(int argc, char *argv[])
{
    QApplication qapp(argc, argv);
//     QGst::init(&argc, &argv);
    
#if 0

    QGst::ElementPtr videoSrc = QGst::ElementFactory::make("videotestsrc");
    QGst::ElementPtr fallbackVideoSink = QGst::ElementFactory::make("qtvideosink");
    QGst::PipelinePtr pipeline = QGst::Pipeline::create("mypipeline");
    QGst::PipelinePtr fallbackPipeline = QGst::Pipeline::create("fallbackPipeline");
      
    QGraphicsScene *scene = new QGraphicsScene;
    QGraphicsView *view = new QGraphicsView(scene);
//     view->setViewport(new QGLWidget); // indicator to use qtglvideosink (hardware rendering!)
    
    QGst::Ui::GraphicsVideoSurface *surface = new QGst::Ui::GraphicsVideoSurface(view); 
    QGst::Ui::GraphicsVideoWidget *widget = new QGst::Ui::GraphicsVideoWidget;
    widget->setSurface(surface);
    
    scene->addItem(widget);
    view->show();
    view->resize(400,400);
    widget->resize(300,300);
    
    pipeline->add(videoSrc, surface->videoSink());
    videoSrc->link(surface->videoSink());
    
    std::cout << "Video source name: " << surface->videoSink()->property("name").toString().toStdString() << std::endl;
    
    /* start playing */
    if(!pipeline->setState(QGst::StatePlaying)) {
        /* There seems to be an opengl problem. Fall back to software rendering using qtvideosink. */
        std::cout << "Could not play pipeline with opengl video sink. Trying to fall back to qtvideosink." << std::endl;
//         pipeline->remove(surface->videoSink());
//         delete pipeline; pipeline = 0;
//         //videoSrc->unparent();
//         //fallbackPipeline->add(videoSrc, fallbackVideoSink);
//         //videoSrc->link(fallbackVideoSink);
//         delete surface; surface = 0;
//         delete widget; widget = 0;
//         
//         view = new QGraphicsView(scene);
//         
//         surface = new QGst::Ui::GraphicsVideoSurface(view); 
//         widget = new QGst::Ui::GraphicsVideoWidget;
//         widget->setSurface(surface);
//         std::cout << "Video source name: " << surface->videoSink()->property("name").toString().toStdString() << std::endl;
//         
//         view->show();
//         view->resize(400,400);
//         widget->resize(300,300);
//         
//         fallbackPipeline->add(surface->videoSink());
//         videoSrc->link(surface->videoSink());
//         
//         std::cout << "before second play call" << std::endl;
//  
//         if(!fallbackPipeline->setState(QGst::StatePlaying)) {
//             /* Still unable to play pipeline */
//             std::cout << "Falling back to software rendering also did not help. Exit." << std::endl;
//             exit(EXIT_FAILURE);
//         }
//         std::cout << "Everything worked. Playing the pipeline." << std::endl;
    }

#endif
    
#if 0

    QGst::ElementPtr videoSrc = QGst::ElementFactory::make("videotestsrc");
    QGst::ElementPtr videoSink = QGst::ElementFactory::make("qtglvideosink");
    
    QGLWidget *glwidget = new QGLWidget;
    glwidget->makeCurrent();
    videoSink->setProperty("glcontext", (void*) QGLContext::currentContext());
    glwidget->doneCurrent();

    if (videoSink->setState(QGst::StateReady) != QGst::StateChangeSuccess) {
        // fall back to qtvideosink
        videoSink = QGst::ElementFactory::make("qtvideosink");
    }
    
    QGst::PipelinePtr pipeline = QGst::Pipeline::create("mypipeline");
    pipeline->add(videoSrc, videoSink);
    videoSrc->link(videoSink);
    
    
#endif

    GstImageView gstImageView;
    gstImageView.show();
    
    qapp.exec();
}