
#ifndef _ORIENTATIONPLUGIN_H
#define _ORIENTATIONPLUGIN_H

#include <QtGui>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class OrientationPlugin : public QObject , public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)

 public:
   OrientationPlugin(QObject *parent = 0);
   virtual ~OrientationPlugin();

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
