/* 
 * File:   MuliViewWidget.cc
 * Author: blueck
 * 
 * Created on 10. Februar 2011, 14:14
 */

#include <QHash>
#include <QMenuBar>
#include <QButtonGroup>
#include <QBoxLayout>
#include <QStackedWidget>
#include <QMetaObject>
#include <QToolTip>

#include "MultiViewWidget.h"
#include "MultiViewPlugin.h"


#include <typeinfo>


MultiViewWidget::MultiViewWidget(QWidget* parent) : QWidget(parent)
{
    initialized = false;
    currentWidget = NULL;
    pseudoWidget = NULL;
    designerMode = false;
    clicking = false;
    currentWidgetIndex = 0;
    thumbnailWidth = 150;
    thumbnailHeight = 150;
    

    top = new QHBoxLayout();
    bottom = new QHBoxLayout();
    left = new QVBoxLayout();
    mid = new QVBoxLayout();
    right = new QVBoxLayout();
    setLayout(&layout);
    initialized=true;		
		
		
    layout.update();
    layout.activate();

}

MultiViewWidget::~MultiViewWidget()
{
}


void MultiViewWidget::updateView(){
		int midStart = 1;
		int centerWidth = 3;

		if(top->count() == 0){
			layout.removeItem(top);
			midStart-=1;
		}else{
			centerWidth++;
			layout.addLayout(top,0,0,1,3);
		}

		
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
                layout.update();
}

void MultiViewWidget::setThumbnailSize(int width, int height)
{
    thumbnailWidth = width;
    thumbnailHeight = height;
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

void MultiViewWidget::addWidget(const QString &name, QWidget* widget_, const QIcon &icon, bool useOnlyIcon, int position)
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

    switch(position){
        case WidgetButton::Left:
        case WidgetButton::Right:
            widgetButton = new WidgetButton(0,Qt::Vertical,thumbnailWidth, thumbnailHeight);
            break;
        case WidgetButton::Top:
        case WidgetButton::Bottom:
        default:
            widgetButton = new WidgetButton(0,Qt::Horizontal,thumbnailWidth, thumbnailHeight);
            break;
    }

    //widgetButton->setFixedSize(thumbnailWidth, thumbnailHeight);
    widgetButton->setIconAlternative(icon, useOnlyIcon);

    
    // show the widget if its the only one
    if(widgets.size() == 0)
    {
        widgetButton->setWidget(name, widget, false);
        widgets.insert(name, widgetButton);
        mid->addWidget(widget);
        currentWidget = widgetButton;
    }
    else
    {
        widgetButton->setWidget(name, widget);
    }

    widgets.insert(name, widgetButton);
    //left->addWidget(widgetButton);

		
    if(position != WidgetButton::Default){
        switch(position){
	    case WidgetButton::Left:
                left->addWidget(widgetButton);
                break;
	    case WidgetButton::Right:
                right->addWidget(widgetButton);
                break;
	    case WidgetButton::Top:
                top->addWidget(widgetButton);
                break;
	    case WidgetButton::Bottom:
                bottom->addWidget(widgetButton);
                break;
        }
    }else{
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
        }
    }
				
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
    if(sender == 0){
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

    mid->removeWidget(currentWidget->getWidget());
    mid->addWidget(widget);
    
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

