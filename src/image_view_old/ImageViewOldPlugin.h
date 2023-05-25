
#ifndef IMAGEVIEWOLDPLUGIN_H
#define IMAGEVIEWOLDPLUGIN_H

#include <QtGui/QtGui>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>

/*!
 * @deprecated This class is deprecated and will no longer be maintained. You should use the new ImageView widget.
 */
class ImageViewOldPlugin : public QObject , public QDesignerCustomWidgetInterface
{
  Q_OBJECT
  Q_INTERFACES(QDesignerCustomWidgetInterface)

 public:
   ImageViewOldPlugin(QObject *parent = 0);
   virtual ~ImageViewOldPlugin();

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
