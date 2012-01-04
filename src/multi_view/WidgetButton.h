/* 
 * File:   WidgetButton.h
 * Author: blueck
 *
 * Created on 11. Februar 2011, 12:01
 */

#ifndef WIDGETBUTTON_H
#define	WIDGETBUTTON_H

#include <QPushButton>
#include <QBoxLayout>
#include <QTimer>
#include <QIcon>
#include <QLabel>
#include <QMetaType>
#include <iostream>

/**
 * QPushButton which can display another widget or a text/icon alternatively.
 * The widget can be showable and if not shown an icon or text will be displayed.
 * when configured without widget display, only the icon or text will be shown.
 * <p>
 * All widgets which have a notifyUpdate(int) method will blink in red if the method
 * is called and the widget is currently shown. When clicked the widget will stop
 * blinking. This can be used to indicate that something has changed on the widget.
 *
 * @author Bjoern Lueck
 * @version 0.1
 */
class WidgetButton : public QPushButton
{
    Q_OBJECT
		Q_ENUMS(Position)
	  Q_PROPERTY(Position position READ getPosition WRITE setPosition)

public:
    /**
     * Enum to determine the position of the thumbnails
     */
    enum Position{Default=0, Left, Top, Right, Bottom};
		

    /**
     * Constructor
     */
    WidgetButton(QWidget *widget=0, Qt::Orientations dir=Qt::Horizontal, int width=150,int height=150);

    /**
     * Destructor
     */
    virtual ~WidgetButton();

 		const Position getPosition() const;  
		const Position getPosition(){return position;} 
		void setPosition(const Position pos){position = pos;}

public slots:

    /**
     * Sets the widget for the button
     * The given name will be used if the widget is not shown
     *
     * @param name the name to show when the widget is not shown
     * @param widget the widget to be shown
     * @param shown true if the widget shall be shown on the button immediately, false otherwise, defaults to true
     */
    void setWidget(const QString &name, QWidget* widget, bool shown=true);

    /**
     * Show or hides the widget
     * @param shown true if the widget shall be shown
     */
    void showWidget(bool shown);

    /**
     * Returns the widget
     * @return the widget on the button
     */
    QWidget* getWidget();

    /**
     * Returns the name of the widget
     * @return teh anme of the widget
     */
    QString getWidgetName();

    /**
     * Slot called when an notifyUpdate from the widget was signaled.
     * @param type the type, currently ignored, defaults to 0
     */
    void widgetUpdate(int type=0);

    /**
     * Changes the palette of the widget from red to the default scheme
     * and the other way around, is called by a timer automatically
     */
    void changePalette();

    /**
     * Sets an alternative Icon which can be used instead of the widget
     * (e.g. if the widget is badly visible if sized down). If the icon is not used
     * as alternative to the widget it is displayed instead of teh text if the widget is not shown
     * @param icon the icon alternative
     * @param isAlternative if the icon is the alternative to the widget or to the text, defaults to false
     */
    void setIconAlternative(const QIcon &icon, bool isAlternative=false);

    void printStatus();

		void setActive(bool active);
    

		void corrcetName();
protected:
    /** The main widget*/
    QWidget* mainWidget;
	

		/**Position for the button*/
		Position position;

    /** The name of the widget*/
    QString name;
    /** The layout of the button*/
    QBoxLayout *layout;
    /** The default palette of teh button*/
    QPalette defaultPalette;
    /** A red palette*/
    QPalette redPalette;
    /** Timer for blinking red palette usage*/
    QTimer paletteTimer;
    /** Icon used as alternative*/
    QIcon icon;
    /** If the icon is an alternative to the widget or only the text*/
    bool isAlternative;
		/** True if this Widget is Active (not minimized) */
		bool isActive;

    Qt::Orientations dir; 
};
		
Q_DECLARE_METATYPE(WidgetButton::Position)

#endif	/* WIDGETBUTTON_H */

