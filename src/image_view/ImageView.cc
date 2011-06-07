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
  MultiWidget(parent),
  contextMenu(this),
  image(0, 0, QImage::Format_RGB888),
  aspect_ratio(false),
  zoomEnabled(false)
{
    resize(width(),height());
    //setting up popup menue
    save_image_act = new QAction(tr("&Save Image"),this);
    user_export_act = new QAction(tr("&User Export"),this);
    save_image_act->setStatusTip(tr("Save the image to disk"));
    user_export_act->setStatusTip(tr("Emits an event for user specific export"));
    connect(save_image_act,SIGNAL(triggered()),this,SLOT(saveImage()));
    connect(user_export_act,SIGNAL(triggered()),this,SLOT(emitUserExport()));
    contextMenu.addAction(save_image_act);
    contextMenu.addAction(user_export_act);

    //setting up widget
    setWindowTitle(tr("ImageView"));
    setDefaultImage();
    calculateRects();
  
    //configure opengl if activated
    image_view_gl = NULL;
    openGL(use_openGL);
}


ImageView::~ImageView()
{
    disconnect(save_image_act, 0, 0, 0);
    delete save_image_act;
}

void ImageView::emitUserExport()
{
    emit userExport();
}


void ImageView::setZoomEnabled(bool enabled)
{
  zoomEnabled = enabled;
}

void ImageView::contextMenuEvent ( QContextMenuEvent * event )
{
   contextMenu.exec(event->globalPos());
}

std::pair<QPoint, bool> ImageView::toImage(QPoint const& p)
{
  //calculate pos on image 
  int offset_x = target.x();
  int offset_y = target.y();
  float scale_x = source.width()/target.width();
  float scale_y = source.height()/target.height();
  
  int x = (int)(scale_x*(p.x()-offset_x)+0.5);
  int y = (int)(scale_y*(p.y()-offset_y)+0.5);

  bool valid = (x>=0 && x <= source.width() &&
      y>=0 && y <= source.height());

  if (!valid)
  {
    x = x < 0 ? 0 : source.width() - 1;
    y = y < 0 ? 0 : source.height() - 1;
  }
  x += currentCrop.x();
  y += currentCrop.y();
  return std::make_pair(QPoint(x, y), valid);
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
  std::pair<QPoint, bool> img = toImage(QPoint(event->x(), event->y()));
  pressPoint = img.first;
  pressValid = img.second;
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->modifiers() & Qt::ControlModifier)
  {
    // Cancel current zoom
    if (!zoomEnabled)
      return;

    resetCrop();
    update();
    return;
  }

  std::pair<QPoint, bool> imgPoint = toImage(QPoint(event->x(), event->y()));
  QPoint releasePoint = imgPoint.first;
  int distance = (releasePoint - pressPoint).manhattanLength();
  if (zoomEnabled && distance > 5)
  {
    int w = abs((pressPoint - releasePoint).x()) + 1;
    int h = abs((pressPoint - releasePoint).y()) + 1;
    int x = std::min(pressPoint.x(), releasePoint.x());
    int y = std::min(pressPoint.y(), releasePoint.y());
    crop(x, y, w, h);
    update();
  }
  else if (pressValid)
    emit clickImage(pressPoint.x(), pressPoint.y());
}

void ImageView::setDefaultImage()
{
   static QColor background(127, 127, 127);
   static QColor textcolor(255,255,255);
   this->originalImage = QImage(width(),height(),QImage::Format_RGB888);
   QPainter painter(&this->originalImage);
   painter.setBrush(background);
   painter.drawRect(0,0,image.width(),image.height());
   painter.setBrush(textcolor);
   painter.drawText(0,0,image.width(),image.height(),
                    Qt::AlignVCenter|Qt::AlignHCenter,"No Signal");
   image = originalImage;
   no_input = true;
}

void ImageView::openGL(bool flag)
{
  if(flag)
  {
    //prevent activating if it is already activated
    if(image_view_gl)
      return;

    image_view_gl = new ImageViewGL(*this);
    if (!image_view_gl)
      return;
    image_view_gl->resize(width(),height());
    image_view_gl->setAspectRatio(aspect_ratio);
    image_view_gl->show();
  }
  else
  {
    delete image_view_gl;
    image_view_gl = NULL;
  }
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
    return true;
}

bool ImageView::saveImage3(const QString &mode, int pixel_size,  int width,  int height,const char* pbuffer, QString path, QString format)
{
    QImage image;
    frame_converter.copyFrameToQImageRGB888(image,mode, pixel_size, width, height,pbuffer);
    image.save(path,format.toAscii().data(),80);
    return true;
}

QObject* ImageView::addText(int xPos, int yPos, int groupNr, const QString &text)
{
    TextItem* textItem = new TextItem(xPos, yPos, groupNr, text);
    if(image_view_gl)
      textItem->openGL(true);
    items.push_back(textItem);
    return textItem;
}

QObject* ImageView::addLine(int xPos, int yPos, int groupNr, const QColor &color, int endX, int endY)
{
    LineItem* item = new LineItem(xPos, yPos, groupNr, color, endX, endY);
    if(image_view_gl)
      item->openGL(true);

    items.push_back(item);
    return item;
}

QObject* ImageView::addEllipse(int xPos, int yPos, int groupNr, const QColor &color, int width, int height)
{
    EllipseItem* item = new EllipseItem(xPos, yPos, groupNr, color, width, height);
    if(image_view_gl)
      item->openGL(true);

    items.push_back(item);
    return item;
}

QObject* ImageView::addRectangle(int xPos, int yPos, int groupNr, const QColor &color, int width, int height)
{
    RectangleItem* item = new RectangleItem(xPos, yPos, groupNr, color, width, height);
    if(image_view_gl)
      item->openGL(true);

    items.push_back(item);
    return item;
}
QObject* ImageView::addPolyline(int groupNr, const QColor &color, const QList<QPoint> &points)
{
    PolylineItem* item = new PolylineItem(color, groupNr, points);
    if(image_view_gl)
      item->openGL(true);

    items.push_back(item);
    return item;
}

QObject* ImageView::addPolygon(int groupNr, const QColor &color, const QList<QPoint> &points)
{
    PolygonItem* item = new PolygonItem(color, groupNr, points);
    if(image_view_gl)
      item->openGL(true);

    items.push_back(item);
    return item;
}

QObject* ImageView::addItem(QObject* object)
{
    DrawItem* drawItem = dynamic_cast<DrawItem*>(object);
    if(image_view_gl)
      drawItem->openGL(true);

    items.push_back(drawItem);
    return object;
}

QObject* ImageView::addPoints(const QList<int> &points_x,const QList<int> &points_y,int groupNr, const QColor &color)
{
    DrawItem* drawItem = new PointItem(points_x,points_y,groupNr,color);
    if(image_view_gl)
      drawItem->openGL(true);
    items.push_back(drawItem);
    return drawItem;
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

void ImageView::resetCrop()
{
  this->image = this->originalImage;
  currentCrop = QRect(0, 0, image.width(), image.height());
  calculateRects();
}

void ImageView::crop(int x, int y, int w, int h)
{
  this->image = this->image.copy(x, y, w, h);
  currentCrop = QRect(x, y, w, h);
  calculateRects();
}

void ImageView::addRawImage(const QString &mode, int pixel_size,  int width,  int height,const char* pbuffer)
{
  //check if image size has been changed
  //zoom factor must be recalculated
  if(frame_converter.copyFrameToQImageRGB888(originalImage,mode, pixel_size, width, height,pbuffer))
  {
    this->image = this->originalImage;
    if(image_view_gl)
    {
      image_view_gl->setGLViewPoint();
      calculateRects();
    }
  }
  this->image = this->originalImage;
  no_input = false;
}

void ImageView::addImage(const QImage &image)
{
  //check if image size has changed
  //zoom factor must be recalculated
  this->image = this->originalImage = image;
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
	    if(!(*iter)->onOpenGL() || all == true)
                (*iter)->draw(&painter);
        }
    }
}

void ImageView::drawDrawItemsToImage(QImage &image,bool all)
{
    QPainter painter(&image);
    drawDrawItemsToPainter(painter,all);
}

//slots from base classes are shadowed in ruby therefore update2 is to
//access ImageView::update from ruby
void ImageView::update2()
{
  update();
}

void ImageView::update()
{
  if(image_view_gl)
  {
    //only opengl overlays are displayed
    image_view_gl->repaint();
  }
  else
  {
    QWidget::update();
  }
}

void ImageView::calculateRects()
{
  int x_offset = 0;
  int y_offset = 0;
  if(aspect_ratio)
  {
    float x =1; 
    float y =1; 
    if (image.width() && image.height())
    {
      x = ((float)width())/ image.width();
      y = ((float)height())/ image.height();
    }
    if(x < y)
      y_offset =  0.5f*(height()-x*image.height());
    else
      x_offset =  0.5f*(width()-y*image.width());
  }
  target.setRect(x_offset, y_offset, width()-x_offset*2, height()-y_offset*2);
  source.setRect(0.0, 0.0, image.width(), image.height());
}

void ImageView::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawImage(target, image, source);
  drawDrawItemsToPainter(painter,true);
}

void ImageView::resizeEvent ( QResizeEvent * event )
{
  QWidget::resizeEvent(event);

  if(no_input)
    setDefaultImage();
  if(image_view_gl)
    image_view_gl->resize(width(),height());

  calculateRects();
}

void ImageView::mouseDoubleClickEvent( QMouseEvent * event )
{
}

