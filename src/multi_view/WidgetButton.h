/* 
 * File:   WidgetButton.h
 * Author: blueck
 *
 * Created on 11. Februar 2011, 12:01
 */

#ifndef WIDGETBUTTON_H
#define	WIDGETBUTTON_H

#include <QPushButton>
#include <QHBoxLayout>

class WidgetButton : public QPushButton
{

    Q_OBJECT

public:
    WidgetButton();
    virtual ~WidgetButton();

public slots:
    void setWidget(const QString &name, QWidget* widget, bool shown=true);
    void showWidget(bool shown);
    QWidget* getWidget();
    QString getWidgetName();
    
protected:
    QWidget* mainWidget;
    QString name;
    QHBoxLayout layout;

};

#endif	/* WIDGETBUTTON_H */

