#include "TimelinePlugin.h"

TimelinePlugin::TimelinePlugin(QObject *parent) 
    : QObject(parent)
{
    initialized = false;
}

TimelinePlugin::~TimelinePlugin()
{
}

bool TimelinePlugin::isContainer() const
{
    return false;
}

bool TimelinePlugin::isInitialized() const
{
    return initialized;
}

QIcon TimelinePlugin::icon() const
{
    // TODO
}

QString TimelinePlugin::domXml() const
{
    return "<ui language=\"c++\">\n"
        " <widget class=\"Timeline\" name=\"timeline\">\n"
//        "  <property name=\"geometry\">\n"
//        "   <rect>\n"
//        "    <x>0</x>\n"
//        "    <y>0</y>\n"
//        "    <width>100</width>\n"
//        "    <height>100</height>\n"
//        "   </rect>\n"
//        "  </property>\n"
//        "  <property name=\"toolTip\" >\n"
//        "   <string>The current time</string>\n"
//        "  </property>\n"
//        "  <property name=\"whatsThis\" >\n"
//        "   <string>The analog clock widget displays the current time.</string>\n"
//        "  </property>\n"
        " </widget>\n"
        "</ui>\n";
}

QString TimelinePlugin::group() const {
    return "Rock-Robotics";
}

QString TimelinePlugin::includeFile() const {
    
}

QString TimelinePlugin::name() const {
    return "Timeline";
}

QString TimelinePlugin::toolTip() const {
    return whatsThis();
}

QString TimelinePlugin::whatsThis() const
{
    return "Slider Widget with section selection and marker support";
}

QWidget* TimelinePlugin::createWidget(QWidget *parent)
{
    return new Timeline(parent);
}

void TimelinePlugin::initialize(QDesignerFormEditorInterface *core)
{
     if (initialized)
         return;
     initialized = true;
}