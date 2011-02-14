/* 
 * File:   multimain.cc
 * Author: blueck
 *
 * Created on 10. Februar 2011, 14:02
 */

#include <cstdlib>
#include <iostream>
#include "QtGui/qapplication.h"
#include <QtGui/qwidget.h>

#include "MultiViewWidget.h"
#include "plotting/PlottingWidget.h"
#include "image_view/ImageView.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    QApplication* app = new QApplication(argc, argv);
    MultiViewWidget* widget = new MultiViewWidget(NULL);
    widget->setThumbnailPosition(1);
    widget->setThumbnailSize(100, 100);
    PlottingWidget* pwidget = new PlottingWidget(NULL);
    widget->addWidget(QString("Plot"), pwidget);
    PlottingWidget* pwidget2 = new PlottingWidget(NULL);
    widget->addWidget(QString("Plot2"), pwidget2);
    ImageView* imageView = new ImageView();
    QImage* image = new QImage();
    image->load("/home/blueck/pics/testimage.jpg");
    imageView->addImage(*image);
    imageView->setMaximumSize(400, 300);
    widget->addWidget(QString("Image"), imageView);
    widget->show();
    app->exec();
    return 0;
}

