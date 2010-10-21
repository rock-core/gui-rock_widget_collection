/* 
 * File:   CurveOptionWidget.h
 * Author: blueck
 *
 * Created on 21. Oktober 2010, 10:37
 */

#ifndef CURVEOPTIONWIDGET_H
#define	CURVEOPTIONWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>

#include <qwt-qt4/qwt_plot_curve.h>

#include "ColorPickerButton.h"

class CurveOptionWidget : public QWidget
{

    Q_OBJECT
    
public:
    CurveOptionWidget();
    virtual ~CurveOptionWidget();
    void initializeLayout(std::vector<QwtPlotCurve*> curves);
    void updateExistingCurves();

protected:
    QGridLayout layout;
    std::vector<ColorPickerButton*> buttons;
    std::vector<QComboBox*> boxes;
    std::vector<QwtPlotCurve*> curves;
    std::vector<QSpinBox*> spinBoxes;
private:

};

#endif	/* CURVEOPTIONWIDGET_H */

