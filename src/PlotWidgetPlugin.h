#ifndef PLOTWIDGETPLUGIN_H
#define	PLOTWIDGETPLUGIN_H

#include "PlotWidget.h"

#include <QDesignerCustomWidgetInterface>

class PlotWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{

    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    PlotWidgetPlugin(QObject* parent=0);
    virtual ~PlotWidgetPlugin();
    // --> QT Designer specific method

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
};

#endif	/* PLOTWIDGETPLUGIN_H */
