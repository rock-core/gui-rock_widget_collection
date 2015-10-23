#include "./SonarPlot.h"
#include <iostream>

using namespace std;
using namespace colorgradient;

SonarPlot::SonarPlot(QWidget *parent)
    : QFrame(parent), changedSize(true),scaleX(1),scaleY(1),range(5)
{
    // apply default colormap
    colormapSelector(COLORMAP_JET);

      QPalette Pal(palette());
      Pal.setColor(QPalette::Background, QColor(0,0,255));
      setAutoFillBackground(true);
      setPalette(Pal);
}

SonarPlot::~SonarPlot()
{
}

void SonarPlot::setData(const base::samples::SonarScan scan)
{
  if(!scan.number_of_bins || !scan.number_of_beams){
    return;
  }
  if(changedSize
    || !(scan.start_bearing == lastSonarScan.start_bearing)
    || !(scan.angular_resolution == lastSonarScan.angular_resolution)
    || !(scan.number_of_beams == lastSonarScan.number_of_beams)
    || !(scan.number_of_bins == lastSonarScan.number_of_bins)){

    // set bearing correction look-up table
    generateBearingTable(scan);

    // set the transfer vector between image pixels and sonar data
    transfer.clear();

    // check pixels
    for (int i = 0; i< width();i++){
        for (int j = 0; j< origin.y();j++){

            QPoint point(i - origin.x(), j - origin.y());
            point.rx() /= scaleX * BINS_REF_SIZE / scan.number_of_bins;
            point.ry() /= scaleY * BINS_REF_SIZE / scan.number_of_bins;

            double radius = sqrt(point.x() * point.x() + point.y() * point.y());
            double angle = asin(point.x() * 1.0 / radius);
            base::Angle theta = base::Angle::fromRad(angle);

            // pixels out of sonar image
            if (theta.rad < bearingTable[0].rad || theta.rad > bearingTable[scan.number_of_beams - 1].rad || radius > scan.number_of_bins)
                transfer.push_back(-1);

            // pixels inside the sonar image
            else {
                int start_beam;
                theta.rad <= 0 ? start_beam = 0 : start_beam = scan.number_of_beams / 2;
                for (int i = start_beam; i < scan.number_of_beams; i++) {
                    if (theta.rad >= bearingTable[i].rad && theta.rad < bearingTable[i + 1].rad) {
                        transfer.push_back(i * scan.number_of_bins + radius);
                        break;
                    }
                }
            }
        }
    }
    changedSize=false;
 }
 lastSonarScan = scan;
 update();
}


void SonarPlot::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // draw sonar image
    for (uint i = 0; i < transfer.size() && !changedSize; ++i) {
        if (transfer[i] != -1) {
            painter.setPen(colorMap[lastSonarScan.data[transfer[i]]]);
            painter.drawPoint(i / origin.y(), i % origin.y());
        }
    }

    // draw overlay
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
      base::Angle sectorSize = lastSonarScan.angular_resolution*(lastSonarScan.number_of_beams-1);
      painter.drawArc(width()/2-i*scaleX*100,height()-30-i*scaleY*100,i*200*scaleX,i*200*scaleY,(90-sectorSize.getDeg()/2)*16,sectorSize.getDeg()*16);
      QString str;
      str.setNum(i*range*1.0/5);
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

void SonarPlot::sonarPaletteChanged(int index){
    colormapSelector((ColormapType) index);
}

// set the current colormap
void SonarPlot::colormapSelector(ColormapType type) {
    heatMapGradient.clearGradient();

    switch (type) {
        case COLORMAP_JET:
            heatMapGradient.createJetMapGradient();
            break;

        case COLORMAP_HOT:
            heatMapGradient.createHotMapGradient();
            break;

        case COLORMAP_GRAYSCALE:
            heatMapGradient.createGrayscaleMapGradient();
            break;
        default:
            break;
    }

    colorMap.clear();
    try {
        float red, green, blue;
        for (int i = 0; i < 256; ++i) {
            heatMapGradient.getColorAtValue((1.0 / 255) * i, red, green, blue);
            colorMap.push_back(QColor(red * 255, green * 255, blue * 255));
        }
    } catch (const std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }
}

// generate the bearing table to correct wall curvatures
void SonarPlot::generateBearingTable(base::samples::SonarScan scan) {
    bearingTable.clear();

    // the wall curvature correction is only applied for Tritech Gemini
    if (scan.beamwidth_horizontal.getDeg() == 120.0 && scan.beamwidth_vertical.getDeg() == 20.0 && scan.number_of_beams == 256) {
        for (int i = 0; i < scan.number_of_beams; i++) {
            double rad = asin(((2 * i - scan.number_of_beams) * 1.0 / scan.number_of_beams) * 0.86602540);
            const base::Angle new_angle = base::Angle::fromRad(rad);
            bearingTable.push_back(new_angle);
        }
    }

    else {
        double interval = scan.beamwidth_horizontal.rad / scan.number_of_beams;
        for (int i = 0; i < scan.number_of_beams; i++) {
            double rad = interval * i + scan.start_bearing.rad;
            const base::Angle new_angle = base::Angle::fromRad(rad);
            bearingTable.push_back(new_angle);
        }
    }

}

