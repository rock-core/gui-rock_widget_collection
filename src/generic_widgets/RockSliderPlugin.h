
#ifndef _ROCKSLIDERPLUGIN_H
#define _ROCKSLIDERPLUGIN_H

#include <QtGui>
#include <QtDesigner/QDesignerCustomWidgetInterface>

class RockSliderPlugin : public QObject , public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)

 public:
   RockSliderPlugin(QObject *parent = 0);
   virtual ~RockSliderPlugin();

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
