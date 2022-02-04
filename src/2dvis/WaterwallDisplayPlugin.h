
#ifndef WATERFALLDISPLAYPLUGIN_H
#define WATERFALLDISPLAYPLUGIN_H

#include <QtGui/QtGui>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class WaterfallDisplayPlugin : public QObject , public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)

 public:
   WaterfallDisplayPlugin(QObject *parent = 0);
   virtual ~WaterfallDisplayPlugin();

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
