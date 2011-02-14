/* 
 * File:   MuliViewWidget.cc
 * Author: blueck
 * 
 * Created on 10. Februar 2011, 14:14
 */

#include <QtCore/qhash.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qbuttongroup.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qstackedwidget.h>

#include "MultiViewWidget.h"

MultiViewWidget::MultiViewWidget(QWidget* parent) : QWidget(parent),
        fileMenu(tr("&File"))
{
    currentWidget = NULL;
    position = Top;
    thumbnailWidth = 150;
    thumbnailHeight = 150;
    upperLayout = new QHBoxLayout();
    vertLayout = new QVBoxLayout();
    layout.setAlignment(Qt::AlignTop);
    layoutWidget.setLayout(upperLayout);
    layout.addWidget(&menuBar, 0, 0, 1, 1);
    layout.addWidget(&layoutWidget, 1, 0, 1, 1, Qt::AlignLeft);
//    addMenu();
    setLayout(&layout);
}

MultiViewWidget::~MultiViewWidget()
{
}

void MultiViewWidget::setThumbnailSize(int width, int height)
{
    thumbnailWidth = width;
    thumbnailHeight = height;
}

void MultiViewWidget::setThumbnailPosition(int position)
{
    this->position = (Position)position;
    switch(position)
    {
        case Left:
        {
            layout.setAlignment(Qt::AlignLeft);
            layout.removeWidget(&layoutWidget);
            QList<QString> keys = widgets.keys();
            std::cout << keys.size() << std::endl;
            for(int i=0;i<keys.size();i++)
            {
                WidgetButton* widget = widgets.value(keys[i]);
                std::cout << widget->getWidgetName().toStdString() << std::endl;
                upperLayout->removeWidget(widget);
                vertLayout->addWidget(widget);
            }
            delete upperLayout;
            layoutWidget.setLayout(vertLayout);
            layout.addWidget(&layoutWidget, 1, 0, 1, 1, Qt::AlignTop);
            if(currentWidget != NULL)
            {
                layout.removeWidget(currentWidget);
                layout.addWidget(currentWidget, 1, 1, 1, 1);
            }
            layout.setColumnStretch(1, 10);
            upperLayout = new QHBoxLayout();
            break;
        }
        case Top:
        {
            layout.removeWidget(&layoutWidget);
            layout.addWidget(&layoutWidget, 1, 0, 1, 1, Qt::AlignLeft);
            break;
        }
        case Right:
        {
            layout.setAlignment(Qt::AlignRight);
            layout.removeWidget(&layoutWidget);
            QList<QString> keys = widgets.keys();
            std::cout << keys.size() << std::endl;
            for(int i=0;i<keys.size();i++)
            {
                WidgetButton* widget = widgets.value(keys[i]);
                std::cout << widget->getWidgetName().toStdString() << std::endl;
                upperLayout->removeWidget(widget);
                vertLayout->addWidget(widget);
            }
            delete upperLayout;
            layoutWidget.setLayout(vertLayout);
            layout.addWidget(&layoutWidget, 1, 2, 1, 1, Qt::AlignTop);
            if(currentWidget != NULL)
            {
                layout.removeWidget(currentWidget);
                layout.addWidget(currentWidget, 1, 1, 1, 1);
            }
            std::cout << "Strectching" << std::endl;
            layout.setColumnStretch(1, 10);
            upperLayout = new QHBoxLayout();
            break;
        }
        case Bottom:
        {
            layout.removeWidget(&layoutWidget);
            layout.addWidget(&layoutWidget, 3, 0, 1, 1, Qt::AlignLeft);
            break;
        }
        default:
        {
            
        }
    }
}

void MultiViewWidget::addMenu()
{
    menuBar.addMenu(&fileMenu);
}

void MultiViewWidget::addWidget(const QString &name, QWidget* widget)
{
    WidgetButton* widgetButton = new WidgetButton();
    widgetButton->setFixedSize(thumbnailWidth, thumbnailHeight);
    // show the widget if its the only one
    if(widgets.size() == 0)
    {
        widgetButton->setWidget(name, widget, false);
        if(position == Top || position == Bottom)
        {
            layout.addWidget(widget, 2, 0, 1, 1);
        }
        else
        {
            layout.addWidget(widget, 1, 1, 1, 1);
        }
        currentWidget = widget;
    }
    else
    {
        widgetButton->setWidget(name, widget);
    }
    widgets.insert(name, widgetButton);
    if(position == Top || position == Bottom)
    {
        upperLayout->addWidget(widgetButton);
    }
    else
    {
        vertLayout->addWidget(widgetButton);
    }
    connect(widgetButton, SIGNAL(clicked()), this, SLOT(widgetClicked()));
}

QWidget* MultiViewWidget::getWidget(const QString& name)
{
    return widgets.value(name);
}

void MultiViewWidget::deleteWidget(const QString& name)
{
    widgets.remove(name);
}

void MultiViewWidget::widgetClicked()
{
    WidgetButton* sender = (WidgetButton*)QObject::sender();
    sender->showWidget(false);
    QWidget* widget = sender->getWidget();
    widget->setEnabled(true);
    layout.removeWidget(currentWidget);
    if(position == Top || position == Bottom)
    {
        layout.addWidget(widget, 2, 0, 1, 1);
    }
    else
    {
        layout.addWidget(widget, 1, 1, 1, 1);
    }
    currentWidget = widget;
    QList<QString> keys = widgets.keys();
    for(int i=0;i<keys.size();i++)
    {
        WidgetButton* button = widgets.value(keys[i]);
        if(button != sender)
        {
            button->showWidget(true);
        }
    }
//    repaint();
}

