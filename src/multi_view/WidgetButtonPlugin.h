
#ifndef WIDGETBUTTONPLUGIN_H
#define WIDGETBUTTONPLUGIN_H 

#include <QtGui/QtGui>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class WidgetButtonPlugin : public QObject , public QDesignerCustomWidgetInterface
{
  Q_OBJECT

 public:
   WidgetButtonPlugin(QObject *parent = 0);
   virtual ~WidgetButtonPlugin();

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
