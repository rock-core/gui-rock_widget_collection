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
ImageView(parent,use_openGL)
{
    	image_view_gl = NULL;
	//setOpenGL(use_openGL);
	setOpenGL(true);
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

    image_view_gl = new RangeViewGL(*this);
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
	
void RangeView::setRangeScan3(const QList<double> &data){
	setRangeScan(data.toVector().data(),data.size());
}

void RangeView::setRangeScan(double *data, int size){
	if(size%3!=0){
		fprintf(stderr,"Cannot set data from non division of three\n");
	}
	RangeViewGL *window = dynamic_cast<RangeViewGL*>(image_view_gl);
	if(!window){
		fprintf(stderr,"Cannot set data have no widget?!\n");
		return;
	}
	std::vector<Eigen::Vector3d> points;
	for(int i=0;i<size;i+=3){
		points.push_back(Eigen::Vector3d(data[i],data[i+1],data[i+2]));
		
	}
	window->setData(points);
}

void RangeView::keyPressEvent ( QKeyEvent * event ){
	RangeViewGL *window = dynamic_cast<RangeViewGL*>(image_view_gl);
	window->keyPressEvent(event);	
}
