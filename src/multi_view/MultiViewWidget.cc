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


MultiViewWidget::MultiViewWidget(QWidget* parent) : QWidget(parent)
{
    initialized = false;
    currentWidget = NULL;
    pseudoWidget = NULL;
    designerMode = true;
    clicking = false;
    currentWidgetIndex = 0;
    thumbnailWidth = 150;
    thumbnailHeight = 150;
    
//		//Central Widget
//		mainLayout = new QVBoxLayout();
//		mainWidget = new QWidget();
//		mainLayout->addWidget(mainWidget);
//
//		topLayout = new QHBoxLayout(mainWidget);
//		mainWidget->setLayout(topLayout);
//		topWidget = new QWidget();
//		top
//
//		midRowLayout = new QHBoxLayout(mainLayout);
//		midLeftLayout = new QVBoxLayout(midRowLayout);
//		centralLayout = new QVBoxLayout(midRowLayout);
//		midRightLayout = new QVBoxLayout(midRowLayout);
//		bottomLayout = new QHBoxLayout(mainLayout);


		//upperLayout = new QHBoxLayout();
    //upperLayout = new QHBoxLayout();
    //vertLayout = new QVBoxLayout();
		
		//top_w = new QWidget();
		//bottom_w = new QWidget();
		//left_w = new QWidget();
		//mid_w = new QWidget();
		//right_w = new QWidget();

		
	/*
		bottom_w->setMinimumSize(thumbnailWidth,thumbnailHeight+20);
		bottom_w->setMaximumSize(10000,thumbnailHeight+20);
		
		top_w->setMinimumSize(thumbnailWidth,thumbnailHeight+20);
		top_w->setMaximumSize(10000,thumbnailHeight+20);
		
		left_w->setMinimumSize(thumbnailWidth+20,thumbnailHeight);
		left_w->setMaximumSize(thumbnailWidth+20,10000);
		
		
		*/
		
		//left_w->setMinimumSize(thumbnailWidth,thumbnailHeight);
		//right_w->setMinimumSize(thumbnailWidth,thumbnailHeight);

		//top->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

		//mid_w->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
		//right_w->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
		//left_w->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

		top = new QHBoxLayout();
		bottom = new QHBoxLayout();
		left = new QVBoxLayout();
		mid = new QVBoxLayout();
		right = new QVBoxLayout();

		top->addSpacerItem(new QSpacerItem(100,100));

		//top_w->setPalette((QColor("red")));
		//top_w->setBackgroundRole(QPalette::Window);
		//mid_w->setPalette((QColor("green")));
		//bottom_w->setPalette((QColor("blue")));

/*
		top->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		bottom->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		mid->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		right->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		left->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	*/


		WidgetButton *button4 = new WidgetButton();
		WidgetButton *button2 = new WidgetButton();
		left->addWidget(button4);
		layout.addLayout(top,0,0,1,3);
		layout.addLayout(left,1,0,1,1);
		layout.addLayout(mid,1,1,1,2);


    //layoutWidget.setLayout(upperLayout);
    //layout.addWidget(&menuBar, 0, 0, 1, 1, Qt::AlignTop);
    //layout.addWidget(&layoutWidget, 1, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);
    setLayout(&layout);
    //setThumbnailPosition(position);
		
		
		//layout.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

		//layout.addWidget(top_w,0,0,1,3);
		//layout.addWidget(left_w,1,0,1,1);
		//layout.addWidget(mid_w,1,0,1,1);
		//layout.addWidget(right_w,1,2,1,1);
		//layout.addWidget(bottom_w,2,0,1,3);
    
		//updateView();
		initialized=true;		
		
		
		left->addWidget(button2);
    layout.update();
    layout.activate();

}

MultiViewWidget::~MultiViewWidget()
{
}


void MultiViewWidget::updateView(){
		return;
		int midStart = 1;

		if(top->count() == 0){
			layout.removeItem(top);
			midStart-=1;
		}else{
			layout.addLayout(top,0,0,1,3);
		}

		int centerWidth = 3;
		if(right->count() > 0){
			centerWidth--;
			layout.removeItem(right);
			layout.addLayout(right,midStart,2,1,1);
		}else{
			layout.removeItem(right);
		}

		if(left->count() > 0){
			layout.removeItem(mid);
			centerWidth--;
			layout.addLayout(left,midStart,0,1,1);
			layout.addLayout(mid,midStart,1,1,centerWidth);
		}else{
			layout.removeItem(left);
			layout.removeItem(mid);
			layout.addLayout(mid,midStart,0,1,centerWidth);
		}


		if(bottom->count() > 0){
			layout.addLayout(bottom,midStart+1,0,1,3);
		}else{
			layout.removeItem(bottom);
		}
		
	
}

void MultiViewWidget::setThumbnailSize(int width, int height)
{
    thumbnailWidth = width;
    thumbnailHeight = height;
}

#if 0
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
#endif 

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
		WidgetButton* widgetButton=0;

    widgetButton = new WidgetButton();
    widgetButton->setFixedSize(thumbnailWidth, thumbnailHeight);
    widgetButton->setIconAlternative(icon, useOnlyIcon);

    
    // show the widget if its the only one
    if(widgets.size() == 0)
    {
        widgetButton->setWidget(name, widget, false);
        widgets.insert(name, widgetButton);
//        if(widgetButton->getPosition() == WidgetButton::Top || widgetButton->getPosition() == WidgetButton::Bottom)
//        {
						printf("Adding an widget to mid\n");
            //mid->addWidget(widget);
//        }
//        else
//        {
//            mid.addWidget(widget);
//        }
        currentWidget = widgetButton;
    }
    else
    {
        widgetButton->setWidget(name, widget);
    }

    widgets.insert(name, widgetButton);
    left->addWidget(widgetButton);

		/*
    switch(currentWidget->getPosition())
    {
			case WidgetButton::Left:
        left->addWidget(widgetButton);
				break;
			case WidgetButton::Right:
        right->addWidget(widgetButton);
				break;
			case WidgetButton::Bottom:
        bottom->addWidget(widgetButton);
				break;
			case WidgetButton::Top:
        top->addWidget(widgetButton);
				break;
		}*/

				
    connect(widgetButton, SIGNAL(clicked()), this, SLOT(widgetClicked()));
    emit widgetButtonAdded(widgetButton);
		updateView();

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
        mid->removeWidget(button);
        top->removeWidget(button);
        left->removeWidget(button);
        right->removeWidget(button);
        bottom->removeWidget(button);
        delete button;
    }
		updateView();
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
		//QWidget::childEvent(event);
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
                            mid->removeWidget(button);
                            left->removeWidget(button);
                            right->removeWidget(button);
                            bottom->removeWidget(button);
                            top->removeWidget(button);
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
/*
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
		*/
    widget->setEnabled(true);

		currentWidget->setActive(false);
		sender->setActive(true);

    //mid->removeWidget(currentWidget->getWidget());
    //mid->addWidget(widget);
    
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

		/*
    // try to be stupid and use an arbitary large number
    // this is NOT a good solution, if you've got a better one....
    if(currentWidget->getWidget()->maximumHeight() < 1000000 || currentWidget->getWidget()->maximumWidth() < 1000000)
    {
        // no breaks in left and right are ON PURPOSE
        switch(currentWidget->getPosition())
        {
            case WidgetButton::Left:
            {
                center.setColumnStretch(3, 10);
            }
            case WidgetButton::Right:
            {
                layout.setColumnStretch(0, 10);
            }
            case WidgetButton::Top:
            {
                layout.setRowStretch(3, 10);
                break;
            }
            case WidgetButton::Bottom:
            {
                layout.setRowStretch(0, 10);
                break;
            }
        }
    }
    else
    {
        switch(currentWidget->getPosition())
        {
            case WidgetButton::Left:
            {
                layout.setColumnStretch(3, 0);
            }
            case WidgetButton::Right:
            {
                layout.setColumnStretch(0, 0);
            }
            case WidgetButton::Top:
            {
                layout.setRowStretch(3, 0);
                break;
            }
            case WidgetButton::Bottom:
            {
                layout.setRowStretch(0, 0);
                break;
            }
        }
    }
		*/
    clicking = false;
//    repaint();
}

