/* 
 * File:   CurveOptionWidget.cc
 * Author: blueck
 * 
 * Created on 21. Oktober 2010, 10:37
 */

#include "CurveOptionWidget.h"

CurveOptionWidget::CurveOptionWidget()
{
    
}

void CurveOptionWidget::updateExistingCurves()
{
    for(int i=0;i<curves.size();i++)
    {
        QwtPlotCurve* curve = curves[i];
        if(curve != NULL)
        {
            QPen pen = curve->pen();
            pen.setColor(buttons[i]->getDisplayColor());
            pen.setWidth(spinBoxes[i]->value());
            curve->setPen(pen);
            QwtPlotCurve::CurveStyle style = (QwtPlotCurve::CurveStyle)boxes[i]->currentIndex();
            curve->setStyle(style);
        }
    }
}

void CurveOptionWidget::initializeLayout(std::vector<QwtPlotCurve*> curves)
{
    this->curves = curves;
    buttons.resize(curves.size());
    boxes.resize(curves.size());
    spinBoxes.resize(curves.size());
    for(int i=0;i<curves.size();i++)
    {
        QwtPlotCurve* curve = curves[i];
        if(curve != NULL)
        {
            QLabel* label = new QLabel(QString::number(i));
            QPen pen = curve->pen();
            
            ColorPickerButton* button = new ColorPickerButton(pen.color());
            buttons[i] = button;

            QSpinBox* spinBox = new QSpinBox();
            spinBox->setValue(pen.width());
            spinBoxes[i] = spinBox;

            QComboBox* combo = new QComboBox();
            combo->addItem("No Curve", QVariant(0));
            combo->addItem("Lines", QVariant(1));
            combo->addItem("Sticks", QVariant(2));
            combo->addItem("Steps", QVariant(3));
            combo->addItem("Dots", QVariant(4));
            combo->setCurrentIndex(curve->style());
            boxes[i] = combo;

            layout.setAlignment(Qt::AlignTop);
            layout.addWidget(label, i, 0, 1, 1);
            layout.addWidget(spinBox, i, 1, 1, 1);
            layout.addWidget(combo, i, 2, 1, 1);
            layout.addWidget(button, i, 3, 1, 1);
        }
    }
    setLayout(&layout);
}

CurveOptionWidget::~CurveOptionWidget() { }

