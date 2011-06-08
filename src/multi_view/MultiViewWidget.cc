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
#include <QtCore/qmetaobject.h>
#include <QtCore/qcoreevent.h>
#include <QtGui/qtooltip.h>

#include "MultiViewWidget.h"
#include "MultiViewPlugin.h"
#include "plotting/PlotWidget.h"


#include <typeinfo>


MultiViewWidget::MultiViewWidget(QWidget* parent) : QWidget(parent),
        fileMenu(tr("&File")), testingAction(tr("&Test"), this)
{
    initialized = false;
    currentWidget = NULL;
    pseudoWidget = NULL;
    designerMode = true;
    clicking = false;
    position = Top;
    currentWidgetIndex = 0;
    thumbnailWidth = 150;
    thumbnailHeight = 150;
    upperLayout = new QHBoxLayout();
    vertLayout = new QVBoxLayout();
    layout.setAlignment(Qt::AlignTop);
    layoutWidget.setLayout(upperLayout);
    layout.addWidget(&menuBar, 0, 0, 1, 1, Qt::AlignTop);
    layout.addWidget(&layoutWidget, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    //addMenu();
    setLayout(&layout);
    setThumbnailPosition(position);
    initialized = true;
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
            for(int i=0;i<keys.size();i++)
            {
                WidgetButton* widget = widgets.value(keys[i]);
                upperLayout->removeWidget(widget);
                vertLayout->addWidget(widget);
            }
            delete upperLayout;
            layoutWidget.setLayout(vertLayout);
            layout.addWidget(&layoutWidget, 1, 0, 1, 1, Qt::AlignTop);
            if(currentWidget != NULL)
            {
                layout.removeWidget(currentWidget->getWidget());
                layout.addWidget(currentWidget->getWidget(), 1, 1, 1, 1);
            }
            layout.setRowStretch(2, 0);
            layout.setRowStretch(1, 10);
            layout.setColumnStretch(1, 10);
            upperLayout = new QHBoxLayout();
            break;
        }
        case Top:
        {
            layout.setAlignment(Qt::AlignTop);
            layout.removeWidget(&layoutWidget);
            layout.setRowStretch(2, 10);
            layout.addWidget(&layoutWidget, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
            break;
        }
        case Right:
        {
            layout.setAlignment(Qt::AlignRight);
            layout.removeWidget(&layoutWidget);
            QList<QString> keys = widgets.keys();
            for(int i=0;i<keys.size();i++)
            {
                WidgetButton* widget = widgets.value(keys[i]);
                upperLayout->removeWidget(widget);
                vertLayout->addWidget(widget);
            }
            delete upperLayout;
            layoutWidget.setLayout(vertLayout);
            layout.addWidget(&layoutWidget, 1, 2, 1, 1, Qt::AlignTop);
            if(currentWidget != NULL)
            {
                layout.removeWidget(currentWidget->getWidget());
                layout.addWidget(currentWidget->getWidget(), 1, 1, 1, 1);
            }
            layout.setColumnStretch(1, 10);
            layout.setRowStretch(2, 0);
            layout.setRowStretch(1, 10);
            upperLayout = new QHBoxLayout();
            break;
        }
        case Bottom:
        {
            layout.setAlignment(Qt::AlignTop);
            layout.removeWidget(&layoutWidget);
            layout.setRowStretch(2, 10);
            layout.addWidget(&layoutWidget, 3, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
            break;
        }
        // same as top
        default:
        {
            layout.setAlignment(Qt::AlignTop);
            layout.removeWidget(&layoutWidget);
            layout.setRowStretch(2, 10);
            layout.addWidget(&layoutWidget, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
            break;
        }
    }
}

void MultiViewWidget::addMenu()
{
    menuBar.addMenu(&fileMenu);
    fileMenu.addAction(&testingAction);
    connect(&testingAction, SIGNAL(triggered()), this, SLOT(doTesting()));
}

WidgetButton* MultiViewWidget::getButtonForWidget(QWidget* widget)
{
    QList<QString> keys = widgets.keys();
    for(int i=0;i<keys.size();i++)
    {
        WidgetButton* button = widgets[keys[i]];
        if(button->getWidget() == widget)
        {
            return button;
        }
    }
    return NULL;
}

void MultiViewWidget::addWidget(const QString &name, QWidget* widget_, const QIcon &icon, bool useOnlyIcon)
{
		QWidget *widget = widget_;
   
#if 0 //Not possible because ruby qt don't initialize all components as QWidget so we have no chance to get the right obect at this point
		//Try cast the QT way
		MultiWidget *w = qobject_cast<MultiWidget*>(widget_);
		if(w){
			widget= w;
			printf("Cast sucsessful\n");
		}else{
			//MultiWidget *bla = (MultiWidget*) widget_;
			printf("ARGHHHH: %s Name: %s \n",typeid(widget).name(),widget->objectName().toStdString().c_str());
			
			printf("QT Means: %s\n",widget->metaObject()->className());
			const QMetaObject *o = widget->metaObject();
			for(int i=0; i<o->methodCount(); i++){
				printf("Method found: %s\n",o->method(i).signature());
			}
			printf("\n\n");
			//printf("Got one: %s \n",bla->getMinimizedLabel().toStdString().c_str());
			//printf("Cast failed\n");
		}
#endif

		
		if(widgets.contains(name))
    {
        std::cerr << "A Widget with the name: [" << name.toStdString() << "] already exists!" << std::endl;
        return;
    }
    WidgetButton* widgetButton = new WidgetButton();
    widgetButton->setFixedSize(thumbnailWidth, thumbnailHeight);
    widgetButton->setIconAlternative(icon, useOnlyIcon);

    
    // show the widget if its the only one
    if(widgets.size() == 0)
    {
        widgetButton->setWidget(name, widget, false);
        widgets.insert(name, widgetButton);
        if(position == Top || position == Bottom)
        {
            layout.addWidget(widget, 2, 0, 1, 1);
        }
        else
        {
            layout.addWidget(widget, 1, 1, 1, 1);
        }
        currentWidget = widgetButton;
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
    emit widgetButtonAdded(widgetButton);

		timer.setSingleShot(true);
		timer.start(500);
	  connect(&timer,SIGNAL(timeout()),this,SLOT(fixStatus()));
}

void MultiViewWidget::fixStatus(){
	QList<WidgetButton*> list = getAllWidgetButtons(); 
	for(int i=0;i<list.size();i++){
		if(list[i] == currentWidget){
			list[i]->setActive(true);
		}else{
			list[i]->setActive(false);
		}
		list[i]->corrcetName();
	}
}

void MultiViewWidget::doTesting()
{
    QList<QString> keys = widgets.keys();
    for(int i=0;i<keys.size();i++)
    {
        QWidget* widget = (QWidget*)widgets[keys[i]]->getWidget();
        const QMetaObject* meta = widget->metaObject();
        int index = meta->indexOfMethod("doTesting()");
        if(index > -1)
        {
            meta->invokeMethod(widget, "doTesting");
        }
    }
}

QWidget* MultiViewWidget::getWidget(const QString& name)
{
    return widgets.value(name)->getWidget();
}

void MultiViewWidget::deleteWidget(const QString& name)
{
    if(widgets.contains(name))
    {
        WidgetButton* button = widgets[name];
        widgets.remove(name);
        QWidget* widget = button->getWidget();
        layout.removeWidget(widget);
        upperLayout->removeWidget(button);
        vertLayout->removeWidget(button);
        delete button;
    }
}

bool MultiViewWidget::event(QEvent* event)
{
    if(event->type() == QEvent::ToolTip)
    {
        QToolTip::hideText();
        event->ignore();
        return true;
    }
    return QWidget::event(event);
}

void MultiViewWidget::childEvent(QChildEvent* event)
{
    if(initialized)
    {
        
        if(event->type() == QChildEvent::ChildAdded)
        {
            if(!clicking)
            {
                if(event->child()->isWidgetType())
                {
                    // we never need to watch adding of widgetbuttons, then everything went well already
                    WidgetButton* testButton = dynamic_cast<WidgetButton*> (event->child());
                    if(testButton != NULL)
                    {
                        return;
                    }
                    // we never want to add a widget which is already added
                    QWidget* child = (QWidget*)event->child();
                    QList<QString> keys = widgets.keys();
                    for(int i=0;i<keys.size();i++)
                    {
                        WidgetButton* button = widgets[keys[i]];
                        if(button->getWidget() == child)
                        {
    //                        if(button->isEnabled())
    //                        {
    //                            child->setMaximumSize(0, 0);
    //                        }
    //                        else
    //                        {
    //                            child->setMaximumSize(1000000, 1000000);
    //                        }
                            return;
                        }
                        
                    }
										if(child->windowTitle().isEmpty())
                    	addWidget(QString::number(currentWidgetIndex), child);
										else
                    	addWidget(child->windowTitle(), child);
			
                    currentWidgetIndex++;
                }
            }
        }
        else if(event->type() == QChildEvent::ChildRemoved)
        {
            if(!clicking)
            {
                if(event->child()->isWidgetType())
                {
                    // look which widget was removed and remove the button also
                    QWidget* child = (QWidget*)event->child();
                    QList<QString> keys = widgets.keys();
                    for(int i=0;i<keys.size();i++)
                    {
                        WidgetButton* button = widgets[keys[i]];
                        if(button->getWidget() == child)
                        {
                            widgets.remove(keys[i]);
                            QWidget* widget = button->getWidget();
                            layout.removeWidget(widget);
                            upperLayout->removeWidget(button);
                            vertLayout->removeWidget(button);
                            widget = NULL;
                            return;
                        }
                    }
                }
            }
        }
    }
}

QList<WidgetButton*> MultiViewWidget::getAllWidgetButtons()
{
    return widgets.values();
}

void MultiViewWidget::widgetClicked()
{
    clicking = true;

		WidgetButton *sender = dynamic_cast<WidgetButton*>(QObject::sender());
		if(sender <= 0){
			fprintf(stderr,"FATAL: Cannot Handle unknown Widget %s:%i\n",__FILE__,__LINE__);
			return;
		}

    //sender->printStatus();
    sender->showWidget(false);
    QWidget* widget = sender->getWidget();

    if(designerMode)
    {
        QList<WidgetButton*> buttons = getAllWidgetButtons();
        for(int i=0;i<buttons.size();i++)
        {
            if(sender != buttons[i])
            {
                buttons[i]->getWidget()->setMaximumSize(0, 0);
            }
            else
            {
                buttons[i]->getWidget()->setMaximumSize(1000000, 1000000);
            }

        }
    }
    widget->setEnabled(true);

		currentWidget->setActive(false);
		sender->setActive(true);

    layout.removeWidget(currentWidget->getWidget());
    if(position == Top || position == Bottom)
    {
        layout.addWidget(widget, 2, 0, 1, 1);
    }
    else
    {
        layout.addWidget(widget, 1, 1, 1, 1);
    }
    currentWidget = sender;
    QList<QString> keys = widgets.keys();
    for(int i=0;i<keys.size();i++)
    {
        WidgetButton* button = widgets.value(keys[i]);
        if(button != sender)
        {
            button->showWidget(true);
        }
    }
    // try to be stupid and use an arbitary large number
    // this is NOT a good solution, if you've got a better one....
    if(currentWidget->getWidget()->maximumHeight() < 1000000 || currentWidget->getWidget()->maximumWidth() < 1000000)
    {
        // no breaks in left and right are ON PURPOSE
        switch(position)
        {
            case Left:
            {
                layout.setColumnStretch(3, 10);
            }
            case Right:
            {
                layout.setColumnStretch(0, 10);
            }
            case Top:
            {
                layout.setRowStretch(3, 10);
                break;
            }
            case Bottom:
            {
                layout.setRowStretch(0, 10);
                break;
            }
        }
    }
    else
    {
        switch(position)
        {
            case Left:
            {
                layout.setColumnStretch(3, 0);
            }
            case Right:
            {
                layout.setColumnStretch(0, 0);
            }
            case Top:
            {
                layout.setRowStretch(3, 0);
                break;
            }
            case Bottom:
            {
                layout.setRowStretch(0, 0);
                break;
            }
        }
    }
    clicking = false;
//    repaint();
}

