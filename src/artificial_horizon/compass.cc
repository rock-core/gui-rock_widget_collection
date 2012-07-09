#include "compass.h"
#include <QtCore/QtPlugin>
#include <iostream>
#include <qwt_compass.h>
#include <qwt_compass_rose.h>
#include <qwt_dial_needle.h>
#include <QHBoxLayout>
#include <QLabel>
#include <math.h>


Compass::Compass(QWidget* parent): MultiWidget(parent)
{
  // add a layout
  layout = new QVBoxLayout;
  setLayout(layout);

  // Label
  number = new QLabel();
  //number->setMaximumSize(100,50);
  number->setAlignment(Qt::AlignHCenter);
  number->setText("NaN / NaN");
  layout->addWidget(number);

  // compass 
  compass = new QwtCompass();
  layout->addWidget(compass);
  compass->setScaleOptions(QwtDial::ScaleTicks | QwtDial::ScaleLabel);
  needle = new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::TriangleStyle, Qt::white, Qt::red);
  compass->setNeedle(needle);
}

Compass* Compass::newInstance()
{
    return new Compass();
}

void Compass::setHeading(double v)
{ 
  QString heading_text = QString("%1 / %2").arg(v).arg(v/M_PI*180.0);

  compass->setValue(-v/M_PI*180.0);
  number->setText(heading_text); 
}

/*
Compass::~Compass()
{
	delete compass;
	delete needle;
}
*/
