#include "./SonarPlot.h"
#include <iostream>

using namespace std;
using namespace frame_helper;

SonarPlot::SonarPlot(QWidget *parent)
    : QFrame(parent)
    , range(5)
    , changedSize(true)
    , changedSectorScan(false)
    , isMultibeamSonar(true)
    , continuous(true)
    , enabledGrid(true)
    , enabledInterpolation(true)
{
    motorStep    = base::Angle::fromRad(0);
    lastDiffStep = base::Angle::fromRad(0);
    leftLimit    = base::Angle::fromRad(0);
    rightLimit   = base::Angle::fromRad(0);

    // apply default colormap
    applyColormap(COLORGRADIENT_JET);

      QPalette Pal(palette());
      Pal.setColor(QPalette::Background, Qt::blue);
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

    isMultibeamSonar = (sonar.beam_count > 1);

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

    // process singlebeam sonar data
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

    lastSonar = sonar;
    update();
}

// check if the motor step angle size is changed
bool SonarPlot::isMotorStepChanged(const base::Angle& bearing) {
    base::Angle diffStep = bearing - lastSonar.bearings[0];
    diffStep.rad = fabs(diffStep.rad);

    // if the sector scanning is enabled, the diffStep could be lower than motorStep when the bearing is closer to both borders
    if (!continuous && (fabs((leftLimit - bearing).rad) < motorStep.rad || fabs((rightLimit - bearing).rad) < motorStep.rad)) {
        lastDiffStep = diffStep;
        return false;
    }

    if (!motorStep.isApprox(diffStep) && lastDiffStep.isApprox(diffStep)) {
        motorStep = diffStep;
        numSteps = M_PI * 2 / motorStep.rad;
        lastDiffStep = diffStep;
        return true;
    }

    lastDiffStep = diffStep;
    return false;
}

// add current beam to accumulated scanning sonar data
void SonarPlot::addScanningData(const base::samples::Sonar& sonar) {
    int id_beam = round((numSteps - 1) * (sonar.bearings[0].rad + M_PI) / (2 * M_PI));
    memcpy(&sonarData[id_beam * sonar.bin_count], &sonar.bins[0], sonar.bin_count * sizeof(float));
}

void SonarPlot::paintEvent(QPaintEvent *)
{
    if (!transfer.size())
        return;

    if (isMultibeamSonar)
        sonarData = lastSonar.bins;

    // generate acoustic representation
    QImage img(width(), height(), QImage::Format_RGB888);
    img.fill(Qt::blue);
    cartesianImage(img);

    // draw sonar image
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawImage(0, 0, img);

    // draw overlay
    drawOverlay();
}

// generate the acoustic data representation in cartesian coordinates
void SonarPlot::cartesianImage(QImage& img) {
    // draw acoustic image without interpolation
    if (!enabledInterpolation) {
        for (size_t i = 0; i < transfer.size() && !changedSize; i++) {
            if (transfer[i] != -1) {
                QColor c = colorMap[round(sonarData[transfer[i]] * 255)];
                img.setPixel(i % width(), i / width(), qRgb(c.red(), c.green(), c.blue()));
            }
        }
    }
    // draw acoustic image using mean interpolation
    else {
        for (size_t i = 0; i < transfer.size() && !changedSize; i++) {
            if (transfer[i] != -1) {
                uint beamIdx = transfer[i] / lastSonar.bin_count;
                uint binIdx  = transfer[i] % lastSonar.bin_count;

                QColor c;
                if (((beamIdx < (lastSonar.beam_count - 1) || beamIdx < (numSteps - 1)) && (binIdx < (lastSonar.bin_count - 1)))) {
                    float s0 = sonarData[beamIdx * lastSonar.bin_count + binIdx];
                    float s1 = sonarData[beamIdx * lastSonar.bin_count + binIdx + 1];
                    float s2 = sonarData[(beamIdx + 1) * lastSonar.bin_count + binIdx];
                    float s3 = sonarData[(beamIdx + 1) * lastSonar.bin_count + binIdx + 1];
                    float v = (s0 + s1 + s2 + s3) / 4;
                    c = colorMap[v * 255];
                } else {
                    c = colorMap[round(sonarData[transfer[i]] * 255)];
                }
                img.setPixel(i % width(), i / width(), qRgb(c.red(), c.green(), c.blue()));
            }
        }
    }
}

void SonarPlot::resizeEvent ( QResizeEvent * event )
{
    // define sonar image center
    origin.rx() = width() * 0.5;
    origin.ry() = isMultibeamSonar ? (height() - 30) : (height() * 0.5);

    // define sonar drawing limits
    drawX = isMultibeamSonar ? (width()  * 0.45) : (width()  * 0.30);
    drawY = isMultibeamSonar ? (height() * 0.85) : (height() * 0.45);

    changedSize=true;
    QWidget::resizeEvent (event);
}

void SonarPlot::drawOverlay()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // draw color pallete
    for(size_t i = 0; i < 256; i++) {
        painter.setPen(QPen(colorMap[i]));
        painter.setBrush(QBrush(colorMap[i]));
        painter.drawRect(width() - 30,height() - 10 -i * 2, 20, 2);
    }

    if(!enabledGrid)
        return;

    // draw sonar grid
    painter.setBrush(QBrush());
    painter.setPen(QPen(Qt::white));

    // multibeam sonar
    if (isMultibeamSonar) {
        for(int i = 1; i <= 5; i++) {
            // arcs
            QRect rect(origin.x() - i * drawX / 5, origin.y() - i * drawY / 5, i * width() * 0.9 / 5, i * height() * 1.70 / 5);
            painter.drawArc(rect, (90 + lastSonar.bearings[0].getDeg()) * 16, (lastSonar.bearings[lastSonar.beam_count - 1] - lastSonar.bearings[0]).getDeg() * 16);
            QPoint point1(origin.x() + i * drawX * sin(lastSonar.bearings[lastSonar.beam_count - 1].getRad()) / 5, origin.y() - i * drawY * cos(lastSonar.bearings[lastSonar.beam_count - 1].getRad()) / 5);
            QString distance = QString::number(i * range * 1.0 / 5);
            painter.drawText(point1.x(), point1.y() + 20, distance);

            // bearings
            int beamIdx = (lastSonar.beam_count - 1) * (i - 1) / 4;
            base::Angle bearing = lastSonar.bearings[beamIdx];
            QPoint point2(origin.x() + drawX * sin(bearing.rad), origin.y() - drawY * cos(bearing.rad));
            painter.drawLine(origin, point2);
            painter.drawText(point2.x() - 10, point2.y() - 10, QString::number(bearing.getDeg(), 'f', 1));
        }
    }

    // scanning sonar
    else {
        painter.drawLine(QPoint(origin.rx(), origin.ry() - drawY), QPoint(origin.rx(), origin.ry() + drawY));
        painter.drawLine(QPoint(origin.rx() - drawX, origin.ry()), QPoint(origin.rx() + drawX, origin.ry()));

        // distances
        for (int i = 1; i <= 5; ++i) {
            QPoint point(i * drawX / 5, i * drawY / 5);
            painter.drawEllipse(origin, point.x(), point.y());
            QString distance = QString::number(i * range * 1.0 / 5);
            painter.drawText(origin.x() + point.x() + 2, origin.y() - 5, distance);
        }

        // scanning reader
        QString str = QString::number(lastSonar.bearings[0].getDeg(), 'f', 1);
        QPoint point(origin.x() - drawX * sin(lastSonar.bearings[0].rad), origin.y() - drawY * cos(lastSonar.bearings[0].rad));
        painter.setPen(QPen(Qt::green));
        painter.drawLine(origin, point);
        painter.drawText(point.x() - 10, point.y() - 10, str);

        // scanning angle
        if (!continuous) {
            QPoint point1(origin.x() - drawX * sin(leftLimit.rad), origin.y() - drawY * cos(leftLimit.rad));
            QPoint point2(origin.x() - drawX * sin(rightLimit.rad), origin.y() - drawY * cos(rightLimit.rad));
            painter.drawLine(origin, point1);
            painter.drawLine(origin, point2);
        }
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

// enable/disable the sonar grid
void SonarPlot::gridChanged(bool value){
    enabledGrid = value;
}

// enable/disable the interpolation
void SonarPlot::interpolationChanged(bool value){
    enabledInterpolation = value;
}

// update the sector scan (for scanning sonars)
void SonarPlot::setSectorScan(bool continuous, base::Angle leftLimit, base::Angle rightLimit){

    // if the parameters changes, the screen will be clean
    if (this->continuous != continuous
            || ((this->leftLimit.rad != leftLimit.rad || this->rightLimit.rad != rightLimit.rad) && !continuous))
        changedSectorScan = true;

    this->continuous = continuous;
    this->leftLimit = leftLimit;
    this->rightLimit = rightLimit;
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
    // initialize transfer table
    transfer.assign(width() * height(), -1);

    // set the origin
    origin.setY(height() - 30);

    // define drawing limits
    drawX = width() * 0.45;
    drawY = height() * 0.85;

    // check pixels
    for (int j = 0; j < height(); j++) {
        int beamIdx = 0;
        for (int i = 0; i < width(); i++) {
            QPoint point(i - origin.x(), j - origin.y());
            point.rx() *= (float) sonar.bin_count / drawX;
            point.ry() *= (float) sonar.bin_count / drawY;

            double radius = sqrt(point.x() * point.x() + point.y() * point.y());
            double theta = atan2(point.x(), -point.y());

            // pixels out of sonar image
            if (theta < sonar.bearings[0].rad || theta > sonar.bearings[sonar.beam_count - 1].rad || radius > sonar.bin_count || !radius || j > origin.y())
                continue;

            // pixels in the sonar image
            else {
                while (theta < sonar.bearings[beamIdx].rad || theta >= sonar.bearings[beamIdx + 1].rad)
                    beamIdx++;
                transfer[j * width() + i] = beamIdx * sonar.bin_count + radius;
            }
        }
    }
}

// set the transfer vector between image pixels and sonar data (for scanning sonars)
void SonarPlot::generateScanningTransferTable(const base::samples::Sonar& sonar) {
    // check motor step value
    if (!motorStep.rad)
        return;

    // initialize transfer table
    transfer.assign(width() * height(), -1);

    // set the origin
    origin.setY(height() / 2);

    // define drawing limits
    drawX = width()  * 0.30;
    drawY = height() * 0.45;

    // check pixels
    for (int j = 0; j < height(); j++) {
        for (int i = 0; i < width(); i++) {
            QPoint point(i - origin.x(), j - origin.y());
            point.rx() *= sonar.bin_count / drawX;
            point.ry() *= (float) sonar.bin_count / drawY;

            double radius = sqrt(point.x() * point.x() + point.y() * point.y());

            // pixels out of sonar image
            if (radius > sonar.bin_count || !radius)
                continue;

            // pixels in the sonar image
            else {
                double theta = atan2(-point.x(), -point.y());
                int beamIdx = round((numSteps - 1) * (theta + M_PI) / (2 * M_PI));
                transfer[j * width() + i] = beamIdx * sonar.bin_count + radius;
            }
        }
    }
}
