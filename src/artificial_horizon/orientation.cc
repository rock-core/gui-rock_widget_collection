#include <QtCore/QtPlugin>
#include <iostream>
#include "orientation.h"
#include "compass.h"
#include "artificialhorizon.h"
#include <QHBoxLayout>

OrientationView::OrientationView(QWidget* parent): MultiWidget(parent)
{
  layout = new QHBoxLayout;
	setLayout(layout);
	compass = new Compass();
	horizon = new ArtificialHorizon();
	layout->addWidget(compass);
	layout->addWidget(horizon);
}

OrientationView* OrientationView::newInstance()
{
    return new OrientationView();
}

void OrientationView::setHeading(double v)
{
	compass->setHeading(v);
}

void OrientationView::setPitch(double v)
{
	horizon->setPitchAngle(v);
}

void OrientationView::setRoll(double v)
{
	horizon->setRollAngle(v);
}

/*
OrientationView::~OrientationView()
{
	delete compass;
	delete needle;
}
*/
