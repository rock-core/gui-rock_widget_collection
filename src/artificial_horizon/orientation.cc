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
  setMinimumWidth(400);
  setMinimumHeight(100);
  
  // minimum size displaying both widgets completely
  //setMinimumSize(compass->minimumSize().expandedTo(horizon->minimumSize()));
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
