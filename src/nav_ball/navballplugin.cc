#include "navballplugin.h"
#include "navball.h"

 NavBallPlugin::NavBallPlugin(QObject *parent) : QObject(parent)
 {
     initialized = false;
 }

 NavBallPlugin::~NavBallPlugin()
 {
 }

 void NavBallPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool NavBallPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *NavBallPlugin::createWidget(QWidget *parent)
 {
     return new NavBallView(parent);
 }

 QString NavBallPlugin::name() const
 {
     return "NavBallView";
 }

 QString NavBallPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon NavBallPlugin::icon() const
 {
     return QIcon(":/artificial_horizon/icon.png");
 }

 QString NavBallPlugin::toolTip() const
 {
     return "Widget for displaying an Orientation";
 }

 QString NavBallPlugin::whatsThis() const
 {
     return "Widget for displaying an Orientation";
 }

 bool NavBallPlugin::isContainer() const
 {
     return false;
 }

 QString NavBallPlugin::domXml() const
 {
     return "<widget class=\"NavBallView\" name=\"NavBall\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>150</width>\n"
            "   <height>150</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
            "  <string>NavBallView</string>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string></string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString NavBallPlugin::includeFile() const
 {
     return "rock_widget_collection/navball.h";
 }

