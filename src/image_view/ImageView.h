/*!
 * \file ImageView.h
 */

#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <QtGui>
#include <QtCore>

#include <base/samples/Frame.hpp>
#include <base/samples/DistanceImage.hpp>
#include <frame_helper/FrameQImageConverter.h>
#include "rock_widget_collection/progress_indicator/ProgressIndicator.h"

#include "GraphicsPointsItem.h"

//TODO NOT ALL OBJECTS GET DELETED AFTER DECONSTRUCTION

/*!
 * \brief Image viewer with optional text and geometry overlay functionality.
 * 
 * ImageView is designed to display image frames, do minor transformation on
 * the image or enhance it with several possible overlays. The image is always
 * displayed completely while keeping its aspect ratio.
 * 
 * Transformation means resizing and rotation by 90 degree steps. The transformation
 * is only applied to the displayed image and is not kept in saved (exported) images.
 * 
 * There are multiple overlay possibilities to choose from like simple geometric forms
 * or text. Except for text overlays, every overlay is attatched to the image, i.e. any
 * transformation the image might receive is going to be executed on the overlay as well.
 * For example: The overlays are immune to image rotation or scaling. They simple get scaled
 * and rotated as well.
 * Text overlays, on the other hand, are considered as status information. They are attatched
 * to the widget and do not care about image transformation. Nevertheless, they react to widget
 * transformation, e.g. they keep their alignment to the assigned widget corner.
 * 
 * Overlays -- image- or widget attatched -- may be set persistent or volatile. Volatile overlays
 * are being removed with the next frame update while persistent overlays stay until you remove
 * them. Currently, you can only remove all (persistent) overlays at once.
 * 
 * It is planned to extend ImageView to work as GStreamer video sink in a future version.
 * 
 * \author Allan Conquest (allan.conquest[at]dfki.de)
 */
class ImageView : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Allan Conquest") 
    
    /*!
     * \brief The widget's background color.
     * 
     * You see the widget's background color in the areas not covered by the image. This is 
     * likely to happen because of the keeping of the images aspect ratio while fitting it
     * in the view.
     */
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    
    /*!
     * \brief Display progress indicator widget if no frames have being received for a defined time.
     * \sa ProgressIndicator
     */
    Q_PROPERTY(bool use_progress_indicator READ useProgressIndicator WRITE setUseProgressIndicator)
    
    /*! \brief The time without frame update in ms after which the progress indicator gets started. */
    Q_PROPERTY(int progress_indicator_timeout READ getProgressIndicatorTimeout WRITE setProgressIndicatorTimeout)
    
    /*! 
     *  \brief Usage of antialiasing techniques during image transformation.
     *  
     *  Do antialiased transformation by using costly techniques like bilinear filtering. That produces
     *  higher CPU load but the displayed image is much smoother. This property only applies to the displayed
     *  image - not to the overlays.
     */
    Q_PROPERTY(bool use_smooth_transformation READ useSmoothTransformation WRITE setSmoothTransformation)

    Q_PROPERTY(bool rgb_swapped READ getRgbSwapped WRITE setRgbSwapped)
    Q_PROPERTY(bool invert_color READ getInvertColor WRITE setInvertColor)
    
#ifdef USE_GST
    /* Properties for use with GStreamer. That means in particluar that any of 
     * the following properties is unimportant if useGst is set to false. */

    /*! 
     *  \brief Text based GStreamer pipeline description 
     *  \warning GStreamer is currently not supported.
     */
    Q_PROPERTY(QString pipelineDescription READ getPipelineDescription WRITE setPipelineDescription)
    
    /*! 
     *  \brief Usage of OpenGl.
     *  \warning This is currently not supported.
     *  Makes only sense if your graphics hardware supports OpenGL. 
     *  If set to false, software rendering is used instead.
     */
    Q_PROPERTY(bool useGl READ getUseGl WRITE setUseGl)
#endif
    
public:
    ImageView(QWidget *parent = 0);
    virtual ~ImageView();
    
    /*!
     * \brief Text overlay alignment specification
     * 
     * The alignment is relative to the corners of the widget (not to the image!).
     */
    enum TextLocation
    {
        TOPLEFT,
        TOPRIGHT,
        BOTTOMLEFT,
        BOTTOMRIGHT
    };
    Q_ENUMS(TextLocation)
    
    QSize sizeHint() const;
    
public slots:
    const QColor& getBackgroundColor() const;
    void setBackgroundColor(const QColor & color);
    bool useProgressIndicator();
    void setUseProgressIndicator(bool use);
    bool useSmoothTransformation();
    void setSmoothTransformation(bool smooth);
    const int getProgressIndicatorTimeout() const;
    void setProgressIndicatorTimeout(int timeout);

    void setInvertColor(bool invert);
    bool getInvertColor(){return invert_color;};
    void setRgbSwapped(bool swapped);
    bool getRgbSwapped(){return rgb_swapped;};
    
#ifdef USE_GST
    const QString getPipelineDescription() const;
    void setPipelineDescription(QString descr);
    bool getUseGl();
    void setUseGl(bool use_gl);
#endif
    
    /* Overlays */
    
    /*!
     * \brief Draws circle overlay.
     * 
     * Draws a circle overlay with center point \a center and radius \a radius.
     * The position has to be submitted in image coordinates with origin in the top left corner.
     * 
     * \param width pen width
     * \param persistent If false, the overlay gets deleted with the next frame update. Otherwise not.
     */
    void addCircle(QPointF &center, double radius, QColor &color, int width, bool persistent = 0);
    
    /*!
     * \brief Draws line overlay.
     * 
     * Draws a line overlay. The line is specified in \a line.
     * The position has to be submitted in image coordinates with origin in the top left corner.
     * 
     * \param width pen width
     * \param persistent If false, the overlay gets deleted with the next frame update. Otherwise not.
     */
    void addLine(QLineF &line, QColor &color, int width, bool persistent = 0);
    
    /*!
     * \brief Draws polygon overlay.
     * 
     * Draws a polygon overlay. The polygon is specified in \a line.
     * The position has to be submitted in image coordinates with origin in the top left corner.
     * 
     * \param width pen width
     * \param persistent If false, the overlay gets deleted with the next frame update. Otherwise not.
     */
    void addPolygon(QPolygonF &polygon, QColor &color, int width, bool persistent = 0);
    
    /*!
     * \brief Draws points overlay (2D point cloud).
     * 
     * Draws a points overlay.  This is to be understood as a 2D point cloud. 
     * The position has to be submitted in image coordinates with origin in the top left corner.
     * Since QtRuby has problems with QList<QPoint> slot signatures, this method 
     * uses two lists: one for the x-coordinates and one for the y-coordinates.
     * 
     * Example:<br>
     * You want to draw the pointcloud {(3,4),(6,8)}. Therefore you have to submit<br>
     * \a points_x : {3,6}<br>
     * \a points_y : {4,8}
     * 
     * \param width pen width
     * \param persistent If false, the overlay gets deleted with the next frame update. Otherwise not.
     */
    void addPoints(const QList<int> points_x,QList<int> points_y, QColor &color, int width, bool persistent=0);

    /*!
     * \brief Draws text overlay
     * 
     *  Draws a text overlay. The text is immune to image transformation, i.e. rotation or scaling.
     *  The text is in monospace style.
     *  You can align the text to the corners.
     * 
     * \param location Text alignment to the corners. The location value is analogous to the TextLocation enum. 
     *                 This is an intermediate solution until we find a way to use the QtRuby binding for enums.
     *                 For convenience from the ruby side, you can use a wrapper method using symbols. This method 
     *                 is defined in the Vizkit C++ Extensions in image_view_widget.rb.
     * \param persistent If false, the overlay gets deleted with the next frame update. Otherwise not.
     * 
     * \sa TextLocation
     */
    void addText(QString text, /*TextLocation*/ int location, QColor color = QColor(Qt::black), bool persistent = 0);
    
    /*!
     * \brief Removes overlays.
     * \param persistent If true, removes also persistent overlays. Otherwise not.
     */
    void clearOverlays(bool clear_persistent_items = 0);
    
    /*!
     * \brief Rotates image display.
     * 
     * Rotates the image display about \a deg degrees. The image gets fit into the widget by keeping its aspect ratio.
     */
    void rotate(int deg);
    
    /*!
     * \brief Saves current image to file.
     * 
     * The saved image is either the plain original image or said image with overlays (depending on \a overlay). 
     * The saved image size is always the size of the original image.
     * 
     * \param path Save destination. If not submitted, a file chooser dialog is presented.
     * \param overlay Saves overlays, too if set to true. Otherwise only the image is being saved.
     */
    void saveImage(QString path = QString(), bool overlay = 0);
    
    /* Other slots */
    
    /*!
     * \brief Frame input.
     * 
     * \param frame The image to be displayed next. The display gets updated immediately.
     */
    void setFrame(const base::samples::DistanceImage &frame);
    void setFrame(const base::samples::frame::Frame &frame);
    void setImage(const QImage &image);
    void setRawImage(const QString &mode, int pixel_size, int width, int height,const char* pbuffer, const int size);
    void refresh();
    void processImage();
    
    /*!
     * \brief Wrapper for QWidget::update(). This is due to some QtRuby limitations.
     */
    void update2();

    /*!
     * \brief returns the real height of the underlaying widget
     */
    int getHeight() const;

    /*!
     * \brief returns the real width of the underlaying widget
     */
    int getWidth() const;


    
signals:
    
    /*!
     * \brief Mouse click coordinates in image.
     * 
     * Gets emitted if the image receives a mouse click event. The position is 
     * in image coordinates where the origin is in the top-left corner.
     */
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
    bool rgb_swapped;
    bool invert_color;
    QString last_path;
#ifdef USE_GST
    QString pipelineDescription;
    bool use_gl;
#endif
    
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

    frame_helper::FrameQImageConverter frame_converter;
    
    ProgressIndicator progress_indicator;
    QTimer progress_indicator_timer;
    
    /* Menus and actions */
    QMenu context_menu;
    QAction rotate_image_clockwise_act;
    QAction save_image_act;
    QAction save_image_overlay_act;
    QAction activate_progress_indicator_act;
    QAction rgb_swapped_act;
    QAction invert_color_act;
};

#endif /* IMAGE_VIEW_H */
