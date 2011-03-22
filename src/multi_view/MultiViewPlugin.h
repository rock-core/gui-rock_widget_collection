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
#include <QtDesigner/QDesignerFormEditorInterface>
#include <QtDesigner/QExtensionManager>
#include <QtDesigner/QExtensionFactory>

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

private:
    bool initialized;
    MultiViewWidget* widget;

};

#endif	/* MULTIVIEWPLUGIN_H */

