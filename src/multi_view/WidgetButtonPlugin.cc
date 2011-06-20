#include "WidgetButtonPlugin.h"
#include "WidgetButton.h"


 WidgetButtonPlugin::WidgetButtonPlugin(QObject *parent)
     : QObject(parent)
 {
     initialized = false;
 }

 WidgetButtonPlugin::~WidgetButtonPlugin()
 {
 }

 void WidgetButtonPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool WidgetButtonPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *WidgetButtonPlugin::createWidget(QWidget *parent)
 {
     return new WidgetButton(parent);
 }

 QString WidgetButtonPlugin::name() const
 {
     return "WidgetButton";
 }

 QString WidgetButtonPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon WidgetButtonPlugin::icon() const
 {
     return QIcon("");
 }

 QString WidgetButtonPlugin::toolTip() const
 {
     return "Widget for displaying multiple Sub-Widget";
 }

 QString WidgetButtonPlugin::whatsThis() const
 {
     return "widget for displaying multi subwididets";
 }

 bool WidgetButtonPlugin::isContainer() const
 {
     return false;
 }

 QString WidgetButtonPlugin::domXml() const
 {
     return "<widget class=\"WidgetButton\" name=\"WidgetButton\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>320</width>\n"
            "   <height>240</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
            "  <string>WidgetButton</string>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string>Widget for displaying Subframes.</string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString WidgetButtonPlugin::includeFile() const
 {
     return "multi_view/WidgetButton.h";
 }

