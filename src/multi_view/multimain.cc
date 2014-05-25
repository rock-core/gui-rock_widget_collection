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
#include "image_view_old/ImageViewOld.h"

#include <limits>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    QApplication* app = new QApplication(argc, argv);
    MultiViewWidget* widget = new MultiViewWidget(NULL);
//    widget->setThumbnailPosition(0);
    widget->setThumbnailSize(100, 100);
    ImageViewOld* imageView = new ImageViewOld();
    QImage* image = new QImage();
    image->load("/home/blueck/pics/testimage.jpg");
    imageView->addImage(*image);
//    imageView->setMaximumSize(image->width(), image->height());

    widget->addWidget(QString("Image"), imageView);
    widget->show();
    app->exec();
    return 0;
}

