/* 
 * File:   main.cc
 * Author: blueck
 *
 * Created on 17. Juni 2010, 13:29
 */

#include <cstdlib>
#include "QtGui/qapplication.h"
#include "ImageViewOldGL.h"
#include "PolygonItem.h"
#include "ArrowItem.h"
#include <QtGui/QImage>
#include <QtGui/QMainWindow>
#include <iostream>
#include <QtGui/QScrollArea>
#include <QtGui/QColor>
#include <QtCore/QPoint>

using namespace std;

/*
 * 
 */
//int main(int argc, char** argv)
//{
///*    QApplication* app = new QApplication(argc, argv);
//    QImage* image = new QImage("/home/blueck/pics/test.jpg");
//    ImageViewWidget* widget = new ImageViewWidget(image->width(), image->height());
//    QMainWindow* mainwindow = new QMainWindow();
//    mainwindow->setFixedSize(640, 480);
//    QScrollArea* area = new QScrollArea();
//    area->setWidget(widget);
//    mainwindow->setCentralWidget(area);
////    mainwindow->setCentralWidget(widget);
//    mainwindow->show();
//    QPoint* points = new QPoint[3];
//    points[0] = QPoint(10, 10);
//    points[1] = QPoint(10, 100);
//    points[2] = QPoint(20, 50);
//    ArrowItem* arrowItem = new ArrowItem(10, 20, 0, QColor(255, 0, 0), 10, 120);
//    arrowItem->setInteriorColor(new QColor(255, 0, 0));
//    arrowItem->setLineWidth(2);
// //   PolygonItem* polygonItem = new PolygonItem(QColor(255, 0, 0), 0, points, 3);
////    polygonItem->setDrawBorder(false);
////    polygonItem->setInteriorColor(new QColor(0, 0, 255));
//    widget->addItem(arrowItem);
//
////    DrawItem* textItem = widget->addText(0, 0, 0, new QColor(255, 0, 0), "Huch, wat da los?");
////    LineItem* lineItem = (LineItem*)widget->addLine(0, 0, 1, new QColor(255, 0, 255), 200, 200);
////    lineItem->setLineWidth(10);
////    EllipseItem* ellipse = (EllipseItem*)widget->addEllipse(100, 200, 2, new QColor(0, 255, 0), 300, 400);
////    RectangleItem* rect = (RectangleItem*)widget->addRectangle(100, 100, 2, new QColor(0, 255, 0), 200, 300);
//
////    ellipse->setDrawInterior(true);
////    ellipse->setDrawBorder(false);
////    rect->setInteriorColor(new QColor(255, 0, 0));
////    widget->addItem(polygonItem);
////    widget->show();
////    widget->addImage(image->bits(), image->numBytes());
//    std::cout << "Remove" << std::endl;
////    widget->removeItem(textItem);
////    widget->addText(textItem);
//    widget->repaint();
////    sleep(10);
////    for(int i=0;i<20;i++)
////    {
////        sleep(1);
////        QImage* image;
////        if(i%2 == 0)
////        {
////            std::cout << "1" << std::endl;
////            image = new QImage("/home/blueck/pics/test.jpg");
////            widget->changeFormat(image->width(), image->height(), image->format());
////            widget->addImage(image->bits(), image->numBytes());
////        }
////        else
////        {
////            std::cout << "2" << std::endl;
////            image = new QImage("/home/blueck/pics/test3.jpg");
////            widget->changeFormat(image->width(), image->height(), image->format());
////            widget->addImage(image->bits(), image->numBytes());
////        }
////    }
//    app->exec();*/
//    return 0;
//}
//
