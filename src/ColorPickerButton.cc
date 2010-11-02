
#include "ColorPickerButton.h"

ColorPickerButton::ColorPickerButton(QColor displayColor, QWidget* parent): QPushButton(parent),
  colorDialog(parent)
{
  this->displayColor = displayColor;
  connect(this, SIGNAL(clicked()), this, SLOT(openColorDialog()));
  connect(&colorDialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(newColorSelected(const QColor&)));
}

void ColorPickerButton::openColorDialog()
{
  colorDialog.show();
}

void ColorPickerButton::newColorSelected(const QColor& color)
{
  displayColor = color;
  emit colorSelected(color);
}


void ColorPickerButton::paintEvent(QPaintEvent* event)
{   
    QPushButton::paintEvent(event);
    QPainter painter(this);
    
    int width = size().width();
    int height = size().height();
    int widthPercentage = width * 0.15;
    int heightPercentage = height * 0.3;
    
    painter.fillRect(widthPercentage, heightPercentage, width - (2*widthPercentage), height - (2*heightPercentage), displayColor);
}
