/* 
 * File:   WidgetButton.cc
 * Author: blueck
 * 
 * Created on 11. Februar 2011, 12:01
 */

#include <QtGui/qwidget.h>
#include <QtGui/qlayout.h>
#include <QtGui/qabstractbutton.h>

#include "WidgetButton.h"

WidgetButton::WidgetButton() : QPushButton()
{
    setLayout(&layout);
}

WidgetButton::~WidgetButton()
{
}

void WidgetButton::setWidget(const QString &name, QWidget* widget, bool shown)
{
    this->name = name;
    mainWidget = widget;
    if(shown)
    {
        setEnabled(true);
        layout.addWidget(mainWidget);
        mainWidget->setEnabled(false);
    }
    else
    {
        setText(name);
        setEnabled(false);
    }
}

void WidgetButton::showWidget(bool shown)
{
    if(shown)
    {
        setText("");
        layout.addWidget(mainWidget);
        mainWidget->setEnabled(false);
        setEnabled(true);
    }
    else
    {
        layout.removeWidget(mainWidget);
        mainWidget->setEnabled(true);
        setText(name);
        setEnabled(false);
    }
    repaint();
}

QWidget* WidgetButton::getWidget()
{
    return mainWidget;
}

QString WidgetButton::getWidgetName()
{
    return name;
}

