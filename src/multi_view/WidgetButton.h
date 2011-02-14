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
#include <QTimer>
#include <QIcon>
#include <QLabel>

#include <iostream>

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
    void widgetUpdate(int type=0);
    void changePalette();
    void setIconAlternative(const QIcon &icon, bool isAlternative=false);
    
protected:
    QWidget* mainWidget;
    QString name;
    QHBoxLayout layout;
    QPalette defaultPalette;
    QPalette redPalette;
    QTimer paletteTimer;
    QIcon icon;
    bool isAlternative;
};

#endif	/* WIDGETBUTTON_H */

