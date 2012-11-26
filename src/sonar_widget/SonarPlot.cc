#include "./SonarPlot.h"
#include <iostream>


SonarPlot::SonarPlot(QWidget *parent)
    : QFrame(parent), mChangedSize(true),scaleX(1),scaleY(1),range(5)
{
  int alpha = 255;
  for(int i=0;i<64;i++){
    colorMap.push_back(QColor(0,4*i,255,alpha));
  }
  for(int i=64;i<128;i++){
    colorMap.push_back(QColor(0,255,255+4*(64-i),alpha));
  }
  for(int i=128;i<192;i++){
    colorMap.push_back(QColor(4*(i-128),255,0,alpha));
  }
  for(int i=192;i<256;i++){
    colorMap.push_back(QColor(255,255+4*(192-i),0,alpha));
  }
  QPalette Pal(palette());
  Pal.setColor(QPalette::Background, colorMap[0]);
  setAutoFillBackground(true);
  setPalette(Pal);  
}

SonarPlot::~SonarPlot()
{
}

void SonarPlot::setData(const base::samples::SonarScan scan)
{
  std::cout << "setData, number of beams " <<mSonarScan.number_of_beams <<" number of bins " << mSonarScan.number_of_bins <<std::endl;
  mSonarScan = scan;
  if(!mSonarScan.number_of_bins || !mSonarScan.number_of_beams){
    return;
  }
  if(mChangedSize){
    mRawPoints.clear();
    for(uint i=0;i<mSonarScan.data.size();i++){
      base::Angle ang = int(i/mSonarScan.number_of_bins)*mSonarScan.angular_resolution-mSonarScan.start_bearing;
      int radius = i % mSonarScan.number_of_bins;
      uint x = radius*sin(ang.getRad());
      uint y = radius*cos(ang.getRad());
      mRawPoints.append(QPoint(x,y));
    }
    fillPoints();
    mChangedSize=false;
 }
 update();
}


void SonarPlot::paintEvent(QPaintEvent *)
{
    if(mPoints.size()!=mSonarScan.data.size()){
      std::cout <<"mPoints.size()!=mSonarScan.data.size() mPoints.size(): " <<mPoints.size() <<" mSonarScan.data.size(): " <<mSonarScan.data.size() <<endl;
      return;
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for(uint i=0;i<mSonarScan.data.size();i++){
      painter.setPen(QPen(colorMap[ mSonarScan.data[i] ]));
      painter.drawPoint(mPoints[i]);
    }    
    drawOverlay();
    
}

void SonarPlot::resizeEvent ( QResizeEvent * event )
{
  scaleX = 0.2;
  if(width()>400){
    scaleX = double((width()-134))/866;
  }
  scaleY = 0.2;
  if(height()>200){
    scaleY = double((height()-100))/500;
  } 
  fillPoints();
  QWidget::resizeEvent (event);
}

void SonarPlot::drawOverlay()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::white));
    QPoint origin;
    origin.setX(width()/2);
    origin.setY(height()-30);
    for(int i=1;i<=5;i++){
      int angle = 120;
      painter.drawArc(width()/2-i*scaleX*100,height()-30-i*scaleY*100,i*200*scaleX,i*200*scaleY,(90-angle/2)*16,angle*16);
      QString str;
      str.setNum(i*range/5);
      int x = width()/2+i*100*scaleX*sin(angle*3.1416/360);
      int y = height()-i*100*scaleY*cos(angle*3.1416/360);
      painter.drawText(x,y-5,str);
      
      base::Angle sectorSize = mSonarScan.angular_resolution*mSonarScan.number_of_beams;
      base::Angle ang = sectorSize*(-0.5+(i-1)*0.25);
      QPoint p2;
      x = width()/2+mSonarScan.number_of_bins*sin(ang.getRad())*scaleX;
      y = height()-30-mSonarScan.number_of_bins*cos(ang.getRad())*scaleY;
      p2.setX(x);
      p2.setY(y);
      painter.drawLine(origin,p2);     
      str.setNum(ang.getDeg());
      x = p2.x();
      if(x<width()/2){
	x-=15;
      }
      if(x==width()/2){
	x-=5;
      }
      y = p2.y()-10;
      painter.drawText(x,y,str);
    } 
    for(int i=0;i<255;i++){
      painter.setPen(QPen(colorMap[i]));
      painter.setBrush(QBrush(colorMap[i]));
      painter.drawRect(width()-30,height()-10-i*2,20,2);
    }
}

void SonarPlot::fillPoints()
{
  mPoints.clear();
  uint w = width()/2;
  uint h = height()-30; 
  for(uint i=0;i<mRawPoints.size();i++){
      mPoints.append(QPoint(w+mRawPoints[i].x()*scaleX,h-mRawPoints[i].y()*scaleY));
  }
}

void SonarPlot::rangeChanged(int value)
{
    range = value;
    drawOverlay();
}
