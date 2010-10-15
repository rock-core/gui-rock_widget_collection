#ifndef COLORPICKERBUTTON_H
#define COLORPICKERBUTTON_H

#include <QtGui/QPushButton>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QColorDialog>

#include <iostream>


class ColorPickerButton : public QPushButton
{
  
  Q_OBJECT
  
  public:
    ColorPickerButton(QColor displayColor, QWidget* parent = 0);
    
    void paintEvent(QPaintEvent* event);
    void setDisplayColor(QColor newColor) { displayColor = newColor; };
    QColor getDisplayColor() { return displayColor; };
    
  public slots:
    void openColorDialog();
    void newColorSelected(const QColor& color);
    
  signals:
    void colorSelected(const QColor& color);
    
  protected:
    QColor displayColor;
    QColorDialog colorDialog;
  
};

#endif // COLORPICKERBUTTON_H
