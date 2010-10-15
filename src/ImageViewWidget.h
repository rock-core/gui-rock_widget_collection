/* 
 * File:   ImageViewWidget.h
 * Author: blueck
 *
 * Created on 17. Juni 2010, 14:14
 */

#include "QtGui/qwidget.h"
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QPen>
#include <QtCore/QByteArray>
#include <iostream>
#include <QtCore/QList>
#include "TextItem.h"
#include "LineItem.h"
#include "EllipseItem.h"
#include "PolygonItem.h"
#include "PolylineItem.h"
#include <QtGui/qfiledialog.h>
#include <QtGui/QMenu>
#include <QtCore/QString>

#include <QtOpenGL/QGLWidget>   //opengl support

#include <frame_helper/FrameQImageConverter.h>

#ifndef IMAGEVIEWWIDGET_H
#define	IMAGEVIEWWIDGET_H

/**
 * Widget which displays an image. The dimensions and the format needs
 * to be given and images can then be added an will be shown immidiately.
 * <h1>Imaging</h1>
 * <p>
 * If an image which is larger than the specified dimension is added, only
 * the images visible parts (within the dimension specified) will be shown.<br>
 * Alternatively one can add an image and scale it. No scaling will be
 * done if the image already has the proportions given at start. Scaling is
 * costy though. If more images of a different size shall be added the best way
 * is to change the format via changeFormat. This will change the
 * format according to the new dimensions. This should be the default behaviour
 * e.g. if a cameras resolution was changed so is the widgets. Scaling
 * should only be done if a few images are changed or if the images are all quite
 * large and should be scaled to enhance the viewing. Note though, again, that
 * scaling <b>does</b> cost performance.
 * <p>
 * <h1>Drawing shapes<h1>
 * One can draw shapes on top of the image. Currently these are:
 * <ul>
 * <li>Texts</li>
 * <li>Lines</li>
 * <li>Ellipses</li>
 * <li>Rectangles</li>
 * <li>Polylines</li>
 * <li>Polygones</li>
 * </ul>
 * One can add the shapes via addItem or a specific convenience method like
 * addText, addLine etc. Added Items will be displayed on top of the image currently displayed and all
 * successive images. RemoveItems, removes the given item. Alternatively all specific shapes 
 * can be removed or all Items.<br>
 * Items can have a group number and all items with a specific group number can be shown or hidden.
 * Note that hidden is <b>NOT</b> the same as removed. Hidden just does not draw them, removed
 * items can not be retrieved by the widget (even though they might be from outside the class).<br>
 * <b>Note:</b>
 * After adding any shape no repaint will be done. You need to either explicitly call
 * repaint on the widget or add a different image will will automatically have the shapes added
 * ontop of it. Otherwise, if adding multiple shapes, repaint would be called very often which
 * especially when working with video would be a performance waste.
 * 
 * @author Bjoern Lueck
 * @version 0.1
 */
class ImageViewWidget : public QGLWidget
{
    Q_OBJECT

public:
    /**
     * Initializing the widget with the given format and dimensions
     * For format parameters @seeQImage
     * @param width the width of images put onto the widget
     * @param height the height of images put onto the widget
     */
    ImageViewWidget(int width=64, int height=64);

    /**
     * Destructor cleaning up
     */
    virtual ~ImageViewWidget();
    void contextMenuEvent ( QContextMenuEvent * event );
    void paintGL();
    void resizeGL(int w, int h);
    int heightForWidth( int w ) {return w*image.height()/image.width(); };
    void mouseDoubleClickEvent ( QMouseEvent * event );

public slots:
    void saveImage();
    bool saveImage2(QString path);
    bool saveImage3(const QString &mode, int pixel_size,  int width,  int height,const char* pbuffer, QString path);
    
/*   QString getRubyCode()
    {
      return    "def test; "
                      " puts 'ja';"
                      "end ";
    };*/

    QWidget *newInstance()
    {
      return new ImageViewWidget();
    }


    void setFreeScale(bool free)
    {
      //    setMinimumSize(QSize(image.width()*scale_factor, image.height()*scale_factor));

    }

    void addRawImage(const QString &mode, int pixel_size, int width, int height,const char* pbuffer);

    /**
     * Adds a QImage to the widget.
     * @param image the image to be added
     */
     void addImage(const QImage &image);

    /**
     * Adds a shape to the image
     * @param drawItem teh itrem which to draw. keep the item given to remove it afterwards
     */
    QObject* addItem(QObject* object);
    
    /**
     * Adds a Text to the image and all successive images
     * @param xPos the starting x position
     * @param yPos the starting y position
     * @param groupNr the group number
     * @param text the text to be displayed
     * @return pointer to a #seeDrawItem to remove the text afterwards.
     */
    QObject* addText(int xPos, int yPos, int groupNr, const QString &text);

    /**
     * Adds a line to the images and all successive images
     * @param xPos the starting x position
     * @param yPos the starting y position
     * @param groupNr the group number
     * @param color the color of the line
     * @param endX the ending x position
     * @param endY the ending y position
     * @return pointer to a #seeDrawItem to remove the line afterwards
     */
    QObject* addLine(int xPos, int yPos, int groupNr, const QColor &color, int endX, int endY);

    /**
     * Adds an ellipse to the image and all successive images. the elipse will be drawn
     * within an outlying rectangle specified.<br>
     * The ellipse will not be filles and the border will have the minimum width.
     * @param xPos the top left x position of the rectangle
     * @param yPos the top left y position of the rectangle
     * @param groupNr teh group number
     * @param color the color of the ellipses border
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     * @return pointer to a #seeDrawItem to remove the ellipse afterwards
     */
    QObject* addEllipse(int xPos, int yPos, int groupNr, const QColor &color, int width, int height);

    /**
     * Adds a rectangle to the image and all successive images.
     * The rectangle will not be filles and the border will have the minimum width.
     * @param xPos the top left x position of the rectangle
     * @param yPos the top left y position of the rectangle
     * @param groupNr the group number
     * @param color the color of the rectangles border
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     * @return pointer to a #seeDrawItem to remove the ellipse afterwards
     */
    QObject* addRectangle(int xPos, int yPos, int groupNr, const QColor &color, int width, int height);

    /**
     * Adds multiple lines
     * @param groupNr the number of the group
     * @param color the color of the lines
     * @param points the points weher a line starts/ends
     * @param numberOfPoints the number of points in the points array
     * @return pointer to a DrawItem
     */
    QObject* addPolyline(int groupNr, const QColor &color, const QList<QPoint> &points);

    /**
     * Adds a Polygon with the given points. An additional line will be drawn between
     * the last and the first poin t given. Polygons can be filles unlike Polylines
     * @param groupNr the group number
     * @param color the color of the lines
     * @param points the points of the polygon
     * @param numberOfPoints the number of points in the points array
     * @return a DrawItem containing the Polygon
     */
    QObject* addPolygon(int groupNr, const QColor &color, const QList<QPoint> &points);

    /**
     * Remobves a shape from the image
     * @param drawItem the shape to remove
     */
    QObject* removeItem(QObject* drawItem,bool delete_object);
    
     /**
     * Removes all shapes from the iamges
     */
    void removeAllItems(bool delete_objects);

    /**
     * Sets the visibvle status of a group (true being visible)
     * @param groupNr the group for which to set the status
     * @param enable true shows all shapes, false hides them
     */
    void setGroupStatus(int groupNr, bool enable);

    /**
     * Removes all groups stati. If there are still shapes with group numbers
     * they will all be visible afterwards
     */
    void clearGroups();

    int getHeight()const {return image.height();};
    int getWidth()const {return image.width();};
    int getFormat()const {return image.format();};


    void setAspectRatio(bool value)
    {
      aspect_ratio = value;
      if(value)
      {
        QSizePolicy policy(QSizePolicy::Preferred,QSizePolicy::Fixed,QSizePolicy::Slider);
        policy.setHeightForWidth(true);
        setSizePolicy(policy);
      }
    }

    void setFixedSize(bool value)
    {
      fixed_size = value;
      if(value)
        QWidget::setFixedSize(width(),height());
    }
  
protected:
    /**
     * Adds all shapes to the image
     * @param shownImage teh iamge to add the shapes to
     */
    void addDrawItemsToWidget(QImage &shownImage);
    int setDisplaySize();

    /** The format used in the widget*/
    /** List of all Draw Items*/
    QList<DrawItem*> items;
    /** List of group numbers currently disabled*/
    QList<int> disabledGroups;

    QMenu contextMenu;
    QAction *save_image_act;
    QString save_path;
    QImage image;
    bool aspect_ratio;
    bool fixed_size;
    FrameQImageConverter frame_converter;
};

#endif	/* IMAGEVIEWWIDGET_H */

