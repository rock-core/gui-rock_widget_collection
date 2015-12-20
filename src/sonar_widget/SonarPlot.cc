#include "./SonarPlot.h"
#include <iostream>

using namespace std;
using namespace frame_helper;

SonarPlot::SonarPlot(QWidget *parent)
    : QFrame(parent), changedSize(true),scaleX(1),scaleY(1),range(5)
{

    // apply default colormap
    applyColormap(COLORGRADIENT_JET);

    numDiv = 640;
    motorStep = base::Angle::fromRad(M_PI * 2 / numDiv);

      QPalette Pal(palette());
      Pal.setColor(QPalette::Background, QColor(0,0,255));
      setAutoFillBackground(true);
      setPalette(Pal);
}

SonarPlot::~SonarPlot()
{
}

void SonarPlot::setMultibeamData(const base::samples::Sonar& sonar)
{

    if(changedSize
           || !(sonar.bin_count == lastSonar.bin_count)
           || !(sonar.beam_count  == lastSonar.beam_count)
           || !(sonar.bearings[0]  == lastSonar.bearings[0])
           || !((sonar.bearings[1] - sonar.bearings[0]) == (lastSonar.bearings[1] - lastSonar.bearings[0]))) {

        // clean the sonar image
        img = QImage(width(), height(), QImage::Format_RGB888);
        img.fill(Qt::blue);

        // check the origin
        if (!(origin.y() == (height() - 30)))
            origin.setY(height() - 30);

        // set the transfer vector between image pixels and sonar data
        generateMultibeamTransferTable(sonar);

        changedSize = false;
    }

    lastSonar = sonar;
    update();
}

void SonarPlot::setScanningData(const base::samples::Sonar& sonar)
{
    if(changedSize
           || !(sonar.bin_count == lastSonar.bin_count)
           || !(sonar.beam_count  == lastSonar.beam_count)) {

        // clean the sonar image
        img = QImage(width(), height(), QImage::Format_RGB888);
        img.fill(Qt::blue);

        // set the transfer vector between image pixels and sonar data
        generateScanningTransferTable(sonar);

        // if number of bins changes, then the accumulated sonar data will be reseted
        if (!(sonar.bin_count == lastSonar.bin_count))
            scanData.assign(numDiv * sonar.bin_count, 0.0);

        changedSize = false;
    }

    lastSonar = sonar;
    update();
}

void SonarPlot::paintEvent(QPaintEvent *)
{
    if (!lastSonar.beam_count)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // multibeam sonar
    if (lastSonar.beam_count > 1) {
        for (unsigned int i = 0; i < transfer.size() && !changedSize; ++i) {
            if (transfer[i] != -1) {
                QColor c = colorMap[round(lastSonar.bins[transfer[i]] * 255)];
                img.setPixel(i / origin.y(), i % origin.y(), qRgb(c.red(), c.green(), c.blue()));
            }
        }
    }

    // scanning sonar
    else {
        int id_beam = lastSonar.bearings[0].rad / motorStep.rad;
        if (id_beam < 0)
            id_beam = (numDiv - 1) + id_beam;

        for (int i = 0; i < lastSonar.bin_count; ++i)
            scanData[id_beam * lastSonar.bin_count + i] = lastSonar.bins[i];

        for (unsigned int i = 0; i < transfer.size() && !changedSize; ++i) {
            if (transfer[i] != -1) {
                QColor c = colorMap[round(scanData[transfer[i]] * 255)];
                img.setPixel(i / height(), i % height(), qRgb(c.red(), c.green(), c.blue()));
            }
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
    if (lastSonar.beam_count > 1)
        origin.setY(height() - 30);
    else
        origin.setY(height() / 2);
    changedSize=true;
    QWidget::resizeEvent (event);
}

void SonarPlot::drawOverlay()
{
    if (!lastSonar.beam_count)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::white));

    // multibeam sonar
    if (lastSonar.beam_count > 1) {

        base::Angle sectorSize = base::Angle::fromRad((lastSonar.beam_width.rad / lastSonar.beam_count) * (lastSonar.beam_count - 1));

        for(int i=1;i<=5;i++){
            painter.drawArc(origin.x() - i * scaleX * 100, origin.y() - i * scaleY * 100, i * 200 * scaleX, i * 200 * scaleY, (90 - sectorSize.getDeg() / 2) * 16, sectorSize.getDeg() * 16);
            QString str = QString::number(i * range * 1.0 / 5);
            int x = origin.x() + i * 100 * scaleX * sin(sectorSize.getDeg() * 3.1416 / 360);
            int y = height()-i*100*scaleY*cos(sectorSize.getDeg()*3.1416/360);
            painter.drawText(x,y-5,str);

            base::Angle ang = lastSonar.bearings[((lastSonar.beam_count - 1) * 1.0 / 4) * (i-1)];
            QPoint point;
            point.rx() = origin.rx() + BINS_REF_SIZE * sin(ang.rad) * scaleX;
            point.ry() = origin.ry() - BINS_REF_SIZE * cos(ang.rad) * scaleY;
            painter.drawLine(origin, point);
            str.setNum(ang.getDeg());
            painter.drawText(point.x() - 10, point.y() - 10, str);
        }
    }

    // scanning sonar
    else {
        base::Angle ang = lastSonar.bearings[0];
        double offsetX = BINS_REF_SIZE * 0.67 * scaleX;
        double offsetY = BINS_REF_SIZE * 0.55 * scaleY;
        QPoint point;
        point.rx() = origin.rx() + offsetX * sin(ang.getRad());
        point.ry() = origin.ry() - offsetY * cos(ang.getRad());
        painter.drawLine(origin, point);
        painter.drawLine(QPoint(origin.rx(), origin.ry() - offsetY), QPoint(origin.rx(), origin.ry() + offsetY));
        painter.drawLine(QPoint(origin.rx() - offsetX, origin.ry()), QPoint(origin.rx() + offsetX, origin.ry()));

        for (int i = 1; i <= 5; ++i) {
            int x = i * offsetX / 5;
            int y = i * offsetY / 5;
            painter.drawEllipse(origin, x, y);
            QString str_radius = QString::number(i * range * 1.0 / 5);
            painter.drawText(origin.rx() + x + 2, origin.ry() - 5, str_radius);
        }

        QString str_deg = QString::number(ang.getDeg());
        painter.drawText(point.x() - 10, point.y() - 10, str_deg);
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

// set the transfer vector between image pixels and sonar data (for multibeam sonars)
void SonarPlot::generateMultibeamTransferTable(const base::samples::Sonar& sonar) {

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

// set the transfer vector between image pixels and sonar data (for scanning sonars)
void SonarPlot::generateScanningTransferTable(const base::samples::Sonar& sonar) {

    transfer.clear();

    // check pixels
    for (int i = 0; i < width(); i++) {
        for (int j = 0; j < height(); j++) {

            QPoint point(i - origin.x(), j - origin.y());
            point.rx() /= scaleX * BINS_REF_SIZE * 0.67 / sonar.bin_count;
            point.ry() /= scaleY * BINS_REF_SIZE * 0.55 / sonar.bin_count;

            double radius = sqrt(point.x() * point.x() + point.y() * point.y());
            double angle = asin(point.x() * 1.0 / radius);
            base::Angle theta = base::Angle::fromRad(angle);

            // pixels out of sonar image
            if (radius > sonar.bin_count || !radius)
                transfer.push_back(-1);

            // pixels in the sonar image
            else {
                int idBeam = theta.rad / motorStep.rad;

                // top image
                if (j <= height() / 2) {
                    if (idBeam < 0) {
                        idBeam = (numDiv - 1) + idBeam;
                    }
                }

                // bottom image
                else
                    idBeam = numDiv / 2 - idBeam;

                transfer.push_back(idBeam * sonar.bin_count + radius);
            }
        }
    }
}
