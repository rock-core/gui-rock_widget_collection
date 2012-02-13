#include <QtGui>
#include <QtCore>
#include <iostream>

#include "Timeline.h"
#include "TestWidget.h"

Timeline *tw = 0;
TestWidget *testWidget = 0;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    QWidget *window = new QWidget();
    window->resize(400,300);
    window->show();
      
    QVBoxLayout vbox(window);
    vbox.setAlignment(Qt::AlignTop);
    
    tw = new Timeline(window);
    tw->setWidth(400);
    tw->setMinIndex(0);
    tw->setSteps(10000);
    tw->setStepSize(1);
    
    tw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tw->setSliderIndex(3);
    
    QVector<int> bookmarks;
    bookmarks << 0 << 123 << 3334 << 5543 << 8888 << 9456;
    tw->addBookmarks(bookmarks);
    tw->show();
    
    vbox.addWidget(tw);

    // Start test widget
    testWidget = new TestWidget(tw);
    testWidget->setWindowTitle("Testwidget");
    
    return app.exec();
}
