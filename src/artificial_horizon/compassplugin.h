
#ifndef _COMPASSPLUGIN_H
#define _COMPASSPLUGIN_H

#include <QtGui>
#include <QtDesigner/QDesignerCustomWidgetInterface>

class CompassPlugin : public QObject , public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)

 public:
   CompassPlugin(QObject *parent = 0);
   virtual ~CompassPlugin();

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
#endif
