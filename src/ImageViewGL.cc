/* 
 * File:   ImageViewGL.cc
 * Author: blueck
 * 
 * Created on 17. Juni 2010, 14:14
 */

#include "ImageViewGL.h"

ImageViewGL::ImageViewGL(QWidget *parent,
                         QImage &image,
                         QList<DrawItem*> &items,
                         QList<int> &disabledGroups):
  QGLWidget(parent),
  image(image),
  items(items),
  disabledGroups(disabledGroups)
{

}

ImageViewGL::~ImageViewGL()
{
}

void ImageViewGL::paintGL()
{
    glDrawPixels(image.width(), image.height(), GL_RGB, GL_UNSIGNED_BYTE, image.bits());
    QList<DrawItem*>::iterator iter = items.begin();
    for(;iter != items.end();++iter)
    {
      if(!disabledGroups.contains((*iter)->getGroupNr()))
        if((*iter)->getRenderOnOpenGl())
          (*iter)->renderOnGl(*this);
    }
}

void ImageViewGL::resizeGL(int w, int h)
{
  float x = ((float)width())/ image.width();
  float y = ((float)height())/ image.height();
  glPixelZoom(x,-y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  glRasterPos2i(0,h);
  paintGL();
}
