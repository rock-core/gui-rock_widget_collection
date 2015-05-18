#include "./SonarPlot.h"
#include <iostream>

using namespace std;

SonarPlot::SonarPlot(QWidget *parent)
    : QFrame(parent), changedSize(true),scaleX(1),scaleY(1),range(5)
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
  lastSonarScan = sonarScan;
  lastSonarScan.data.clear();
  sonarScan = scan;
  if(!sonarScan.number_of_bins || !sonarScan.number_of_beams){
    return;
  }
  if(changedSize
    || !(sonarScan.start_bearing == lastSonarScan.start_bearing)
    || !(sonarScan.angular_resolution == lastSonarScan.angular_resolution)
    || !(sonarScan.number_of_beams == lastSonarScan.number_of_beams)
    || !(sonarScan.number_of_bins == lastSonarScan.number_of_bins)){
    pixelList.clear();
    for (int i = 0; i< width();i++){
        for (int j = 0; j< origin.y();j++){
            QPoint point(i,j);
            point -= origin;
            point.rx()/=scaleX*BINS_REF_SIZE/sonarScan.number_of_bins;
            point.ry()/=scaleY*BINS_REF_SIZE/sonarScan.number_of_bins;
            int radius = sqrt(point.x()*point.x() + point.y()*point.y());
            base::Angle angle = base::Angle::fromDeg(0);
            if(radius){
                angle = base::Angle::fromRad(asin(double(point.x())/radius));
            }
            if(radius <= sonarScan.number_of_bins && fabs(angle.getDeg()) <= fabs(sonarScan.start_bearing.getDeg())){
                uint index = abs(int((angle-sonarScan.start_bearing).getDeg()/sonarScan.angular_resolution.getDeg()))*sonarScan.number_of_bins + radius;
                if(index<=sonarScan.data.size()){
                    SonarPlotPixel sonarPlotPixel;
                    sonarPlotPixel.point = QPoint(i,j);
                    sonarPlotPixel.index = index;
                    pixelList.append(sonarPlotPixel);
                }
            }
        }
    }
    changedSize=false;
 }
 update();
}


void SonarPlot::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for(int i=0;i<pixelList.size();i++){
        painter.setPen(QPen(colorMap[sonarScan.data[pixelList[i].index]]));
        painter.drawPoint(pixelList[i].point);
    }
    drawOverlay();
    
}

void SonarPlot::resizeEvent ( QResizeEvent * event )
{
    scaleX = 0.2;
    if(width()>400){
        scaleX = double(width())/(BASE_WIDTH-134);
    }
    scaleY = 0.2;
    if(height()>200){
        scaleY = double(height()-100)/(BASE_HEIGHT-100);
    } 
    origin.setX(width()/2);
    origin.setY(height()-30);
    changedSize=true;
    QWidget::resizeEvent (event);
}

void SonarPlot::drawOverlay()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::white));
    for(int i=1;i<=5;i++){
      base::Angle sectorSize = sonarScan.angular_resolution*(sonarScan.number_of_beams-1);
      painter.drawArc(width()/2-i*scaleX*100,height()-30-i*scaleY*100,i*200*scaleX,i*200*scaleY,(90-sectorSize.getDeg()/2)*16,sectorSize.getDeg()*16);
      QString str;
      str.setNum(i*range/5);
      int x = width()/2+i*100*scaleX*sin(sectorSize.getDeg()*3.1416/360);
      int y = height()-i*100*scaleY*cos(sectorSize.getDeg()*3.1416/360);
      painter.drawText(x,y-5,str);      
      base::Angle ang = sectorSize*(-0.5+(i-1)*0.25);
      QPoint p2;
      x = width()/2+BINS_REF_SIZE*sin(ang.getRad())*scaleX;
      y = height()-30-BINS_REF_SIZE*cos(ang.getRad())*scaleY;
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

void SonarPlot::rangeChanged(int value)
{
    range = value;
    drawOverlay();
}
