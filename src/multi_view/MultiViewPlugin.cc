
#include "MultiViewPlugin.h"
#include "MultiViewWidget.h"
//#include <QtPlugin>


MultiViewPlugin::MultiViewPlugin(QObject* parent) : QObject(parent)
{
    initialized = false;
}

MultiViewPlugin::~MultiViewPlugin()
{

}

bool MultiViewPlugin::isContainer() const
{
    return false;
}

bool MultiViewPlugin::isInitialized() const
{
    return initialized;
}

QIcon MultiViewPlugin::icon() const
{
    return QIcon(":/multi_view/icon.png");
}

QString MultiViewPlugin::group() const
{
    return "Rock-Robotics";
}

QString MultiViewPlugin::includeFile() const
{
    return "MultiViewWidget.h";
}

QString MultiViewPlugin::name() const
{
    return "MultiViewWidget";
}

QString MultiViewPlugin::toolTip() const
{
    return "Widget for displaying multiple widgets";
}

QString MultiViewPlugin::whatsThis() const
{
    return "Container widget for other widgets";
}

void MultiViewPlugin::initialize(QDesignerFormEditorInterface * core)
{
    if (initialized)
    {
        return;
    }
    // potential initialization here
    initialized = true;
}

QWidget* MultiViewPlugin::createWidget(QWidget* parent)
{
    return new MultiViewWidget(parent);
}

QString MultiViewPlugin::domXml() const
{
    return "<widget class=\"MultiViewWidget\" name=\"MultiViewWidget\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>400</width>\n"
        "   <height>300</height>\n"
        "  </rect>\n"
        " </property>\n"
        " <property name=\"toolTip\" >\n"
        "  <string>MultiView</string>\n"
        " </property>\n"
        " <property name=\"whatsThis\" >\n"
        "  <string>Displays multiple widgets in a container.</string>\n"
        " </property>\n"
        "</widget>\n";

}
