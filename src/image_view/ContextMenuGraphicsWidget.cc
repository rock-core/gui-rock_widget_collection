#include "ContextMenuGraphicsWidget.h"
#include <QGraphicsSceneContextMenuEvent>

ContextMenuGraphicsWidget::ContextMenuGraphicsWidget(QGraphicsItem * parent)
    : QGraphicsWidget(parent, 0)
{
    
}

ContextMenuGraphicsWidget::~ContextMenuGraphicsWidget()
{

}

void ContextMenuGraphicsWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    emit contextMenuRequest(event->screenPos());
}

