#include "artificialhorizon.h"
#include <QPixmap>
#include <QPainter>
#include <math.h>
#include <QtCore/QtPlugin>
 #include <QtCore/QDir>
#include <iostream>

ArtificialHorizon::ArtificialHorizon(QWidget* parent):
QWidget(parent)
{
    setWindowTitle("ArtificialHorizon");
    setMinimumSize(160,160);
    rollangle  = 0;
    pitchangle = 0;
    setDataDir(":/");
}

void ArtificialHorizon::setDataDir(QString const& data_dir)
{
    stationary_o = QImage(data_dir + "stationary.svg");
    background_o = QPixmap(stationary_o.size());
    rotate_o     = QImage(data_dir + "rotate.svg");
    dial_o       = QImage(data_dir + "dial.svg");
		background = background_o;
	  stationary = stationary_o; 
	  rotate     = rotate_o;     
	  dial       = dial_o;       


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
    transform.translate(background.width()/2.0,background.height()/2.0);
    transform.rotate(-rollangle*180.0/M_PI);
    transform.translate(0,-pitchangle*180.0/M_PI*3.5);
    transform.translate(-background.width()/2.0,-background.height()/2.0);

    QPainter painter(this);
    painter.save(); // save the current painter settings before changing them
    painter.setClipRect(0,0,background.width(),background.height());

    painter.setTransform(transform);
    painter.translate(0,-background.height()/2.0-background.height()*0.0328947368421);
    painter.drawImage(0,0, rotate); // draw the stationary

    transform.translate(background.width()/2.0,background.height()/2.0);
    transform.translate(0,pitchangle*180.0/M_PI*3.5);
    transform.translate(-background.width()/2.0,-background.height()/2.0);
    painter.setTransform(transform);

    painter.drawImage(0,0, dial); // draw the stationary
    painter.restore(); // restore the previous painter settings
    painter.drawImage( point,stationary);
    painter.end();
}
		
void ArtificialHorizon::resizeEvent(QResizeEvent *event){
	QWidget::resizeEvent(event);

	background = background_o.scaled(size(),Qt::KeepAspectRatio);
  stationary = stationary_o.scaled(size(),Qt::KeepAspectRatio);
  dial       = dial_o.scaled(size(),Qt::KeepAspectRatio);      
  rotate     = rotate_o.scaledToWidth(dial.width()*1.01315789474);    
}

ArtificialHorizon::~ArtificialHorizon()
{
	
}
