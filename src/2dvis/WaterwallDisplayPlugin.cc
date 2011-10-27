#include "WaterwallDisplayPlugin.h"
#include "qwaterfalldisplay.h"


 WaterfallDisplayPlugin::WaterfallDisplayPlugin(QObject *parent)
     : QObject(parent)
 {
     initialized = false;
 }

 WaterfallDisplayPlugin::~WaterfallDisplayPlugin()
 {
 }

 void WaterfallDisplayPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool WaterfallDisplayPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *WaterfallDisplayPlugin::createWidget(QWidget *parent)
 {
     return new QWaterfallDisplay(320,240,parent);
 }

 QString WaterfallDisplayPlugin::name() const
 {
     return "WaterfallDisplay";
 }

 QString WaterfallDisplayPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon WaterfallDisplayPlugin::icon() const
 {
     return QIcon();
     //return QIcon(":/image_view/icon.png");
 }

 QString WaterfallDisplayPlugin::toolTip() const
 {
     return "Widget for displaying data in a waterfall display";
 }

 QString WaterfallDisplayPlugin::whatsThis() const
 {
     return "Widget for displaying data";
 }

 bool WaterfallDisplayPlugin::isContainer() const
 {
     return false;
 }

 QString WaterfallDisplayPlugin::domXml() const
 {
     return "<widget class=\"QWaterfallDisplay\" name=\"waterfalldisplay\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>320</width>\n"
            "   <height>240</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
            "  <string>WaterfallDisplay</string>\n"
            " </property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string>Widget for displaying data in a waterfall display</string>\n"
            " </property>\n"
            "</widget>\n";
 }

 QString WaterfallDisplayPlugin::includeFile() const
 {
     return "2dvis/qwaterfalldisplay.h";
 }

