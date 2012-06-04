#include "GstImageView.h"

#include <base/logging.h>

#include <QGst/Init>
#include <QGst/Parse>
#include <QGst/Pipeline>


GstImageView::GstImageView(QWidget *parent)
    : QWidget(parent)
{
    resize(300,120);
    frame = NULL;
    
    /* Default widget property values */
    pipelineDescription = "videotestsrc ! qtglvideosink";
    
    /* GStreamer setup */
    QGst::init();
    QGst::PipelinePtr pipeline = QGst::Parse::launch(pipelineDescription).dynamicCast<QGst::Pipeline>();
    
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
    QWidget::update();
}