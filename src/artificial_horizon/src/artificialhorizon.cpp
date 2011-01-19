#include "artificialhorizon.h"
#include <QPixmap>
#include <QPainter>
#include <math.h>
#include <QtCore/QtPlugin>

Q_EXPORT_PLUGIN2(ArtificialHorizon, ArtificialHorizon);

ArtificialHorizon::ArtificialHorizon()
{
    rollangle  = 0;
    pitchangle = 0;
}

void ArtificialHorizon::setDataDir(QString const& data_dir)
{
    background = QPixmap(data_dir + "background.svg");
    stationary = QImage(data_dir + "stationary.svg");
    rotate     = QImage(data_dir + "rotate.svg");
    dial       = QImage(data_dir + "dial.svg");
}

ArtificialHorizon* ArtificialHorizon::newInstance()
{
    return new ArtificialHorizon();
}

void ArtificialHorizon::setRollAngle(double myvalue)
{
    if (myvalue!=rollangle)
    {
        rollangle = myvalue;
        this->update();
    }
}
void ArtificialHorizon::setPitchAngle(double myvalue)
{
    if (myvalue!=pitchangle)
    {
        pitchangle = myvalue;
        this->update();
    }
}
void ArtificialHorizon::paintEvent(QPaintEvent*)
{
    QPoint point=QPoint(0,0);
    QTransform transform;
    transform.translate(background.width()/2,background.height()/2);
    transform.rotate(rollangle*180.0/M_PI);
    transform.translate(0,pitchangle*180.0/M_PI*3.5);
    transform.translate(-background.width()/2,-background.height()/2);

    QPainter painter(this);
    painter.save(); // save the current painter settings before changing them
    painter.setClipRect(0,0,300,290);

    painter.setTransform(transform);
    painter.translate(0,-background.height()/2-10);
    painter.drawImage(0,0, rotate); // draw the stationary

    transform.translate(background.width()/2,background.height()/2);
    transform.translate(0,-pitchangle*180.0/M_PI*3.5);
    transform.translate(-background.width()/2,-background.height()/2);
    painter.setTransform(transform);

    painter.drawImage(0,0, dial); // draw the stationary
    painter.restore(); // restore the previous painter settings
    painter.drawImage( point,stationary);
    painter.end();
}

ArtificialHorizon::~ArtificialHorizon()
{

}
