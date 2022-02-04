/* 
 * File:   MultiViewPlugin.h
 * Author: blueck
 *
 * Created on 15. Februar 2011, 15:14
 */

#ifndef MULTIVIEWPLUGIN_H
#define	MULTIVIEWPLUGIN_H

#include "MultiViewWidget.h"

#include <QtGui>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#include <QtDesigner/QDesignerFormWindowInterface>

#include <vector>

/**
 * Simple qt designer plugin class. Only thing to note that the multi view widget is used
 * as a container widget. It handles the adding of widgets for the MultiViewWidget
 * in a QTDesigner specific way. No preview will be shown as QT Designer can
 * not handle widgets which will be taken care of by the MultiViewWidget.
 */
class MultiViewPlugin : public QObject, public QDesignerCustomWidgetInterface
{

    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    /**
     * Simple constructor
     * @param parent parent widget
     */
    MultiViewPlugin(QObject* parent);

    /**
     * Standard Destructor
     */
    virtual ~MultiViewPlugin();

    /**
     * Returns whether this is a container widget
     * @return will always return true
     */
    bool isContainer() const;

    /**
     * Returns whether the plugin is initialized
     * @return if the plugin is initialized
     */
    bool isInitialized() const;

    /**
     * Returns teh icon of the plugin
     * @return the icon of the plugin
     */
    QIcon icon() const;

    /**
     * Returns an XML defining the plugins internals
     * @return an xml string
     */
    QString domXml() const;

    /**
     * Returns a group name
     * @return a group name
     */
    QString group() const;

    /**
     * Returns the include File
     * @return the include file
     */
    QString includeFile() const;

    /**
     * Returns the name
     * @return the name
     */
    QString name() const;

    /**
     * Returns the tooltip
     * @return the tooltip
     */
    QString toolTip() const;

    /**
     * Returns a whats this string
     * @return teh whatsthis string
     */
    QString whatsThis() const;

    /**
     * Creates the widget which the plugin handles
     * @param parent parent widget
     * @return the widget which the plugin shall handle
     */
    QWidget *createWidget(QWidget *parent);

    /**
     * Initializes the plugin
     * @param core Interface given by QT Designer
     */
    void initialize(QDesignerFormEditorInterface *core);

public slots:

    /**
     * Called when a widget was added so it will be managed correctly in
     * the active form
     * @param widget the widgfet which shall be managed
     */
    void manageWidget(QWidget* widget);

    /**
     * Called when a preview Button was selected to imitate clicks, which will
     * normally be caught by designer to change the buttons name
     */
    void selectionChanged();

    /**
     * Called when the active Form window was changed. Used when loading a ui file
     * to know when the designer is ready to add teh already added widgets
     * @param formWindow The active form window
     */
    void activeFormWindowChanged(QDesignerFormWindowInterface* formWindow);

private:
    /** If the plugin is initialized*/
    bool initialized;
    /** The widget to be handled by the plugin*/
    MultiViewWidget* widget;
    /** The formInterface given by the designer*/
    QDesignerFormEditorInterface* formInterface;
    /** Saved widgets when loading via an ui file*/
    std::vector<QWidget*> lastWidgets;
    /** whether designer mode is enabled*/
    bool designerMode;

};

#endif	/* MULTIVIEWPLUGIN_H */

