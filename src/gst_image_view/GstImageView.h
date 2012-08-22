/**
 * \file GstImageView.h
 */

#ifndef GST_IMAGE_VIEW_H
#define GST_IMAGE_VIEW_H

#include <QtGui>
#include <QtCore>

#include <base/samples/frame.h>
#include <frame_helper/FrameQImageConverter.h>
#include "rock_widget_collection/progress_indicator/ProgressIndicator.h"

#include "GraphicsPointsItem.h"

/**
 * \brief Image viewer with optional text and geometry overlay functionality.
 * \author Allan Conquest (allan.conquest[at]dfki.de)
 */
class GstImageView : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Allan Conquest") 
    
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    
    Q_PROPERTY(bool use_progress_indicator READ useProgressIndicator WRITE setUseProgressIndicator)
    
    /** The time without frame update in ms after which the progress indicator gets started  */
    Q_PROPERTY(int progress_indicator_timeout READ getProgressIndicatorTimeout WRITE setProgressIndicatorTimeout)
    
    /** Do antialiased transformation by using costly techniques like bilinear filtering. This is produces
     *  higher CPU load but the displayed image is much smoother. This property only applies to the displayed
     *  image - not to the overlays. */
    Q_PROPERTY(bool use_smooth_transformation READ useSmoothTransformation WRITE setSmoothTransformation)
    
    /* Properties for use with GStreamer. That means in particluar that any of 
     * the following properties is unimportant if useGst is set to false. */
    
    /** Text based GStreamer pipeline description */
    Q_PROPERTY(QString pipelineDescription READ getPipelineDescription WRITE setPipelineDescription)
    
    /** Use OpenGL. Makes only sense if your graphics hardware supports OpenGL. 
     * If set to false, software rendering is used instead. */
    Q_PROPERTY(bool useGl READ getUseGl WRITE setUseGl)
    
    
public:
    GstImageView(QWidget *parent = 0);
    virtual ~GstImageView();
    
    enum TextLocation
    {
        TOPLEFT,
        TOPRIGHT,
        BOTTOMLEFT,
        BOTTOMRIGHT
    };
    Q_ENUMS(TextLocation)
    
public slots:
    const QColor& getBackgroundColor() const;
    void setBackgroundColor(const QColor & color);
    bool useProgressIndicator();
    void setUseProgressIndicator(bool use);
    bool useSmoothTransformation();
    void setSmoothTransformation(bool smooth);
    const QString getPipelineDescription() const;
    void setPipelineDescription(QString descr);
    const int getProgressIndicatorTimeout() const;
    void setProgressIndicatorTimeout(int timeout);
    bool getUseGl();
    void setUseGl(bool use_gl);
    
    /* Overlays */
    void addCircle(QPointF &center, double radius, QColor &color, int width, bool persistent = 0);
    void addLine(QLineF &line, QColor &color, int width, bool persistent = 0);
    void addPolygon(QPolygonF &polygon, QColor &color, int width, bool persistent = 0);
    void addPoints(const QList<int> points_x,QList<int> points_y, QColor &color, int width, bool persistent=0);

    /** Writes text on top of the image. The text is immune to image transformation, i.e. rotation or scaling. */
    void addText(QString text, /*TextLocation*/ int location, QColor color = QColor(Qt::black), bool persistent = 0);
    
    void clearOverlays(bool clear_persistent_items = 0);
    
    /**
     * Rotates the displayed image about deg degrees while keeping its aspect ratio.
     */
    void rotate(int deg);
    void saveImage(QString path = QString(), bool overlay = 0);
    
    /* Other slots */
    void setFrame(const base::samples::frame::Frame &frame);
    void update2();
    
signals:
    void clickedImage(const QPoint &point);

protected:
    void resizeEvent(QResizeEvent *event);
    void contextMenuEvent ( QContextMenuEvent * event );
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void displayContextMenu(QPoint screenPos);
    void rotate_clockwise();
    void save_image();
    void save_image_overlay();
    void startProgressIndicator();
    
private:
    void addDrawItem(QGraphicsScene *scene, QGraphicsItem *item, bool persistent = 0);
    void setItemPositions();
    void setupContextMenu();
    
    QColor bgColor;
    bool use_progress_indicator;
    bool use_smooth_transformation;
    int progress_indicator_timeout;
    QString pipelineDescription;
    bool use_gl;

    QGraphicsView *imageView;
    QGraphicsProxyWidget *imageViewProxy;
    QGraphicsScene *imageScene;
    QGraphicsView *fixedOverlayView;
    QGraphicsScene *fixedOverlayScene;
    QGraphicsWidget *overlayWidget;
    QGraphicsGridLayout *overlay_grid;
    QMap<int, QLabel*> overlayMap;
    QGraphicsPixmapItem *imageItem;
    QImage image;

    QList<QGraphicsItem*> persistentDrawItems;
    QList<QGraphicsItem*> volatileDrawItems;

    QSize imageSize;
    QString timestamp;

    frame_helper::FrameQImageConverter frame_converter;
    
    ProgressIndicator *progress_indicator;
    QTimer *progress_indicator_timer;
    
    /* Menus and actions */
    QMenu *contextMenu;
    QAction *rotate_image_clockwise_act;
    QAction *save_image_act;
    QAction *save_image_overlay_act;
    QAction *activate_progress_indicator_act;
    QAction *activate_smooth_transformation_act;
};

#endif /* GST_IMAGE_VIEW_H */
