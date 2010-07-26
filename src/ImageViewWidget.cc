/* 
 * File:   ImageViewWidget.cc
 * Author: blueck
 * 
 * Created on 17. Juni 2010, 14:14
 */

#include "ImageViewWidget.h"
#include <stdexcept>

using namespace base::samples::frame;

ImageViewWidget::ImageViewWidget(int width, int height, QImage::Format format):
format(format),width(width),height(height),image(width, height, format)
{
    setMinimumSize(QSize(width, height));
}

ImageViewWidget::~ImageViewWidget()
{
//   items.clear();
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

QObject* ImageViewWidget::addPolyline(int groupNr, const QColor &color, QPoint* points, int numberOfPoints)
{
    PolylineItem* item = new PolylineItem(color, groupNr, points, numberOfPoints);
    items.push_back(item);
    return item;
}

QObject* ImageViewWidget::addPolygon(int groupNr, const QColor &color, QPoint* points, int numberOfPoints)
{
    PolygonItem* item = new PolygonItem(color, groupNr, points, numberOfPoints);
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


void ImageViewWidget::changeFormat(int width, int height, QImage::Format format)
{
    image = QImage(width, height, format);
}

void ImageViewWidget::changeFormat2(QString mode, int pixel_size, int width, int height)
{
  std::string temp = mode.toStdString();
  base::samples::frame::frame_mode_t _mode = Frame::toFrameMode(temp);
  if(_mode == MODE_BAYER_RGGB || _mode == MODE_BAYER_GRBG || _mode == MODE_BAYER_BGGR || _mode == MODE_BAYER_GBRG)
     changeFormat(width,height,QImage::Format_RGB888);
  else
     changeFormat(width,height,getFormat(_mode,pixel_size));
}

QImage::Format ImageViewWidget::getFormat(frame_mode_t mode,int pixel_size)
{
  switch (Frame::getChannelCount(mode))
  {
  case 1:
      switch (pixel_size)
      {
	case 1:
	  return QImage::Format_Indexed8;
	default:
	  throw "Unknown format. Can not convert Frame "
		"to QImage.";
      }
      break;
  case 3:
	switch (pixel_size)
	{
	case 3:
	    return QImage::Format_RGB888;
	default:
	    throw "Unknown format. Can not convert Frame "
	    "to QImage.";
	}
	break;
  default:
      throw "Unknown format. Can not convert Frame "
	    "to QImage.";
  }
  return QImage::Format_Invalid;
}

void ImageViewWidget::addRawImage(const QString &mode, int pixel_size,  int width,  int height,const char* pbuffer)
{
    std::string temp = mode.toStdString();
    base::samples::frame::frame_mode_t _mode = Frame::toFrameMode(temp);
    if(_mode == MODE_UNDEFINED)
      throw std::runtime_error(" ImageViewWidget::addRawImage: Wrong mode");
    
    //convert bayer pattern to rgb24 (rgb888) 
    if(_mode == MODE_BAYER_RGGB || _mode == MODE_BAYER_GRBG || _mode == MODE_BAYER_BGGR || _mode == MODE_BAYER_GBRG)
        FrameHelper::convertBayerToRGB24((const uint8_t*)pbuffer,(uint8_t*) this->image.bits(), width, height ,_mode);
    else
    {
	QImage::Format format = getFormat(_mode,pixel_size);
	memcpy( this->image.bits(),pbuffer,width*height*pixel_size);
    }
    repaint();
}

void ImageViewWidget::addImage(const QImage &image)
{
    memcpy(this->image.bits(), image.bits(),  image.numBytes());
    repaint();
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

void ImageViewWidget::paintEvent(QPaintEvent* event)
{
   /* QImage shownImage(image);
    addDrawItemsToWidget(shownImage);
    QPainter qpainter(this);
    qpainter.drawImage(0, 0, shownImage);
    */
    addDrawItemsToWidget(image);
    QPainter qpainter(this);
    qpainter.drawImage(0, 0, image);
}


