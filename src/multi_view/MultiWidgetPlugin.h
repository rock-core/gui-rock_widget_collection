
#ifndef MULTIWIDGETPLUGIN_H
#define MULTIWIDGETPLUGIN_H 

#include <QtWidgets>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class MultiWidgetPlugin : public QObject , public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)
  
 public:
   MultiWidgetPlugin(QObject *parent = 0);
   virtual ~MultiWidgetPlugin();

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
