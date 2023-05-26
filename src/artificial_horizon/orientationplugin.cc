#include "orientationplugin.h"
#include "orientation.h"


 OrientationPlugin::OrientationPlugin(QObject *parent) : QObject(parent)
 {
     Q_INIT_RESOURCE(resources);

     initialized = false;
 }

 OrientationPlugin::~OrientationPlugin()
 {
 }

 void OrientationPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool OrientationPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *OrientationPlugin::createWidget(QWidget *parent)
 {
     return new OrientationView(parent);
 }

 QString OrientationPlugin::name() const
 {
     return "OrientationView";
 }

 QString OrientationPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon OrientationPlugin::icon() const
 {
     return QIcon(":/artificial_horizon/icon.png");
 }

 QString OrientationPlugin::toolTip() const
 {
     return "Widget for displaying an Orientation";
 }

 QString OrientationPlugin::whatsThis() const
 {
     return "Widget for displaying an Orientation";
 }

 bool OrientationPlugin::isContainer() const
 {
     return false;
 }

 QString OrientationPlugin::domXml() const
 {
     return "<widget class=\"OrientationView\" name=\"Orientation\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>309</width>\n"
            "   <height>289</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
            "  <string>OrientationView</string>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string></string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString OrientationPlugin::includeFile() const
 {
     return "rock_widget_collection/orientation.h";
 }

