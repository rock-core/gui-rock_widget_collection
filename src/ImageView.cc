/* 
 * File:   ImageView.cc
 * Author: blueck
 * 
 * Created on 17. Juni 2010, 14:14
 */

#include "ImageView.h"
#include <stdexcept>
#include <QtCore/QtPlugin>

using namespace base::samples::frame;
//Q_EXPORT_PLUGIN2(ImageView, ImageView)

ImageView::ImageView(QWidget *parent,bool use_openGL):
  image(0, 0, QImage::Format_RGB888),
  contextMenu(this),
  fixed_size(false),
  QWidget(parent)
{
    //setting up popup menue
    save_image_act = new QAction(tr("&Save Image"),this);
    save_image_act->setStatusTip(tr("Save the image to disk"));
    connect(save_image_act,SIGNAL(triggered()),this,SLOT(saveImage()));
    contextMenu.addAction(save_image_act);

    //setting up widget
    setWindowTitle(tr("ImageView"));
    setDefaultImage();
  
    //configure opengl if activated
    image_view_gl = NULL;
    useOpenGL(use_openGL);
}


ImageView::~ImageView()
{
    disconnect(save_image_act, 0, 0, 0);
    delete save_image_act;
}

void ImageView::contextMenuEvent ( QContextMenuEvent * event )
{
   contextMenu.exec(event->globalPos());
}


void ImageView::setDefaultImage()
{
   static QColor background(127, 127, 127);
   static QColor textcolor(255,255,255);
   image = QImage(width(),height(),QImage::Format_RGB888);
   QPainter painter(&image);
   painter.setBrush(background);
   painter.drawRect(0,0,image.width(),image.height());
   painter.setBrush(textcolor);
   painter.drawText(0,0,image.width(),image.height(),
                    Qt::AlignVCenter|Qt::AlignHCenter,"No Signal");
   no_input = true;
}

bool ImageView::useOpenGL(bool flag)
{
  if(flag)
  {
    //prevent activating if it is already activated
    if(image_view_gl)
      return true;

    image_view_gl = new ImageViewGL(this,image,items,disabledGroups);
    if (!image_view_gl)
      return false;
    image_view_gl->resize(width(),height());
  }
  else
  {
    delete image_view_gl;
    image_view_gl = NULL;
  }
  return true;
}

void ImageView::saveImage(bool overlay)
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"),
                            save_path,
                            tr("Images (*.png)"));
    if(path.length() > 0)	
      saveImage2(path,overlay);
}
bool ImageView::saveImage2(QString path,bool overlay)
{
    if(overlay)
    {
      QImage temp(image);
      drawDrawItemsToImage(temp,true);
      temp.save(path,"PNG",80);
    }
    else
      image.save(path,"PNG",80);
}

bool ImageView::saveImage3(const QString &mode, int pixel_size,  int width,  int height,const char* pbuffer, QString path)
{
    QImage image;
    frame_converter.copyFrameToQImageRGB888(image,mode, pixel_size, width, height,pbuffer);
    image.save(path,"PNG",80);
}

QObject* ImageView::addText(int xPos, int yPos, int groupNr, const QString &text)
{
    TextItem* textItem = new TextItem(xPos, yPos, groupNr, text);
    items.push_back(textItem);
    return textItem;
}

QObject* ImageView::addLine(int xPos, int yPos, int groupNr, const QColor &color, int endX, int endY)
{
    LineItem* item = new LineItem(xPos, yPos, groupNr, color, endX, endY);
    items.push_back(item);
    return item;
}

QObject* ImageView::addEllipse(int xPos, int yPos, int groupNr, const QColor &color, int width, int height)
{
    EllipseItem* item = new EllipseItem(xPos, yPos, groupNr, color, width, height);
    items.push_back(item);
    return item;
}

QObject* ImageView::addRectangle(int xPos, int yPos, int groupNr, const QColor &color, int width, int height)
{
    RectangleItem* item = new RectangleItem(xPos, yPos, groupNr, color, width, height);
    items.push_back(item);
    return item;
}
QObject* ImageView::addPolyline(int groupNr, const QColor &color, const QList<QPoint> &points)
{
    PolylineItem* item = new PolylineItem(color, groupNr, points);
    items.push_back(item);
    return item;
}

QObject* ImageView::addPolygon(int groupNr, const QColor &color, const QList<QPoint> &points)
{
    PolygonItem* item = new PolygonItem(color, groupNr, points);
    items.push_back(item);
    return item;
}

QObject* ImageView::addItem(QObject* object)
{
    DrawItem* drawItem = dynamic_cast<DrawItem*>(object);
    items.push_back(drawItem);
    return object;
}

 QObject* ImageView::removeItem(QObject* object,bool delete_object)
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

void ImageView::removeAllItems(bool delete_objects)
{
   if(delete_objects)
   {
      QList<DrawItem*>::iterator iter = items.begin();
      for(;iter != items.end();++iter)
          delete(*iter);
   }
   items.clear();
}

void ImageView::setGroupStatus(int groupNr, bool enable)
{
    if(!enable)
    {
        disabledGroups.push_back(groupNr);
    }
    else
    {
        disabledGroups.removeAll(groupNr);
    }
}

void ImageView::clearGroups()
{
    disabledGroups.clear();
}

void ImageView::addRawImage(const QString &mode, int pixel_size,  int width,  int height,const char* pbuffer)
{
  //check if image size has been changed
  //zoom factor must be recalculated
  frame_converter.copyFrameToQImageRGB888(image,mode, pixel_size, width, height,pbuffer);
  no_input = false;
}

void ImageView::addImage(const QImage &image)
{
  //check if image size has changed
  //zoom factor must be recalculated
  this->image = image;
  no_input = false;
}

void ImageView::drawDrawItemsToPainter(QPainter &painter,bool all)
{
    if(!image_view_gl)
      all = true;

    if(!items.empty())
    {
        painter.setRenderHint(QPainter::TextAntialiasing, true);
	QList<DrawItem*>::iterator iter = items.begin();
        for(;iter != items.end();++iter)
        {
	  if(!disabledGroups.contains((*iter)->getGroupNr()))
	    if(!(*iter)->getRenderOnOpenGl())
                (*iter)->draw(&painter);
        }
    }
}

void ImageView::drawDrawItemsToImage(QImage &image,bool all)
{
    QPainter painter(&image);
    drawDrawItemsToPainter(painter,all);
}

void ImageView::paintEvent(QPaintEvent *)
{
  if(image_view_gl)
  {
    //only opengl overlays are displayed
    image_view_gl->repaint();
  }
  else
  {
    //calc apect ratio
    QRectF target(0, 0, width(), height());
    QRectF source(0.0, 0.0, image.width(), image.height());
    QPainter painter(this);
    painter.drawImage(target, image, source);
    drawDrawItemsToPainter(painter,true);
  }
}

void ImageView::resizeEvent ( QResizeEvent * event )
{
  QWidget::resizeEvent(event);
  if(no_input)
    setDefaultImage();
  if(image_view_gl)
    image_view_gl->resize(width(),height());
}

void ImageView::mouseDoubleClickEvent ( QMouseEvent * event )
{
}

