/* 
 * File:   MuliViewWidget.h
 * Author: blueck
 *
 * Created on 10. Februar 2011, 14:14
 */

#ifndef MULIVIEWWIDGET_H
#define	MULIVIEWWIDGET_H

#include <QWidget>
#include <QHash>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMenuBar>
#include <QMenu>

#include <iostream>


#include "WidgetButton.h"

class MultiViewWidget : public QWidget
{

    enum Position{Left, Top, Right, Bottom};

    Q_OBJECT

public:
    MultiViewWidget(QWidget* parent);
    virtual ~MultiViewWidget();

    void addMenu();
public slots:
    void addWidget(const QString &name, QWidget* widget);
    QWidget* getWidget(const QString &name);
    void deleteWidget(const QString &name);
    void widgetClicked();
    void setThumbnailPosition(int position);
    void setThumbnailSize(int width=150, int height=150);

protected:
    QHash<QString, WidgetButton*> widgets;
    QMenuBar menuBar;
    QGridLayout layout;
    QHBoxLayout* upperLayout;
    QVBoxLayout* vertLayout;
    QMenu fileMenu;
    QWidget* currentWidget;
    QWidget layoutWidget;
    Position position;
    int thumbnailWidth;
    int thumbnailHeight;
};

#endif	/* MULIVIEWWIDGET_H */

