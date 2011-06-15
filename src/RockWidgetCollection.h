#ifndef ROCKWIDGETCOLLECTION_H
#define ROCKWIDGETCOLLECTION_H

#include "image_view/ImageViewPlugin.h"
#include "sonar_view/SonarViewPlugin.h"
#include "range_view/RangeViewPlugin.h"
#include "plotting/PlotWidgetPlugin.h"
#include "artificial_horizon/artificialhorizonplugin.h"
#include "artificial_horizon/compassplugin.h"
#include "artificial_horizon/orientationplugin.h"
#include "multi_view/MultiViewPlugin.h"
#include "multi_view/MultiWidgetPlugin.h"

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
