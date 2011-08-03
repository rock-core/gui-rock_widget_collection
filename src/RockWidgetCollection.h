#ifndef ROCKWIDGETCOLLECTION_H
#define ROCKWIDGETCOLLECTION_H

#include <QtDesigner/QtDesigner>
#include <QtCore/qplugin.h>

class RockWidgetCollection: public QObject, public QDesignerCustomWidgetCollectionInterface
{
   Q_OBJECT
   Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
   RockWidgetCollection(QObject *parent = 0);
   virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
   QList<QDesignerCustomWidgetInterface*> widgets;
};

#endif
