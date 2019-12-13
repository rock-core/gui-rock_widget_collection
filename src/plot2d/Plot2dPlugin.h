
#ifndef PLOT2DPLUGIN_H
#define PLOT2DPLUGIN_H 

#include <QtGui/QtGui>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class Plot2dPlugin : public QObject , public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)

 public:
   Plot2dPlugin(QObject *parent = 0);
   virtual ~Plot2dPlugin();

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
