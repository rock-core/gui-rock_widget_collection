/* 
 * File:   ImageViewGL.h
 * Author: blueck
 *
 * Created on 17. Juni 2010, 14:14
 */
#ifndef IMAGEVIEWGL_H
#define	IMAGEVIEWGL_H

#include <QtOpenGL/QGLWidget>   //opengl support

#include "LineItem.h"
#include "TextItem.h"
#include "EllipseItem.h"
#include "PolygonItem.h"
#include "PolylineItem.h"
#include "PointItem.h"

class ImageView;

class ImageViewGL :public QGLWidget
{
    Q_OBJECT
  private:
    void paintGL();
    void resizeGL(int w, int h);
    bool aspect_ratio;

  public:
    ImageViewGL(ImageView &parent);

    virtual ~ImageViewGL();
    void setGLViewPoint(int display_width=0,int display_height=0);
    void setAspectRatio(bool value){aspect_ratio = value;};

  private:
    ImageView &image_view;    
};

#endif	/* IMAGEVIEWWGL_H */

