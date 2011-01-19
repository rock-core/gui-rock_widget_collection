#ifndef ROCKWIDGETCOLLECTION_H
#define ROCKWIDGETCOLLECTION_H

#include "image_view/ImageViewPlugin.h"
#include "sonar_view/SonarViewPlugin.h"

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
