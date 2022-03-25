#ifndef TIMELINEPLUGIN_H
#define	TIMELINEPLUGIN_H

#include <QtGui/QtGui>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#include "Timeline.h"

class TimelinePlugin : public QObject , public QDesignerCustomWidgetInterface
{
  Q_OBJECT

 public:
   TimelinePlugin(QObject *parent = 0);
   virtual ~TimelinePlugin();

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

#endif	/* TIMELINEPLUGIN_H */

