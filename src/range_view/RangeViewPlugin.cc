#include "RangeViewPlugin.h"
#include "RangeView.h"


 RangeViewPlugin::RangeViewPlugin(QObject *parent)
     : QObject(parent)
 {
     initialized = false;
 }

 RangeViewPlugin::~RangeViewPlugin()
 {
 }

 void RangeViewPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool RangeViewPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *RangeViewPlugin::createWidget(QWidget *parent)
 {
     return new RangeView(parent);
 }

 QString RangeViewPlugin::name() const
 {
     return "RangeView";
 }

 QString RangeViewPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon RangeViewPlugin::icon() const
 {
     return QIcon(":/sonar_view/icon.png");
 }

 QString RangeViewPlugin::toolTip() const
 {
     return "Widget for displaying sonar data";
 }

 QString RangeViewPlugin::whatsThis() const
 {
     return "widget for displaying sonar data";
 }

 bool RangeViewPlugin::isContainer() const
 {
     return false;
 }

 QString RangeViewPlugin::domXml() const
 {
     return "<widget class=\"RangeView\" name=\"sonarview\">\n"
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
            "  <string>Widget for displaying frames.</string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString RangeViewPlugin::includeFile() const
 {
     return "image_view_widget/sonar_view/RangeView.h";
 }

