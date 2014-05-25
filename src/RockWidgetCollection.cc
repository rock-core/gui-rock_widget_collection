#include "RockWidgetCollection.h"
#include "multi_view/MultiViewWidget.h"
#include "image_view_old/ImageViewOldPlugin.h"
#include "sonar_view/SonarViewPlugin.h"
#include "range_view/RangeViewPlugin.h"
#include "artificial_horizon/artificialhorizonplugin.h"
#include "artificial_horizon/compassplugin.h"
#include "artificial_horizon/orientationplugin.h"
#include "multi_view/MultiViewPlugin.h"
#include "multi_view/MultiWidgetPlugin.h"
#include "virtual_joystick/VirtualJoystickPlugin.h"
#include "generic_widgets/RockSliderPlugin.h"
#include "plot2d/Plot2dPlugin.h"
#include "2dvis/WaterwallDisplayPlugin.h"
#include "timeline/TimelinePlugin.h"
#include "image_view/ImageViewPlugin.h"
#include "progress_indicator/ProgressIndicatorPlugin.h"
#include "sonar_widget/SonarWidgetPlugin.h"

#ifdef USE_VTK
#include "vtk/sonar_display/SonarDisplayPlugin.h"
#include "vtk/vectorfield3D/vectorfield3DPlugin.h"
#endif

Q_EXPORT_PLUGIN2(RockWidgetCollection, RockWidgetCollection)

RockWidgetCollection::RockWidgetCollection(QObject *parent)
       : QObject(parent)
{
// *** Please sort these alphabetically after the displayed name! ***
   widgets.append(new ArtificialHorizonPlugin(this));
   widgets.append(new CompassPlugin(this));
   widgets.append(new ImageViewPlugin(this));
   widgets.append(new ImageViewOldPlugin(this));
   widgets.append(new MultiViewPlugin(this));
   widgets.append(new MultiWidgetPlugin(this));
   widgets.append(new OrientationPlugin(this));
   widgets.append(new Plot2dPlugin(this));
   widgets.append(new ProgressIndicatorPlugin(this));
   widgets.append(new RangeViewPlugin(this));
   widgets.append(new RockSliderPlugin(this));
#ifdef USE_VTK
   widgets.append(new SonarDisplayPlugin(this));
#endif
   widgets.append(new SonarViewPlugin(this));
   widgets.append(new TimelinePlugin(this));
#ifdef USE_VTK
   widgets.append(new Vectorfield3DPlugin(this));
#endif
   widgets.append(new VirtualJoystickPlugin(this));
   widgets.append(new WaterfallDisplayPlugin(this));
   widgets.append(new SonarWidgetPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> RockWidgetCollection::customWidgets() const
{
   return widgets;
}

