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
ImageView(parent,false)
{
	image_view_gl = 0;
	use_openGL=true;
	setOpenGL(use_openGL);
	lastScale=0;
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

    image_view_gl = new SonarViewGL(*this);
    if (!image_view_gl)
      return;
    image_view_gl->resize(width(),height());
    //image_view_gl->setAspectRatio(aspect_ratio);
    image_view_gl->show();
    //SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
    //window->reset(0.024); //TODO hardcoded value
  }
  else
  {
    delete image_view_gl;
    image_view_gl = NULL;
  }
   
}

void SonarView::setSonarScan2(base::samples::SonarScan *scan){
	SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
	if(!window){
		fprintf(stderr,"Cannot set data have no widget?!\n");
		return;
	}
 	double newScale = (scan->time_beetween_bins/1e-9)/640.0;
	if(newScale != lastScale){
		window->reset(newScale);
		lastScale = newScale;
	} 
	window->setData(scan->scanData,scan->angle/2.0*M_PI*6399.0);
}

void SonarView::setDistance(double distance, double angle){
	SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
	if(!window){
		fprintf(stderr,"Cannot set data have no widget?!\n");
		return;
	}
	double bearing = angle/(M_PI*2.0)*6399.0;
	window->setWallDist(bearing,distance,0);
}

void SonarView::setSonarScan(const char *data_, int size, double angle, double timeBetweenBins,bool fromBearing){
	SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
	if(!window){
		fprintf(stderr,"Cannot set data have no widget?!\n");
		return;
	}
	double bearing = angle;
 	double newScale = ((timeBetweenBins*640.0)*1e-9);
	if(!fromBearing){
		bearing = angle/(M_PI*2.0)*6399.0;
 		newScale = timeBetweenBins*size/2.0;
	}
	newScale = (newScale*1500.0)/size;
	if(newScale != lastScale){
		lastScale = newScale;
		printf("new Scale: %f\n",newScale);	
		window->reset(newScale);
	}
 
	std::vector<uint8_t> data;
	for(int i=0;i<size;i++){
		data.push_back(data_[i]);
	}
	window->setData(data,bearing);
}

void SonarView::keyPressEvent ( QKeyEvent * event ){
	SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
	window->keyPressEvent(event);	
}


void SonarView::setPosition(double posX, double posY, double sigmaX, double sigmaY){
	SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
	if(window)window->setPosition(posX,posY,sigmaX,sigmaY);	
}
  
void SonarView::setOrientation(const double orientation){
	SonarViewGL *window = dynamic_cast<SonarViewGL*>(image_view_gl);
	if(window)window->setOrientation(orientation);
}

