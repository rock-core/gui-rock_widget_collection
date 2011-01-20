/* 
 * File:   ImageViewGL.cc
 * Author: blueck
 * 
 * Created on 17. Juni 2010, 14:14
 */
#include <iostream>
#include "ImageViewGL.h"

ImageViewGL::ImageViewGL(QWidget *parent) :
QGLWidget(parent),
image(new QImage()),
items(new QList<DrawItem*>()),
disabledGroups(new QList<int>())
{
}


ImageViewGL::ImageViewGL(QWidget *parent,
                         QImage &image,
                         QList<DrawItem*> &items,
                         QList<int> &disabledGroups):
  QGLWidget(parent),
  image(&image),
  items(&items),
  disabledGroups(&disabledGroups)
{
}

ImageViewGL::~ImageViewGL()
{
}

void ImageViewGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawPixels(image->width(), image->height(), GL_RGB, GL_UNSIGNED_BYTE, image->bits());
    QList<DrawItem*>::iterator iter = items->begin();
    for(;iter != items->end();++iter)
    {
      if(!disabledGroups->contains((*iter)->getGroupNr()))
        if((*iter)->getRenderOnOpenGl())
          (*iter)->renderOnGl(*this);
    }
}

void ImageViewGL::setGLViewPoint(int display_width,int display_height)
{
  if(!display_width || !display_height)
  {
    display_width = width();
    display_height = height();
  }

  float x =1; 
  float y =1; 
 
  if (image->width() && image->height())
  {
    x = ((float)display_width)/ image->width();
    y = ((float)display_height)/ image->height();
  }

  int x_offset = 0;
  int y_offset = 0;
  if(aspect_ratio)
  {
    if(x < y)
    {
      y_offset =  -0.5f*(display_height-x*image->height());
      y = x;
    }
    else
    {
      x_offset =  0.5f*(display_width-y*image->width());
      x = y;
    }
  }
  glPixelZoom(x,-y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, display_width, 0, display_height, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glViewport(0,0, display_width, display_height);
  glRasterPos2i(x_offset,display_height+y_offset);
}

void ImageViewGL::resizeGL(int w, int h)
{
  setGLViewPoint(w,h);
  paintGL();
}
