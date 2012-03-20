#include "NewImageView.h"

#include <base/logging.h>
#include <iostream>

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
    const char* path = ":/artificial_horizon/icon.png";
    image.load(path);
    if(image.isNull()) {
        LOG_ERROR("Could not load image from %s", path);
        exit(EXIT_FAILURE);
    }
                    
    imageItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(imageItem);
    
    /* Text Overlay */
    textOverlay = new QGraphicsSimpleTextItem("New image view widget (by Allan E. Conquest)", imageItem);
    textOverlay->show();
    textOverlay->setZValue(10);
    textOverlay->setBrush(Qt::red);
    
    scene->addItem(textOverlay);
    
    
    //view->fitInView(imageItem, Qt::KeepAspectRatio);

    resize(500,400);
    update();
}

NewImageView::~NewImageView()
{
//    delete imageLabel;
//    delete scrollArea;
}

void NewImageView::setFrame(const base::samples::frame::Frame &frame)
{
    //std::cout << "setFrame!" << std::endl;
    
    if(1 == frame_converter.copyFrameToQImageRGB888(image,frame)) {
        LOG_DEBUG("Frame size changed while converting frame to QImage (says converter)");
    }
    imageSize = image.size();
    imageItem->setPixmap(QPixmap::fromImage(image.scaled(size(), Qt::KeepAspectRatio)));
    textOverlay->setText(QString::fromStdString(frame.time.toString()));
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
//    int width = event->size().width();
//    int height = heightForWidth(width);
    
//    view->resize(width, height);
    
    view->resize(event->size());
    imageItem->setPixmap(QPixmap::fromImage(image.scaled(event->size(), Qt::KeepAspectRatio)));
   
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