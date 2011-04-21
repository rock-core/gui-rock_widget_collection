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
#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtDesigner/QDesignerFormWindowInterface>

#include <vector>

/**
 * Simple qt designer plugin class. Only thing to note that the multi view widget is used
 * as a container widget.
 */
class MultiViewPlugin : public QObject, public QDesignerCustomWidgetInterface
{

    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    MultiViewPlugin(QObject* parent);
    virtual ~MultiViewPlugin();
    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget *createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

public slots:
    void manageWidget(QWidget* widget);
    void selectionChanged();
    void widgetUnmanaged(QWidget* widget);
    void widgetRemoved(QWidget* widget);
    void widgetManaged(QWidget* widget);
    void activeFormWindowChanged(QDesignerFormWindowInterface* formWindow);

private:
    bool initialized;
    MultiViewWidget* widget;
    QDesignerFormEditorInterface* formInterface;
    std::vector<QWidget*> lastWidgets;
    bool designerMode;

};

#endif	/* MULTIVIEWPLUGIN_H */

