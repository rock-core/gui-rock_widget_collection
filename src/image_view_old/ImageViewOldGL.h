/* 
 * File:   ImageViewOldGL.h
 * Author: blueck
 *
 * Created on 17. Juni 2010, 14:14
 */
#ifndef IMAGEVIEWOLDGL_H
#define	IMAGEVIEWOLDGL_H

#include <QtOpenGL/QGLWidget>   //opengl support

#include "LineItem.h"
#include "TextItem.h"
#include "EllipseItem.h"
#include "PolygonItem.h"
#include "PolylineItem.h"
#include "PointItem.h"

class ImageViewOld;

/*!
 * @deprecated This class is deprecated and will no longer be maintained. You should use the new ImageView widget.
 */
class ImageViewOldGL :public QGLWidget
{
    Q_OBJECT
  private:
    void paintGL();
    void resizeGL(int w, int h);
    bool aspect_ratio;

  public:
    ImageViewOldGL(ImageViewOld &parent);

    virtual ~ImageViewOldGL();
    void setGLViewPoint(int display_width=0,int display_height=0);
    void setAspectRatio(bool value){aspect_ratio = value;};
    
  private:
    ImageViewOld &image_view;    
};

#endif	/* IMAGEVIEWOLDGL_H */

