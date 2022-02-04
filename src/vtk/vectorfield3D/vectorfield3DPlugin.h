#ifndef VECTORFIELD3DPLUGIN_H
#define VECTORFIELD3DPLUGIN_H

#include <QtGui>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class Vectorfield3DPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    Vectorfield3DPlugin(QObject *parent = 0);
    virtual ~Vectorfield3DPlugin();

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

#endif /* VECTORFIELD3DPLUGIN_H */  
