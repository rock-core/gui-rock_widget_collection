#include "compass.h"
#include <QtCore/QtPlugin>
#include <iostream>
#include <qwt_compass.h>
#include <qwt_compass_rose.h>
#include <qwt_dial_needle.h>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <math.h>


Compass::Compass(QWidget* parent): MultiWidget(parent)
{
  layout2 = new QHBoxLayout;
  layout = new QVBoxLayout;
	setLayout(layout);
	compass = new QwtCompass();
	layout->addWidget(compass);
	compass->setScaleOptions(QwtDial::ScaleTicks | QwtDial::ScaleLabel);
	needle = new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::TriangleStyle, Qt::white, Qt::red);
	compass->setNeedle(needle);
	label = new QLabel("Heading:");
	number = new QLCDNumber();
	label->setMaximumSize(100,50);
	number->setMaximumSize(100,50);
	layout2->addWidget(label);
	layout2->addWidget(number);
	layout->addLayout(layout2);
}

Compass* Compass::newInstance()
{
    return new Compass();
}

void Compass::setHeading(double v)
{
	compass->setValue(-v/M_PI*180.0);
	number->display(v/M_PI*180.0);
}

/*
Compass::~Compass()
{
	delete compass;
	delete needle;
}
*/
