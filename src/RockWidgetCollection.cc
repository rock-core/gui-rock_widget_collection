#include "RockWidgetCollection.h"

Q_EXPORT_PLUGIN2(RockWidgetCollection, RockWidgetCollection)

RockWidgetCollection::RockWidgetCollection(QObject *parent)
       : QObject(parent)
{
   widgets.append(new ImageViewPlugin(this));
   widgets.append(new SonarViewPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> RockWidgetCollection::customWidgets() const
{
   return widgets;
}

