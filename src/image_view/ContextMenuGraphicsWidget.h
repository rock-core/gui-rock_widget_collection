#ifndef CONTEXTMENUGRAPHICSWIDGET_H
#define CONTEXTMENUGRAPHICSWIDGET_H

#include <QGraphicsWidget>

class ContextMenuGraphicsWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    ContextMenuGraphicsWidget(QGraphicsItem * parent = 0);
    virtual ~ContextMenuGraphicsWidget();

signals:
    void contextMenuRequest(QPoint);
    
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // CONTEXTMENUGRAPHICSWIDGET_H
