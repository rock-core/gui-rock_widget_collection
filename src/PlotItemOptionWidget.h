
#ifndef CURVEOPTIONWIDGET_H
#define CURVEOPTIONWIDGET_H

#include <QWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QColorDialog>
#include <QtGui/QPushButton>
#include <QtGui/QPen>
#include <QtGui/QComboBox>

#include <qwt-qt4/qwt_plot_curve.h>

#include <iostream>

#include "ColorPickerButton.h"

class PlotItemOptionWidget : public QWidget
{
  
    Q_OBJECT
  
  public:
    
    PlotItemOptionWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    void layoutWidget(int id, QPen pen);
    
  public slots:
    void newColorSelected(const QColor& color);
    
  signals:
    void colorSelected(int curveId, const QColor& color);
    
  protected:
    
    int itemId;
    QGridLayout layout;
    QLabel plotItemIdLabel;
    QColorDialog colorDialog;
    QComboBox comboBox;
    ColorPickerButton colorButton;
    
};

#endif // CURVEOPTIONWIDGET_H
