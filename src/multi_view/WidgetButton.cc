/* 
 * File:   WidgetButton.cc
 * Author: blueck
 * 
 * Created on 11. Februar 2011, 12:01
 */

#include <QtGui/qwidget.h>
#include <QtGui/qlayout.h>
#include <QtGui/qabstractbutton.h>
#include <QtCore/qtimer.h>
#include <QtGui/qlabel.h>

#include "WidgetButton.h"

WidgetButton::WidgetButton() : QPushButton(),
        redPalette(QPalette(Qt::red))
{
    isAlternative = false;
    paletteTimer.setInterval(750);
    connect(&paletteTimer, SIGNAL(timeout()), this, SLOT(changePalette()));
    defaultPalette = this->palette();
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
        if(isAlternative == false)
        {
            setIcon(QIcon());
            layout.addWidget(mainWidget);
        }
        else
        {
            if(!icon.isNull())
            {
                setText("");
                setIcon(icon);
            }
            else
            {
                setText(name);
                setIcon(QIcon());
            }
        }
//        layout.addWidget(mainWidget);
        mainWidget->setEnabled(false);
    }
    else
    {
        if(icon.isNull() || isAlternative == false)
        {
            setText(name);
        }
        else
        {
            setText("");
            setIcon(icon);
        }
        setEnabled(false);
        setPalette(QPalette(QColor(0, 255, 0)));
    }
    const QMetaObject* meta = widget->metaObject();
    int index = meta->indexOfMethod("notifyUpdate(int)");
    if(index > -1)
    {
        connect(widget, SIGNAL(notifyUpdate(int)), this, SLOT(widgetUpdate(int)));
    }
    else
    {
        std::cout << "No notifyUpdate method found, updates will not be displayed for: " << name.toStdString() << std::endl;
    }
}

void WidgetButton::showWidget(bool shown)
{
    if(shown)
    {
        setText("");
        setPalette(defaultPalette);
        if(isAlternative == false)
        {
            setIcon(QIcon());
            layout.addWidget(mainWidget);
        }
        else
        {
            layout.removeWidget(mainWidget);
            if(!icon.isNull())
            {
                setText("");
                setIcon(icon);
            }
            else
            {
                setText(name);
                setIcon(QIcon());
            }
        }
        mainWidget->setEnabled(false);
        setEnabled(true);
    }
    else
    {
        paletteTimer.stop();
        setPalette(QPalette(QColor(0, 255, 0)));
        layout.removeWidget(mainWidget);
        mainWidget->setEnabled(true);
        if(icon.isNull())
        {
            setText(name);
        }
        else
        {
            setIcon(icon);
        }
        setEnabled(false);
    }
    repaint();
}

void WidgetButton::setIconAlternative(const QIcon& icon, bool isAlternative)
{
    this->icon = icon;
    this->isAlternative = isAlternative;
}

QWidget* WidgetButton::getWidget()
{
    return mainWidget;
}

QString WidgetButton::getWidgetName()
{
    return name;
}

void WidgetButton::widgetUpdate(int type)
{
    if(!mainWidget->isEnabled())
    {
        setPalette(redPalette);
        paletteTimer.start();
    }
}

void WidgetButton::changePalette()
{
    if(palette().color(QPalette::Background).red() == 255)
    {
        setPalette(defaultPalette);
    }
    else
    {
        setPalette(redPalette);
    }
}

