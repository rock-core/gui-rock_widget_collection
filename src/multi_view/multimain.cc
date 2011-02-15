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
#include "plotting/PlotWidget.h"
#include "image_view/ImageView.h"

#include <limits>

using namespace std;

/*
 * 
 */
//int main(int argc, char** argv)
//{
//    QApplication* app = new QApplication(argc, argv);
//    MultiViewWidget* widget = new MultiViewWidget(NULL);
//    widget->setThumbnailPosition(0);
//    widget->setThumbnailSize(100, 100);
//    PlotWidget* pwidget = new PlotWidget(NULL);
//    widget->addWidget(QString("Plot"), pwidget);
//    PlotWidget* pwidget2 = new PlotWidget(NULL);
//    QIcon icon("/usr/share/psi/iconsets/system/default/psiplus/logo_48.png");
//    widget->addWidget(QString("Plot2"), pwidget2);
//    ImageView* imageView = new ImageView();
//    QImage* image = new QImage();
//    image->load("/home/blueck/pics/testimage.jpg");
//    imageView->addImage(*image);
////    imageView->setMaximumSize(image->width(), image->height());
//
//    widget->addWidget(QString("Image"), imageView);
//    widget->show();
//    app->exec();
//    return 0;
//}

