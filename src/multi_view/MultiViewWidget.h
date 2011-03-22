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

#include <QtDesigner/QDesignerExportWidget>

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
class QDESIGNER_WIDGET_EXPORT MultiViewWidget : public QWidget
{

    /**
     * Enum to determine the position of the thumbnails
     */
    enum Position{Left, Top, Right, Bottom};

    Q_OBJECT
    Q_CLASSINFO("Author", "Bjoern Lueck")

public:
    /**
     * Constructor
     * @param parent parentWidget will be given to QWidgets constructor
     */
    MultiViewWidget(QWidget* parent);

    /**
     * Destructor
     * @todo implement correctly
     */
    virtual ~MultiViewWidget();

    void childEvent(QChildEvent* event);

public slots:

    /**
     * Adds a widget and a thumbnail to the widget
     * @param name the name given to the widget, must be unique.
     * @param widget teh widget to add
     * @param icon an icon as alternative for text or the wisget itself
     * @param useOnlyIcon if only the icon and or text shall be displayed as thumbnail and NOT the widget
     */
    void addWidget(const QString &name, QWidget* widget, const QIcon &icon=QIcon(), bool useOnlyIcon=false);

    /**
     * Return sthe widget for the given name
     * @param name the name of the widget to return given when added via addWidget
     * @return the widget which was added with the given name
     * @todo implement correctly
     */
    QWidget* getWidget(const QString &name);

    /**
     * Removes the widget with the given name.
     * This will DELETE the widget that was added with that name!!
     * @param name the anme of the widget to remove
     */
    void deleteWidget(const QString &name);

    /**
     * Called when a thumbnail was clicked
     */
    void widgetClicked();

    /**
     * Sets the position of the thumbnails. The given int corresponds to
     * teh Position enum. int is taken for ruby mainly.
     * @param position 0=left, 1=top, 2=right, 3=bottom
     */
    void setThumbnailPosition(int position);

    /**
     * Sets the size of teh thumbnail widgets. Defaults the thumbnails to
     * 150x150.
     * @param width teh width of the thumbnails, defaults to 150
     * @param height the height of the thumbnails defaults to 150
     */
    void setThumbnailSize(int width=150, int height=150);

    /**
     * Simple Test method. This method cycles all widgets added and calls the
     * doTesting method slot on the widget if there is one, or nothing if there isn't.
     */
    void doTesting();


protected:

    /**
     * Adds the menu to the widget, currently its only use is to have
     * a testing environment
     */
    void addMenu();

    /** All thumbnails and widgets added*/
    QHash<QString, WidgetButton*> widgets;
    /** Menu Bar*/
    QMenuBar menuBar;
    /** Main layout*/
    QGridLayout layout;
    /**Layout if Top or Bottom position is used*/
    QHBoxLayout* upperLayout;
    /** Layout if Left or Right position is used*/
    QVBoxLayout* vertLayout;
    /** File Menu*/
    QMenu fileMenu;
    /** The widget currently displayed in the center part of the widget*/
    QWidget* currentWidget;
    /** A widget for layouting purposes*/
    QWidget layoutWidget;
    /** The current Thumbnail position*/
    Position position;
    /** The width of thumbnails*/
    int thumbnailWidth;
    /** The height of thumbnails*/
    int thumbnailHeight;
    /** Action to test the widget, calls doTesting*/
    QAction testingAction;
    /** Widget for Layouting purposes*/
    QWidget* pseudoWidget;

    bool initialized;
    int currentWidgetIndex;
    bool clicking;
};

#endif	/* MULIVIEWWIDGET_H */

