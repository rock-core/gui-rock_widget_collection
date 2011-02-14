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
#include "plotting/PlotWidget.h"

/**
 * Widget which displays other widgets. All added widgets will be displayed as thumbnails
 * either left, right, on top or below one widget. Thumbnail Widgets can be clicked to show
 * in the main potion of the screen. S one widget will be the "active" widget which takes most of the
 * sapce within the widget all other widgets will be displayed as small widget in one of
 * the positions as mentioned above.<br>
 * If there are any changes to an added widget, the thumnail will blink in red. This
 * only works if the added widget has a notifyUpdate(int) signal. The signal will automatically
 * be taken into account via the qt meta object infrastructure. The currently selected widget
 * will be displayed with a name and is colorized in green to indicate selection.<br>
 * The size and orientation of the thumbnails can be configurede via the methods
 * setThumbnailPosition(int) and setThumnailSize(int, int).<br>
 * If the widget is too time consuming or can not be properly displayed when being small
 * one can either only show the widgets name or alternatively an icon.<br>
 * The widget will try to resize teh viewable center widget as large as it can. If this is not the desired
 *  behaviour setting a fixed or maximum size will work properly, so the widget will not stretch to
 * more than the given sizes.
 *
 * @author Bjoern Lueck <blueck@dfki.de>
 * @version 0.1
 */
class MultiViewWidget : public QWidget
{

    enum Position{Left, Top, Right, Bottom};

    Q_OBJECT

public:
    MultiViewWidget(QWidget* parent);
    virtual ~MultiViewWidget();

    void addMenu();
public slots:
    void addWidget(const QString &name, QWidget* widget, const QIcon &icon=QIcon(), bool useOnlyIcon=false);
    QWidget* getWidget(const QString &name);
    void deleteWidget(const QString &name);
    void widgetClicked();
    void setThumbnailPosition(int position);
    void setThumbnailSize(int width=150, int height=150);
    void doTesting();

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
    QAction testingAction;
    QWidget* pseudoWidget;
};

#endif	/* MULIVIEWWIDGET_H */

