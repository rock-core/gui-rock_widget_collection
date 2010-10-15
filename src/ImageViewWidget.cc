/* 
 * File:   ImageViewWidget.cc
 * Author: blueck
 * 
 * Created on 17. Juni 2010, 14:14
 */

#include "ImageViewWidget.h"
#include <stdexcept>
#include <QtCore/QtPlugin>

using namespace base::samples::frame;
Q_EXPORT_PLUGIN2(ImageViewWidget, ImageViewWidget)

ImageViewWidget::ImageViewWidget(int width, int height):
image(width, height, QImage::Format_RGB888),contextMenu(this),
aspect_ratio(true),fixed_size(false)
{
 //   setMinimumSize(QSize(width, height));
    //create actions
    save_image_act = new QAction(tr("&Save Image"),this);
    save_image_act->setStatusTip(tr("Save the image to disk"));
    connect(save_image_act,SIGNAL(triggered()),this,SLOT(saveImage()));
    contextMenu.addAction(save_image_act);
}


ImageViewWidget::~ImageViewWidget()
{
    disconnect(save_image_act, 0, 0, 0);
    delete save_image_act;
//   items.clear();
}

void ImageViewWidget::contextMenuEvent ( QContextMenuEvent * event )
{
   contextMenu.exec(event->globalPos());
}

void ImageViewWidget::saveImage()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"),
                            save_path,
                            tr("Images (*.png)"));
    if(path.length() > 0)	
    {
      save_path = path;
      image.save(save_path,"PNG",80);
    }
}
bool ImageViewWidget::saveImage2(QString path)
{
    image.save(path,"PNG",80);
}

bool ImageViewWidget::saveImage3(const QString &mode, int pixel_size,  int width,  int height,const char* pbuffer, QString path)
{
    QImage image;
    frame_converter.copyFrameToQImageRGB888(image,mode, pixel_size, width, height,pbuffer);
    image.save(path,"PNG",80);
}

QObject* ImageViewWidget::addText(int xPos, int yPos, int groupNr, const QString &text)
{
    TextItem* textItem = new TextItem(xPos, yPos, groupNr, text);
    items.push_back(textItem);
    return textItem;
}

QObject* ImageViewWidget::addLine(int xPos, int yPos, int groupNr, const QColor &color, int endX, int endY)
{
    LineItem* item = new LineItem(xPos, yPos, groupNr, color, endX, endY);
    items.push_back(item);
    return item;
}

QObject* ImageViewWidget::addEllipse(int xPos, int yPos, int groupNr, const QColor &color, int width, int height)
{
    EllipseItem* item = new EllipseItem(xPos, yPos, groupNr, color, width, height);
    items.push_back(item);
    return item;
}

QObject* ImageViewWidget::addRectangle(int xPos, int yPos, int groupNr, const QColor &color, int width, int height)
{
    RectangleItem* item = new RectangleItem(xPos, yPos, groupNr, color, width, height);
    items.push_back(item);
    return item;
}
QObject* ImageViewWidget::addPolyline(int groupNr, const QColor &color, const QList<QPoint> &points)
{
    PolylineItem* item = new PolylineItem(color, groupNr, points);
    items.push_back(item);
    return item;
}

QObject* ImageViewWidget::addPolygon(int groupNr, const QColor &color, const QList<QPoint> &points)
{
    PolygonItem* item = new PolygonItem(color, groupNr, points);
    items.push_back(item);
    return item;
}

QObject* ImageViewWidget::addItem(QObject* object)
{
    DrawItem* drawItem = dynamic_cast<DrawItem*>(object);
    items.push_back(drawItem);
    return object;
}

 QObject* ImageViewWidget::removeItem(QObject* object,bool delete_object)
{
    DrawItem *draw_item = dynamic_cast<DrawItem*>(object);
    items.removeAll(draw_item);
    if(delete_object)
    {
      delete object;
      object = NULL;
    }
    return object;
}

void ImageViewWidget::removeAllItems(bool delete_objects)
{
   if(delete_objects)
   {
      QList<DrawItem*>::iterator iter = items.begin();
      for(;iter != items.end();++iter)
          delete(*iter);
   }
   items.clear();
}

void ImageViewWidget::setGroupStatus(int groupNr, bool enable)
{
    if(!enable)
    {
        disabledGroups.push_back(groupNr);
    }
    else
    {
        disabledGroups.removeAll(groupNr);
    }
    repaint();
}

void ImageViewWidget::clearGroups()
{
    disabledGroups.clear();
}

void ImageViewWidget::addRawImage(const QString &mode, int pixel_size,  int width,  int height,const char* pbuffer)
{
  //check if image size has changed
  //zoom factor must be recalculated
  if(image.width() != width || image.height() != height)
    setScaleFactor();
  frame_converter.copyFrameToQImageRGB888(image,mode, pixel_size, width, height,pbuffer);
  updateGL();
}

void ImageViewWidget::addImage(const QImage &image)
{
  //check if image size has changed
  //zoom factor must be recalculated
  if(image.width() != this->image.width() || image.height() != this->image.height())
    setScaleFactor();
  this->image = image;
  updateGL();
}

void ImageViewWidget::addDrawItemsToWidget(QImage &shownImage)
{
    if(!items.empty())
    {
        QPainter painter(&shownImage);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
	QList<DrawItem*>::iterator iter = items.begin();
        for(;iter != items.end();++iter)
        {
	  if(!disabledGroups.contains((*iter)->getGroupNr()))
                (*iter)->draw(&painter);
        }
    }
}

void ImageViewWidget::setScaleFactor()
{
    glPixelZoom(((float)width())/ image.width(),-((float)height())/ image.height());
}

void ImageViewWidget::paintGL()
{
    glDrawPixels(image.width(), image.height(), GL_RGB, GL_UNSIGNED_BYTE, image.bits());
 // renderText(10,10,"Test");
}

void ImageViewWidget::resizeGL(int w, int h)
{
    setScaleFactor();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glRasterPos2i(0,h);
}


