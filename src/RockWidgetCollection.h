#ifndef ROCKWIDGETCOLLECTION_H
#define ROCKWIDGETCOLLECTION_H

#include <QtDesigner/QtDesigner>
#include <QtCore/qplugin.h>

class RockWidgetCollection: public QObject, public QDesignerCustomWidgetCollectionInterface
{
   Q_OBJECT
#if QT_VERSION >= 0x050000
   Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
#endif
   Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
   RockWidgetCollection(QObject *parent = 0);
   virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
   QList<QDesignerCustomWidgetInterface*> widgets;
};

#endif
