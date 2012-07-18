#ifndef GST_IMAGE_VIEW_H
#define GST_IMAGE_VIEW_H

#include <QtGui>
#include <QtCore>

#include <base/samples/frame.h>
#include <frame_helper/FrameQImageConverter.h>

enum TextLocation
{
    TOPLEFT = 0,
    TOPRIGHT,
    BOTTOMLEFT,
    BOTTOMRIGHT
};

class GstImageView : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Allan Conquest")
    Q_PROPERTY(QString pipelineDescription READ getPipelineDescription WRITE setPipelineDescription)
    Q_PROPERTY(bool useGl READ getUseGl WRITE setUseGl)
    Q_PROPERTY(bool useGst READ getUseGst WRITE setUseGst)
    
    
public:
    GstImageView(QWidget *parent = 0);
    virtual ~GstImageView();
    
public slots:
    QString getPipelineDescription();
    void setPipelineDescription(QString descr);
    bool getUseGl();
    void setUseGl(bool use_gl);
    bool getUseGst();
    void setUseGst(bool use_gst);
    
    /* Overlays */
    
    void addCircle(QPointF center, double radius, bool persistent = 0);
    void addLine(QLineF &line, bool persistent = 0);
    void addText(QString text, TextLocation location, bool persistent = 0);
    
    void clearOverlays(bool clear_persistent_items = 0);
    
    /**
     * Rotates the displayed image about deg degrees while keeping its aspect ratio.
     */
    void rotate(int deg);
    
    /* Other slots */
    
    void setFrame(const base::samples::frame::Frame &frame);
    void update2();

protected:
    void resizeEvent(QResizeEvent *event);
    
private:
    void addDrawItem(QGraphicsItem *item, bool persistent = 0); 
    
    QString pipelineDescription;
    bool use_gst;
    bool use_gl;

    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *imageItem;
    QImage image;

    QList<QGraphicsItem*> persistentDrawItems;
    QList<QGraphicsItem*> volatileDrawItems;

    QSize imageSize;

    frame_helper::FrameQImageConverter frame_converter;
};

#endif /* GST_IMAGE_VIEW_H */
