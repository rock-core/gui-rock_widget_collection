/* 
 * File:   SonarViewGL.cc
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de) 
 * 
 */

#include <stdexcept>
#include "SonarView.h"
#include "SonarViewGL.h"
#include <QtCore/QtPlugin>
#include "SonarViewGL.h"

SonarView::SonarView(QWidget *parent,bool use_openGL):
ImageView(parent,true, new SonarViewGL())
{

}


SonarView::~SonarView()
{
}

void SonarView::setOpenGL(bool flag)
{
  if(flag)
  {
    //prevent activating if it is already activated
    if(image_view_gl)
      return;

    image_view_gl = new SonarViewGL();
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

void SonarView::setSonarScan(base::samples::SonarScan *scan){
	SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
	if(!window){
		fprintf(stderr,"Cannot set data have no widget?!\n");
		return;
	}
	window->setData(scan->scanData,scan->angle/2.0*M_PI*6399.0);
}


