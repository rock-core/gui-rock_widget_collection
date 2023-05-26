#ifndef PROGRESSINDICATORPLUGIN_H
#define PROGRESSINDICATORPLUGIN_H

#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#else
#include <QtDesigner/QDesignerCustomWidgetInterface>
#endif

class ProgressIndicatorPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    ProgressIndicatorPlugin(QObject *parent = 0);
    virtual ~ProgressIndicatorPlugin();

    bool isContainer() const;
    bool isInitialized() const;
    QIcon icon() const;
    QString domXml() const;
    QString group() const;
    QString includeFile() const;
    QString name() const;
    QString toolTip() const;
    QString whatsThis() const;
    QWidget* createWidget(QWidget *parent);
    void initialize(QDesignerFormEditorInterface *core);

private:
    bool initialized; 
};

#endif /* PROGRESSINDICATORPLUGIN_H */  
