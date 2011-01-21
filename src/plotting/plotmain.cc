/* 
 * File:   plotmain.cc
 * Author: blueck
 *
 * Created on 19. Januar 2011, 12:46
 */

#include <cstdlib>
#include <iostream>
#include "QtGui/qapplication.h"
#include <QtGui/qwidget.h>

#include "PlotWidget.h"
#include "AxisOptionsWidget.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    QApplication* app = new QApplication(argc, argv);
    PlotWidget* widget = new PlotWidget(NULL);
//    AxisOptionsWidget* widget = new AxisOptionsWidget();
//    widget->setFixedSize(400, 300);
//    widget->initializeLayout();
    widget->show();
    app->exec();
    return 0;
}

