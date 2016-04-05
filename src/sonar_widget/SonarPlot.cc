#include "./SonarPlot.h"
#include <iostream>

using namespace std;
using namespace frame_helper;

SonarPlot::SonarPlot(QWidget *parent)
    : QFrame(parent), scaleX(1), scaleY(1), range(5), changedSize(true), changedSectorScan(false), isMultibeamSonar(true), continuous(true)
{
    motorStep.rad = 0;
    lastDiffStep.rad = 0;
    leftLimit.rad = 0;
    rightLimit.rad = 0;

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

// process sonar data
void SonarPlot::setData(const base::samples::Sonar& sonar)
{
    if (!sonar.beam_count || !sonar.bin_count)
        return;

    sonar.beam_count > 1 ? isMultibeamSonar = true : isMultibeamSonar = false;

    // process multibeam sonar data
    if (isMultibeamSonar) {
        if(changedSize
               || sonar.bin_count != lastSonar.bin_count
               || sonar.beam_count  != lastSonar.beam_count
               || !(sonar.bearings[0]  == lastSonar.bearings[0])
               || !((sonar.bearings[1] - sonar.bearings[0]) == (lastSonar.bearings[1] - lastSonar.bearings[0]))) {

            // set the transfer vector between image pixels and sonar data
            generateMultibeamTransferTable(sonar);

            changedSize = false;
        }
    }

    // process scanning sonar data
    else {

        // check if the motor step size is changed
        bool changedMotorStep = lastSonar.beam_count && isMotorStepChanged(sonar.bearings[0]);

        if ((changedSize
                || changedMotorStep
                || changedSectorScan
                || sonar.bin_count != lastSonar.bin_count) && lastSonar.beam_count && motorStep.rad) {

            // set the transfer vector between image pixels and sonar data
            generateScanningTransferTable(sonar);

            // if the number of bins, the motor step or the sector scan changes, the accumulated sonar data will be reseted
            if (sonar.bin_count != lastSonar.bin_count || changedMotorStep || changedSectorScan)
                sonarData.assign(numSteps * sonar.bin_count, 0.0);

            changedSize = false;
            changedSectorScan = false;
        }

        // add current beam to accumulated scanning sonar data
        if (sonarData.size())
            addScanningData(sonar);
    }

    beforeLastSonar = lastSonar;
    lastSonar = sonar;
    update();
}

// check is the motor step angle size is changed
bool SonarPlot::isMotorStepChanged(const base::Angle& bearing) {
    base::Angle diffStep = bearing - lastSonar.bearings[0];
    diffStep.rad = fabs(diffStep.rad);

    if (!motorStep.isApprox(diffStep) && lastDiffStep.isApprox(diffStep) && !bearing.isApprox(beforeLastSonar.bearings[0])) {
        motorStep = diffStep;
        numSteps = round(M_PI * 2 / motorStep.rad);
        lastDiffStep = diffStep;
        return true;
    }

    lastDiffStep = diffStep;
    return false;
}

// add current beam to accumulated scanning sonar data
void SonarPlot::addScanningData(const base::samples::Sonar& sonar) {
    int id_beam = (numSteps / 2) + sonar.bearings[0].rad / motorStep.rad;
    if (id_beam == numSteps) id_beam--;

    for (unsigned int i = 0; i < sonar.bin_count; ++i)
        sonarData[id_beam * sonar.bin_count + i] = sonar.bins[i];
}

void SonarPlot::paintEvent(QPaintEvent *)
{
    if (!transfer.size())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // draw sonar image
    QImage img(width(), height(), QImage::Format_RGB888);
    img.fill(QColor(0, 0, 255));

    if (isMultibeamSonar)
        sonarData = lastSonar.bins;

    for (unsigned int i = 0; i < transfer.size() && !changedSize; ++i) {
        if (transfer[i] != -1) {
            QColor c = colorMap[round(sonarData[transfer[i]] * 255)];
            img.setPixel(i % width(), i / width(), qRgb(c.red(), c.green(), c.blue()));
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
    isMultibeamSonar ? origin.setY(height() - 30) : origin.setY(height() / 2);
    changedSize=true;
    QWidget::resizeEvent (event);
}

void SonarPlot::drawOverlay()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::white));

    // multibeam sonar
    if (isMultibeamSonar) {

        base::Angle sectorSize = base::Angle::fromRad((lastSonar.beam_width.rad / lastSonar.beam_count) * (lastSonar.beam_count - 1));

        for(int i=1;i<=5;i++){
            painter.drawArc(origin.x() - i * scaleX * 100, origin.y() - i * scaleY * 100, i * 200 * scaleX, i * 200 * scaleY, (90 - sectorSize.getDeg() / 2) * 16, sectorSize.getDeg() * 16);
            QString str = QString::number(i * range * 1.0 / 5);
            int x = origin.x() + i * 100 * scaleX * sin(sectorSize.rad / 2);
            int y = height() - i * 100 * scaleY * cos(sectorSize.rad / 2);
            painter.drawText(x,y-5,str);

            base::Angle ang = lastSonar.bearings[((lastSonar.beam_count - 1) * 1.0 / 4) * (i-1)];
            QPoint point(origin.rx() + BINS_REF_SIZE * sin(ang.rad) * scaleX, origin.ry() - BINS_REF_SIZE * cos(ang.rad) * scaleY);
            painter.drawLine(origin, point);
            str = QString::number(ang.getDeg(), 'f', 1);
            painter.drawText(point.x() - 10, point.y() - 10, str);
        }
    }

    // scanning sonar
    else {
        double offsetX = BINS_REF_SIZE * 0.67 * scaleX;
        double offsetY = BINS_REF_SIZE * 0.55 * scaleY;
        painter.drawLine(QPoint(origin.rx(), origin.ry() - offsetY), QPoint(origin.rx(), origin.ry() + offsetY));
        painter.drawLine(QPoint(origin.rx() - offsetX, origin.ry()), QPoint(origin.rx() + offsetX, origin.ry()));

        for (int i = 1; i <= 5; ++i) {
            int x = i * offsetX / 5;
            int y = i * offsetY / 5;
            painter.drawEllipse(origin, x, y);
            QString str_radius = QString::number(i * range * 1.0 / 5);
            painter.drawText(origin.rx() + x + 2, origin.ry() - 5, str_radius);
        }

        QString str = QString::number(lastSonar.bearings[0].getDeg(), 'f', 1);
        QPoint point(origin.rx() - offsetX * sin(lastSonar.bearings[0].rad), origin.ry() - offsetY * cos(lastSonar.bearings[0].rad));
        painter.setPen(QPen(Qt::green));
        painter.drawLine(origin, point);
        painter.drawText(point.x() - 10, point.y() - 10, str);

        if (!continuous) {
            QPoint point1(origin.rx() - offsetX * sin(leftLimit.rad), origin.ry() - offsetY * cos(leftLimit.rad));
            QPoint point2(origin.rx() - offsetX * sin(rightLimit.rad), origin.ry() - offsetY * cos(rightLimit.rad));
            painter.drawLine(origin, point1);
            painter.drawLine(origin, point2);
        }
    }

    painter.setPen(QPen(Qt::white));
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
}

// update the current palette
void SonarPlot::sonarPaletteChanged(int index){
    applyColormap((ColorGradientType) index);
}

// update the sector scan (for scanning sonars)
void SonarPlot::setSectorScan(bool continuous, base::Angle left, base::Angle right){
    this->continuous = continuous;
    this->leftLimit = left;
    this->rightLimit = right;

    if (!continuous)
        changedSectorScan = true;
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

    // set the origin
    origin.setY(height() - 30);

    // check pixels
    for (int j = 0; j < height(); j++) {
        for (int i = 0; i < width(); i++) {

            QPoint point(i - origin.x(), j - origin.y());
            point.rx() /= scaleX * BINS_REF_SIZE / sonar.bin_count;
            point.ry() /= scaleY * BINS_REF_SIZE / sonar.bin_count;

            double radius = sqrt(point.x() * point.x() + point.y() * point.y());
            double angle = atan2(point.x(), -point.y());

            // pixels out of sonar image
            if (angle < sonar.bearings[0].rad || angle > sonar.bearings[sonar.beam_count - 1].rad || radius > sonar.bin_count || !radius || j > origin.y())
                transfer.push_back(-1);

            // pixels in the sonar image
            else {
                for (unsigned int k = 0; k < sonar.beam_count - 1; k++) {
                    if (angle >= sonar.bearings[k].rad && angle < sonar.bearings[k + 1].rad) {
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

    // check motor step value
    if (!motorStep.rad)
        return;

    // set the origin
    origin.setY(height() / 2);

    // check pixels
    for (int j = 0; j < height(); j++) {
        for (int i = 0; i < width(); i++) {

            QPoint point(i - origin.x(), j - origin.y());
            point.rx() /= scaleX * BINS_REF_SIZE * 0.67 / sonar.bin_count;
            point.ry() /= scaleY * BINS_REF_SIZE * 0.55 / sonar.bin_count;

            double radius = sqrt(point.x() * point.x() + point.y() * point.y());

            // pixels out of sonar image
            if (radius > sonar.bin_count || !radius)
                transfer.push_back(-1);

            // pixels in the sonar image
            else {
                double angle = atan2(-point.x(), -point.y());
                int idBeam = (numSteps / 2) + angle / motorStep.rad;
                if (idBeam == numSteps) idBeam--;
                transfer.push_back(idBeam * sonar.bin_count + radius);
            }
        }
    }
}
