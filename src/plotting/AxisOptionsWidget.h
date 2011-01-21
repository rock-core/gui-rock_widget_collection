#ifndef AXISOPTIONSWIDGET_H
#define	AXISOPTIONSWIDGET_H

#include <QWidget>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>

#include <iostream>

#include "DataManager.h"

class AxisOptionsWidget : public QWidget
{

    Q_OBJECT

public:
    AxisOptionsWidget();
    void initializeLayout();
    void updateDataManager();

public slots:
    void autoscaleSelected();
    void fixedSizeSelected();

protected:
    QGridLayout layout;
    QRadioButton autoscaleBox;
    QRadioButton autoscrollBox;
    QRadioButton fixedBox;
    QDoubleSpinBox minXBox;
    QDoubleSpinBox maxXBox;
    QDoubleSpinBox minYBox;
    QDoubleSpinBox maxYBox;
    QButtonGroup buttonGroup;
    QLabel scrollLabel;
    QLabel xBoundary;
    QLabel yBoundary;
    DataManager* dataManager;
private:

};

#endif	/* AXISOPTIONSWIDGET_H */
