
#include "Plot2d.h"
#include <stdexcept>
#include <sstream>
#include <QtCore/QtPlugin>
#include <QMouseEvent>


Plot2d::Plot2d(QWidget *parent):
  QCustomPlot(parent),auto_scroll(true)
{
}

Plot2d::~Plot2d(){} 

// wrapper function for ruby bindings
void Plot2d::removeGraph(int index)
{
   QCustomPlot::removePlottable(index);
}



