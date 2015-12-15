#include "./SonarPlot.h"
#include <iostream>

using namespace std;
using namespace frame_helper;

SonarPlot::SonarPlot(QWidget *parent)
    : QFrame(parent), changedSize(true),scaleX(1),scaleY(1),range(5)
{
    // apply default colormap
    applyColormap(COLORGRADIENT_JET);

      QPalette Pal(palette());
      Pal.setColor(QPalette::Background, QColor(0,0,255));
      setAutoFillBackground(true);
      setPalette(Pal);
}

SonarPlot::~SonarPlot()
{
}

void SonarPlot::setData(const base::samples::Sonar sonar)
{
    if (!sonar.bin_count)
        return;

    if(changedSize
        || !(sonar.bin_count == lastSonar.bin_count)
        || !(sonar.beam_count  == lastSonar.beam_count)
        || !(sonar.bearings[0]  == lastSonar.bearings[0])
        || !((sonar.bearings[1] - sonar.bearings[0]) == (lastSonar.bearings[1] - lastSonar.bearings[0]))) {

        // set the transfer vector between image pixels and sonar data
        generateTransferTable(sonar);

        changedSize = false;
    }
    lastSonar = sonar;
    update();
}

void SonarPlot::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // draw sonar image
    QImage img(width(), height(), QImage::Format_RGB888);
    img.fill(QColor(0, 0, 255));

    for (unsigned int i = 0; i < transfer.size() && !changedSize; ++i) {
        if (transfer[i] != -1) {
            QColor c = colorMap[round(lastSonar.bins[transfer[i]] * 255)];
            img.setPixel(i / origin.y(), i % origin.y(), qRgb(c.red(), c.green(), c.blue()));
        }
    }

    painter.drawImage(0, 0, img);

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

    // multibeam sonar
    if (lastSonar.beam_count) {

        // draw grid
        base::Angle sectorSize = base::Angle::fromRad((lastSonar.beam_width.rad / lastSonar.beam_count) * (lastSonar.beam_count - 1));

        for(int i=1;i<=5;i++){
            painter.drawArc(origin.rx() - i * scaleX * 100, origin.ry() - i * scaleY * 100, i * 200 * scaleX, i * 200 * scaleY, (90 - sectorSize.getDeg() / 2) * 16, sectorSize.getDeg() * 16);
            QString str;
            str.setNum(i*range*1.0/5);
            int x = width()/2+i*100*scaleX*sin(sectorSize.getDeg()*3.1416/360);
            int y = height()-i*100*scaleY*cos(sectorSize.getDeg()*3.1416/360);
            painter.drawText(x,y-5,str);

            base::Angle ang = lastSonar.bearings[((lastSonar.beam_count - 1) * 1.0 / 4) * (i-1)];
            QPoint point;
            point.rx() = origin.rx() + BINS_REF_SIZE * sin(ang.getRad()) * scaleX;
            point.ry() = origin.ry() - BINS_REF_SIZE * cos(ang.getRad()) * scaleY;
            painter.drawLine(origin, point);
            str.setNum(ang.getDeg());
            x = point.x();
            if (x < width() / 2)
                x -= 15;
            if (x == width() / 2)
                x -= 5;
            y = point.y()-10;
            painter.drawText(x,y,str);
        }
    }

    // draw color pallete
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
    applyColormap((ColorGradientType) index);
}

// applies a color gradient
void SonarPlot::applyColormap(ColorGradientType type){

    heatMapGradient.colormapSelector(type);

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

// set the transfer vector between image pixels and sonar data
void SonarPlot::generateTransferTable(const base::samples::Sonar& sonar) {

    transfer.clear();

    // check pixels
    for (int i = 0; i< width();i++){
        for (int j = 0; j< origin.y();j++){

            QPoint point(i - origin.x(), j - origin.y());
            point.rx() /= scaleX * BINS_REF_SIZE / sonar.bin_count;
            point.ry() /= scaleY * BINS_REF_SIZE / sonar.bin_count;

            double radius = sqrt(point.x() * point.x() + point.y() * point.y());
            double angle = asin(point.x() * 1.0 / radius);
            base::Angle theta = base::Angle::fromRad(angle);

            // pixels out of sonar image
            if (theta.rad < sonar.bearings[0].rad || theta.rad > sonar.bearings[sonar.beam_count - 1].rad || radius > sonar.bin_count || !radius)
                transfer.push_back(-1);

            // pixels in the sonar image
            else {
                for (unsigned int k = 0; k < sonar.beam_count - 1; k++) {
                    if (theta.rad >= sonar.bearings[k].rad && theta.rad < sonar.bearings[k + 1].rad) {
                        transfer.push_back(k * sonar.bin_count + radius);
                        break;
                    }
                }
            }
        }
    }
}
