
#ifndef SONARDISPLAYPLUGIN_H
#define SONARDISPLAYPLUGIN_H

#include <QtWidgets>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class SonarDisplayPlugin : public QObject , public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)
 public:
   SonarDisplayPlugin(QObject *parent = 0);
   virtual ~SonarDisplayPlugin();

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
