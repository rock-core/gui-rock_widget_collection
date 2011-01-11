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

class ImageViewGL :public QGLWidget
{
    Q_OBJECT
  private:
    void paintGL();
    void resizeGL(int w, int h);
    bool aspect_ratio;

  public:
    ImageViewGL(QWidget *parent,QImage &image,
                QList<DrawItem*> &items,
                QList<int> &disabledGroups);

    virtual ~ImageViewGL();
    void setGLViewPoint(int display_width=0,int display_height=0);
    void setAspectRatio(bool value){aspect_ratio = value;};

  private:
    QImage &image;
    QList<DrawItem*> &items;
    QList<int> &disabledGroups;
};

#endif	/* IMAGEVIEWWGL_H */

