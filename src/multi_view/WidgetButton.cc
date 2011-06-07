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
#include "MultiWidget.h"

#include <stdio.h>
#include <typeinfo>


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

void WidgetButton::printStatus()
{
    std::cout << (this == NULL) << std::endl;
    std::cout << "Alt: " << isAlternative << std::endl;
}
		
		
void WidgetButton::setActive(bool active){
	isActive=false;
	MultiWidget *widget = dynamic_cast<MultiWidget*>(mainWidget);
	if(widget > 0){
		widget->setActive(active);
	}else{
		printf("Not updateing from class: %s %i\n",typeid(mainWidget).name(),widget);
	}

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
							MultiWidget *widget = dynamic_cast<MultiWidget*>(mainWidget);
							if(widget > 0){
							printf("\nbla3\n");
								setText(widget->getMinimizedLabel());
							}else{
							printf("\nbla4\n");
								setText(name);
							}
              setIcon(QIcon());
            }
        }
        mainWidget->setEnabled(false);
    }
    else
    {
        if(icon.isNull() || isAlternative == false)
        {
						MultiWidget *widget = dynamic_cast<MultiWidget*>(mainWidget);
						if(widget > 0){
							printf("\nbla1\n");
							setText(widget->getMinimizedLabel());
						}else{
							printf("\nbla2\n");
							setText(name);
						}
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
        std::cout << "No notifyUpdate(int) method found, updates will not be displayed for: " << name.toStdString() << std::endl;
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

