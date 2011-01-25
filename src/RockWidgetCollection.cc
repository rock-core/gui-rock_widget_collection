#include "RockWidgetCollection.h"

Q_EXPORT_PLUGIN2(RockWidgetCollection, RockWidgetCollection)

RockWidgetCollection::RockWidgetCollection(QObject *parent)
       : QObject(parent)
{
   widgets.append(new ImageViewPlugin(this));
   widgets.append(new SonarViewPlugin(this));
   widgets.append(new PlotWidgetPlugin(this));
   widgets.append(new ArtificialHorizonPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> RockWidgetCollection::customWidgets() const
{
   return widgets;
}

