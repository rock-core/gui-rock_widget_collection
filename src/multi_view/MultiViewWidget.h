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
#include <QTimer>
#include <QtDesigner/QDesignerExportWidget>

#include <iostream>


#include "WidgetButton.h"

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

    /**
     * Handles the events when a widget is added to or removed from the widget.
     * This will not do anything when used in standalone mode, but will be used
     * heavily when used in the qt designer.<br>
     * When adding a widget to or removing a widget from the MultiViewWidget from
     * within QTDesigner the childEvent method will map them to the
     * corresponding methods within the MultiViewWidget. Thus enabling the widget to
     * perform within the qt designer as container with teh desired functionality.<br>
     * Note the only thing which does not work is the thumbnail widgets. Widgets will not
     * be displayed as thumbnails within the designer, as the designer needs ownership
     * of those. In Designer's preview everything will act asexpected
     */
    void childEvent(QChildEvent* event);

    bool event(QEvent* event);

public slots:

		void updateView();

    /**
     * Adds a widget and a thumbnail to the widget
     * @param name the name given to the widget, must be unique.
     * @param widget teh widget to add
     * @param icon an icon as alternative for text or the wisget itself
     * @param useOnlyIcon if only the icon and or text shall be displayed as thumbnail and NOT the widget
     */
    void addWidget(const QString &name, QWidget* widget, const QIcon &icon=QIcon(), bool useOnlyIcon=false, int position = 0);

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
     * Sets the size of teh thumbnail widgets. Defaults the thumbnails to
     * 150x150.
     * @param width teh width of the thumbnails, defaults to 150
     * @param height the height of the thumbnails defaults to 150
     */
    void setThumbnailSize(int width=150, int height=150);

    /**
     * Returns the widget which is shown on the Button
     * @param widget  the widget for which the button shall be returned
     * @return the button for the given widget
     */
    WidgetButton* getButtonForWidget(QWidget* widget);

    /**
     * Returns all Buttons
     * @return  all Buttons as QList
     */
    QList<WidgetButton*> getAllWidgetButtons();

    /**
     * Sets whether designer mode is enabled (no small widgets)
     * NOTE: You should normally never have to call this manually
     * @param designerMode true if the designer is shown, false otherwise
     */
    void setDesignerMode(bool designerMode) { this->designerMode = designerMode; };

    /**
     * Returns whether designer mode is enabled
     * @return if designer mode is enabled
     */
    bool isDesignerMode() { return designerMode; };


		/** Fix the Status (names and view Style of all Widgets, 
		* this cannot be done during adding because rubyqt says 
		* all widets are QWidget
		 */
		void fixStatus();


    signals:
    /**
     * Emitted when a new widget is added
     * @param widget the widget which was added to the MultiWidget
     */
    void widgetButtonAdded(QWidget* widget);


protected:

    /**
     * Adds the menu to the widget, currently its only use is to have
     * a testing environment
     */

    /** All thumbnails and widgets added*/
    QHash<QString, WidgetButton*> widgets;
    /** Main layout*/
    QGridLayout layout;

    QHBoxLayout *top, *bottom;
    QVBoxLayout *left,*mid,*right;

    /** The widget currently displayed in the center part of the widget*/
    WidgetButton* currentWidget;
    
    /** The width of thumbnails*/
    int thumbnailWidth;
    /** The height of thumbnails*/
    int thumbnailHeight;
    /** Widget for Layouting purposes*/
    QWidget* pseudoWidget;

    /** Whether the view is already initialized*/
    bool initialized;
    /** The current index for use with qt designer. automatically gives numbers instead of names*/
    int currentWidgetIndex;
    /** if a widget was clicked and the widgetClicked method is currently processing*/
    bool clicking;
    /** Whether the designer mode is enabled*/
    bool designerMode;

    /**  Workaround to fix Ruby initialization, the widgets are not recognized well */
    QTimer timer;
};

#endif	/* MULIVIEWWIDGET_H */

