
#include "PlotItemOptionWidget.h"

PlotItemOptionWidget::PlotItemOptionWidget(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f), 
  colorDialog(parent), colorButton(Qt::black)
{
  
  connect(&colorButton, SIGNAL(colorSelected(const QColor&)), this, SLOT(newColorSelected(const QColor&)));
  
}


void PlotItemOptionWidget::newColorSelected(const QColor& color)
{
  emit colorSelected(itemId, color);
}

void PlotItemOptionWidget::layoutWidget(int id, QPen pen)
{
  itemId = id;
  plotItemIdLabel.setText(QString::number(id));
  colorButton.setDisplayColor(pen.color());
  colorButton.repaint();
  layout.addWidget(&plotItemIdLabel, 0, 0);
  layout.addWidget(&colorButton, 0, 1);
  setLayout(&layout);
}

