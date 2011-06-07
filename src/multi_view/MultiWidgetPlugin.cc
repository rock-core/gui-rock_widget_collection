#include "MultiWidgetPlugin.h"
#include "MultiWidget.h"


 MultiWidgetPlugin::MultiWidgetPlugin(QObject *parent)
     : QObject(parent)
 {
     initialized = false;
 }

 MultiWidgetPlugin::~MultiWidgetPlugin()
 {
 }

 void MultiWidgetPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool MultiWidgetPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *MultiWidgetPlugin::createWidget(QWidget *parent)
 {
     return new MultiWidget(parent);
 }

 QString MultiWidgetPlugin::name() const
 {
     return "MultiWidget";
 }

 QString MultiWidgetPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon MultiWidgetPlugin::icon() const
 {
     return QIcon("");
 }

 QString MultiWidgetPlugin::toolTip() const
 {
     return "Widget for displaying multiple Sub-Widget";
 }

 QString MultiWidgetPlugin::whatsThis() const
 {
     return "widget for displaying multi subwididets";
 }

 bool MultiWidgetPlugin::isContainer() const
 {
     return true;
 }

 QString MultiWidgetPlugin::domXml() const
 {
     return "<widget class=\"MultiWidget\" name=\"multiwidget\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>320</width>\n"
            "   <height>240</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
            "  <string>MultiWidget</string>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string>Widget for displaying Subframes.</string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString MultiWidgetPlugin::includeFile() const
 {
     return "multi_view/MultiWidget.h";
 }

