#include <QtGui/qwidget.h>
#include <QtGui/qlayout.h>
#include <QtGui/qspinbox.h>

#include "AxisOptionsWidget.h"
#include "DataManager.h"

AxisOptionsWidget::AxisOptionsWidget() : layout(this), autoscaleBox(tr("Autoscale")),
        autoscrollBox(tr("Autoscroll")), fixedBox(tr("Fixed")), scrollLabel(tr("Scroll Behaviour")),
        xBoundary(tr("X Boundary")), yBoundary(tr("Y Boundary"))
{

    minXBox.setMaximum(100000);
    maxXBox.setMaximum(100000);
    minYBox.setMaximum(100000);
    maxYBox.setMaximum(100000);
    minXBox.setMinimum(-100000);
    maxXBox.setMinimum(-100000);
    minYBox.setMinimum(-100000);
    maxYBox.setMinimum(-100000);
    dataManager = DataManager::getInstance();
    connect(&fixedBox, SIGNAL(clicked()), this, SLOT(fixedSizeSelected()));
    connect(&autoscrollBox, SIGNAL(clicked()), this, SLOT(fixedSizeSelected()));
    connect(&autoscaleBox, SIGNAL(clicked()), this, SLOT(autoscaleSelected()));
}


void AxisOptionsWidget::fixedSizeSelected()
{
    minXBox.setEnabled(true);
    maxXBox.setEnabled(true);
    minYBox.setEnabled(true);
    maxYBox.setEnabled(true);
}

void AxisOptionsWidget::autoscaleSelected()
{
    minXBox.setEnabled(false);
    maxXBox.setEnabled(false);
    minYBox.setEnabled(false);
    maxYBox.setEnabled(false);
}

void AxisOptionsWidget::updateDataManager()
{
    dataManager->setMaxX(maxXBox.value());
    dataManager->setMinX(minXBox.value());
    dataManager->setMaxY(maxYBox.value());
    dataManager->setMinY(minYBox.value());
    dataManager->setAutoscaling(autoscaleBox.isChecked());
    dataManager->setAutoscrolling(autoscrollBox.isChecked());
    dataManager->setIsFixedSize(fixedBox.isChecked());
}

void AxisOptionsWidget::initializeLayout()
{
    buttonGroup.addButton(&fixedBox);
    buttonGroup.addButton(&autoscrollBox);
    buttonGroup.addButton(&autoscaleBox);
    if(dataManager->isAutoscrolling())
    {
        autoscrollBox.setChecked(true);
        fixedSizeSelected();
    }
    else if(dataManager->isFixedSize())
    {
        fixedBox.setChecked(true);
        fixedSizeSelected();
    }
    else if(dataManager->isAutoscaling())
    {
        autoscaleBox.setChecked(true);
        autoscaleSelected();
    }
    minXBox.setValue(dataManager->getMinX());
    maxXBox.setValue(dataManager->getMaxX());
    minYBox.setValue(dataManager->getMinY());
    maxYBox.setValue(dataManager->getMaxY());
    layout.setAlignment(Qt::AlignTop);
    layout.setMargin(5);
    layout.setSpacing(5);
    layout.addWidget(&scrollLabel, 0, 0);
    layout.addWidget(&fixedBox, 1, 0);
    layout.addWidget(&autoscaleBox, 1, 1);
    layout.addWidget(&autoscrollBox, 1, 2);
    layout.addWidget(&xBoundary, 2, 0);
    layout.addWidget(&minXBox, 2, 1);
    layout.addWidget(&maxXBox, 2, 2);
    layout.addWidget(&yBoundary, 3, 0);
    layout.addWidget(&minYBox, 3, 1);
    layout.addWidget(&maxYBox, 3, 2);
    setLayout(&layout);
}

