#include "Plot2dPlugin.h"
#include "Plot2d.h"


 Plot2dPlugin::Plot2dPlugin(QObject *parent)
     : QObject(parent)
 {
     initialized = false;
 }

 Plot2dPlugin::~Plot2dPlugin()
 {
 }

 void Plot2dPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool Plot2dPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *Plot2dPlugin::createWidget(QWidget *parent)
 {
     return new Plot2d(parent);
 }

 QString Plot2dPlugin::name() const
 {
     return "Plot2d";
 }

 QString Plot2dPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon Plot2dPlugin::icon() const
 {
     return QIcon(":/vtk/plot2d/icon.png");
 }

 QString Plot2dPlugin::toolTip() const
 {
     return "Widget for plotting 2d data";
 }

 QString Plot2dPlugin::whatsThis() const
 {
     return "widgte for plotting 2d data";
 }

 bool Plot2dPlugin::isContainer() const
 {
     return false;
 }

 QString Plot2dPlugin::domXml() const
 {
     return "<widget class=\"Plot2d\" name=\"plot2d\">\n"
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

 QString Plot2dPlugin::includeFile() const
 {
     return "vtk/plot2d/Plot2d.h";
 }

