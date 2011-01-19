#include "PlotWidgetPlugin.h"
#include <QtPlugin>


PlotWidgetPlugin::PlotWidgetPlugin(QObject* parent) : QObject(parent)
{
    initialized = false;
}

PlotWidgetPlugin::~PlotWidgetPlugin()
{

}

bool PlotWidgetPlugin::isContainer() const
{
    return false;
}

bool PlotWidgetPlugin::isInitialized() const
{
    return initialized;
}

QIcon PlotWidgetPlugin::icon() const
{
    return QIcon();
}

QString PlotWidgetPlugin::group() const
{
    return "Rock-Robotics";
}

QString PlotWidgetPlugin::includeFile() const
{
    return "PlotWidget.h";
}

QString PlotWidgetPlugin::name() const
{
    return "PlotWidget";
}

QString PlotWidgetPlugin::toolTip() const
{
    return "Widget for plotting purposes";
}

QString PlotWidgetPlugin::whatsThis() const
{
    return "Widget for plotting 2 dimensional data";
}

void PlotWidgetPlugin::initialize(QDesignerFormEditorInterface * core)
{
    if (initialized)
    {
        return;
    }
    // potential initialization here
    initialized = true;
}

QWidget* PlotWidgetPlugin::createWidget(QWidget* parent)
{
    return new PlotWidget(parent);
}

QString PlotWidgetPlugin::domXml() const
{
    return "<widget class=\"PlotWidget\" name=\"PlotWidget\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>400</width>\n"
        "   <height>300</height>\n"
        "  </rect>\n"
        " </property>\n"
        " <property name=\"toolTip\" >\n"
        "  <string>Plot</string>\n"
        " </property>\n"
        " <property name=\"whatsThis\" >\n"
        "  <string>Displays two dimensional plots.</string>\n"
        " </property>\n"
        "</widget>\n";

}

Q_EXPORT_PLUGIN2(PlotWidgetPlugin, PlotWidgetPlugin)
