#include "artificialhorizonplugin.h"
#include "artificialhorizon.h"


 ArtificialHorizonPlugin::ArtificialHorizonPlugin(QObject *parent)
     : QObject(parent)
 {
     initialized = false;
 }

 ArtificialHorizonPlugin::~ArtificialHorizonPlugin()
 {
 }

 void ArtificialHorizonPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool ArtificialHorizonPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *ArtificialHorizonPlugin::createWidget(QWidget *parent)
 {
     return new ArtificialHorizon(parent);
 }

 QString ArtificialHorizonPlugin::name() const
 {
     return "ArtificialHorizon";
 }

 QString ArtificialHorizonPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon ArtificialHorizonPlugin::icon() const
 {
     return QIcon(":/artificial_horizon/icon.png");
 }

 QString ArtificialHorizonPlugin::toolTip() const
 {
     return "Widget for displaying an artificial horizon";
 }

 QString ArtificialHorizonPlugin::whatsThis() const
 {
     return "Widget for displaying an artificial horizon";
 }

 bool ArtificialHorizonPlugin::isContainer() const
 {
     return false;
 }

 QString ArtificialHorizonPlugin::domXml() const
 {
     return "<widget class=\"ArtificialHorizon\" name=\"ArtificialHorizon\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>309</width>\n"
            "   <height>289</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
            "  <string>ImageView</string>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string></string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString ArtificialHorizonPlugin::includeFile() const
 {
     return "rock_widget_collection/artificial_horizon/artificialhorizon.h";
 }

