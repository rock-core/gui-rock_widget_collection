/* 
 * File:   RangeViewGL.cc
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de) 
 * 
 */

#include <stdexcept>
#include "RangeView.h"
#include "RangeViewGL.h"
#include <QtCore/QtPlugin>
#include "RangeViewGL.h"

RangeView::RangeView(QWidget *parent,bool use_openGL):
ImageView(parent,false)
{
	setOpenGL(use_openGL);
}


RangeView::~RangeView()
{
}

void RangeView::setOpenGL(bool flag)
{
  if(flag)
  {
    //prevent activating if it is already activated
    if(image_view_gl)
      return;

    image_view_gl = new RangeViewGL(this);
    if (!image_view_gl)
      return;
    image_view_gl->resize(width(),height());
    //image_view_gl->setAspectRatio(aspect_ratio);
    image_view_gl->show();
    RangeViewGL *window = dynamic_cast<RangeViewGL*>(image_view_gl);
  }
  else
  {
    delete image_view_gl;
    image_view_gl = NULL;
  }
   
}

void RangeView::setRangeScan2(base::samples::LaserScan *scan){
	RangeViewGL *window = dynamic_cast<RangeViewGL*>(image_view_gl);
	if(!window){
		fprintf(stderr,"Cannot set data have no widget?!\n");
		return;
	}
	std::vector<Eigen::Vector3d> data = scan->convertScanToPointCloud(Eigen::Transform3d());
	window->setData(data);
}

/*
void RangeView::setRangeScan(const char *data_, int size, double angle, bool fromBearing){
	RangeViewGL *window = dynamic_cast<RangeViewGL*>(image_view_gl);
	if(!window){
		fprintf(stderr,"Cannot set data have no widget?!\n");
		return;
	}
	double bearing =angle;
	if(!fromBearing)
		bearing = angle/2.0*M_PI*6399.0;
	
	std::vector<uint8_t> data;
	for(int i=0;i<size;i++){
		data.push_back(data_[i]);
	}
	window->setData(data,bearing);
}
*/

void RangeView::keyPressEvent ( QKeyEvent * event ){
	RangeViewGL *window = dynamic_cast<RangeViewGL*>(image_view_gl);
	window->keyPressEvent(event);	
}
