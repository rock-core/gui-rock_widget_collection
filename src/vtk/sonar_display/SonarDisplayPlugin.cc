#include "SonarDisplayPlugin.h"
#include "SonarDisplay.h"


 SonarDisplayPlugin::SonarDisplayPlugin(QObject *parent)
     : QObject(parent)
 {
     Q_INIT_RESOURCE(resources);

     initialized = false;
 }

 SonarDisplayPlugin::~SonarDisplayPlugin()
 {
 }

 void SonarDisplayPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool SonarDisplayPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *SonarDisplayPlugin::createWidget(QWidget *parent)
 {
     return new SonarDisplay(parent);
 }

 QString SonarDisplayPlugin::name() const
 {
     return "SonarDisplay";
 }

 QString SonarDisplayPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon SonarDisplayPlugin::icon() const
 {
     return QIcon(":/vtk/sonar_display/icon.png");
 }

 QString SonarDisplayPlugin::toolTip() const
 {
     return "Widget for displaying sonar data";
 }

 QString SonarDisplayPlugin::whatsThis() const
 {
     return "widgte for displaying sonar data";
 }

 bool SonarDisplayPlugin::isContainer() const
 {
     return false;
 }

 QString SonarDisplayPlugin::domXml() const
 {
     return "<widget class=\"SonarDisplay\" name=\"sonar_display\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>320</width>\n"
            "   <height>240</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
            "  <string>ImageView</string>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string>Widget for displaying sonar data.</string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString SonarDisplayPlugin::includeFile() const
 {
     return "vtk/sonar_display/SonarDisplay.h";
 }

