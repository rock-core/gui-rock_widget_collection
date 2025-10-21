#include "compassplugin.h"
#include "compass.h"


 CompassPlugin::CompassPlugin(QObject *parent) : QObject(parent)
 {
     Q_INIT_RESOURCE(resources);

     initialized = false;
 }

 CompassPlugin::~CompassPlugin()
 {
 }

 void CompassPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool CompassPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *CompassPlugin::createWidget(QWidget *parent)
 {
     return new Compass(parent);
 }

 QString CompassPlugin::name() const
 {
     return "Compass";
 }

 QString CompassPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon CompassPlugin::icon() const
 {
     return QIcon(":/artificial_horizon/compass_icon.png");
 }

 QString CompassPlugin::toolTip() const
 {
     return "Widget for displaying an Compass";
 }

 QString CompassPlugin::whatsThis() const
 {
     return "Widget for displaying an Compass";
 }

 bool CompassPlugin::isContainer() const
 {
     return false;
 }

 QString CompassPlugin::domXml() const
 {
     return "<widget class=\"Compass\" name=\"Compass\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>309</width>\n"
            "   <height>289</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
            "  <string>Compass</string>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string></string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString CompassPlugin::includeFile() const
 {
     return "rock_widget_collection/compass.h";
 }

