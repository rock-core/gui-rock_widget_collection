#ifndef NEWIMAGEVIEW_H
#define NEWIMAGEVIEW_H

#include <QtGui>
#include <base/samples/frame.h>
#include <frame_helper/FrameQImageConverter.h>

class NewImageView : public QWidget
{
    Q_OBJECT
public:
    NewImageView(QWidget *parent = 0);
    virtual ~NewImageView();
    
public slots:
    void setFrame(const base::samples::frame::Frame &frame);
    void update2();
    
protected:
    void resizeEvent(QResizeEvent *event);
    
private:
//    QSize fittingSize(QSize &proposed_size);
//    int heightForWidth(int width);
    
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *imageItem;
    QImage image;
            
//    QLabel *imageLabel;
//    QScrollArea *scrollArea;
    
    QSize imageSize;
    
    frame_helper::FrameQImageConverter frame_converter;
};

#endif /* NEWIMAGEVIEW_H */
