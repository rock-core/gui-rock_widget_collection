#ifndef GSTIMAGEVIEWPLUGIN_H
#define GSTIMAGEVIEWPLUGIN_H

#include <QtGui>
#include <QtDesigner/QDesignerCustomWidgetInterface>

class GstImageViewPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    GstImageViewPlugin(QObject *parent = 0);
    virtual ~GstImageViewPlugin();

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

#endif /* GSTIMAGEVIEWPLUGIN_H */  
