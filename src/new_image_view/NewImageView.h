#ifndef NEWIMAGEVIEW_H
#define NEWIMAGEVIEW_H

#include <QtGui>
#include <base/samples/frame.h>
#include <frame_helper/FrameQImageConverter.h>

enum TextLocation
{
    TOPLEFT = 0,
    TOPRIGHT,
    BOTTOMLEFT,
    BOTTOMRIGHT
};

class NewImageView : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Allan Conquest")
public:
    NewImageView(QWidget *parent = 0);
    virtual ~NewImageView();
    
public slots:
    
    void addCircle(QPointF center, double radius, bool persistent = 0);
    void addLine(QLineF &line, bool persistent = 0);
    void addText(QString text, TextLocation location, bool persistent = 0);
    
    void clearOverlays(bool clear_persistent_items = 0);
    
    /**
     * Rotates the displayed image about deg degrees. Only steps of 90 degrees
     * are allowed: 0, 90, 180, 270, 360
     */
    void rotate(int deg);
    void setFrame(const base::samples::frame::Frame &frame);
    void update2();
    
protected:
//    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    
private:
//    QSize fittingSize(QSize &proposed_size);
//    int heightForWidth(int width);
    void addDrawItem(QGraphicsItem *item, bool persistent = 0); 
    
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *imageItem;
    QImage image;
    
    QList<QGraphicsItem*> persistentDrawItems;
    QList<QGraphicsItem*> volatileDrawItems;
            
//    QLabel *imageLabel;
//    QScrollArea *scrollArea;
    
    QSize imageSize;
    
    frame_helper::FrameQImageConverter frame_converter;
};

#endif /* NEWIMAGEVIEW_H */
