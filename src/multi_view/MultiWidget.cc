#include "MultiWidget.h"



MultiWidget::MultiWidget(QWidget *parent):
		PaintWidget(parent),
		isActive(false),
		hideWhenMinimized(false),
		wasHidden(false)
{
}       

MultiWidget::~MultiWidget()
{
}

void MultiWidget::setActive(bool b)
{
		isActive=b;
		emit activityChanged(b);

		for(QObjectList::const_iterator it = children().begin(); it != children().end(); it++){
			MultiWidget *child = dynamic_cast<MultiWidget*>(*it);
			if(child > 0){
				child->setActive(isActive);
			}
		}

		if(!isActive){
			if(hideWhenMinimized){
				wasHidden=isHidden();
				hide();
			}
		}else{
			//if(!wasHidden){
				show();
			//}
		}
}

QString MultiWidget::getMinimizedLabel()
{
	return minimizedLabel;
}

void MultiWidget::setMinimizedLabel(QString label)
{
	minimizedLabel = label;
}

 
void MultiWidget::childEvent(QChildEvent* event)
{
	QWidget::childEvent(event);
}

bool MultiWidget::event(QEvent* event)
{
	return QWidget::event(event);
}

bool MultiWidget::isHiddenWhenMinimized() const
{
	return hideWhenMinimized;
}

void MultiWidget::hideWhenMin(bool b)
{
	hideWhenMinimized = b;
}

