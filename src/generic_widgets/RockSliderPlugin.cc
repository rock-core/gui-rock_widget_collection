#include "RockSliderPlugin.h"
#include "RockSlider.h"


 RockSliderPlugin::RockSliderPlugin(QObject *parent) : QObject(parent)
 {
     Q_INIT_RESOURCE(resources);
     initialized = false;
 }

 RockSliderPlugin::~RockSliderPlugin()
 {
 }

 void RockSliderPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool RockSliderPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *RockSliderPlugin::createWidget(QWidget *parent)
 {
     return new RockSlider(parent);
 }

 QString RockSliderPlugin::name() const
 {
     return "RockSlider";
 }

 QString RockSliderPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon RockSliderPlugin::icon() const
 {
     return QIcon(":/artificial_horizon/icon.png");
 }

 QString RockSliderPlugin::toolTip() const
 {
     return "";
 }

 QString RockSliderPlugin::whatsThis() const
 {
     return "";
 }

 bool RockSliderPlugin::isContainer() const
 {
     return false;
 }

 QString RockSliderPlugin::domXml() const
 {
     return "<widget class=\"RockSlider\" name=\"RockSlider\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>309</width>\n"
            "   <height>289</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
            "  <string>RockSlider</string>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string></string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString RockSliderPlugin::includeFile() const
 {
     return "rock_widget_collection/RockSlider.h";
 }

