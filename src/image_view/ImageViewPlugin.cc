#include "ImageViewPlugin.h"
#include "ImageView.h"


 ImageViewPlugin::ImageViewPlugin(QObject *parent)
     : QObject(parent)
 {
     initialized = false;
 }

 ImageViewPlugin::~ImageViewPlugin()
 {
 }

 void ImageViewPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool ImageViewPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *ImageViewPlugin::createWidget(QWidget *parent)
 {
     return new ImageView(parent);
 }

 QString ImageViewPlugin::name() const
 {
     return "ImageView";
 }

 QString ImageViewPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon ImageViewPlugin::icon() const
 {
     return QIcon(":/image_view/icon.png");
 }

 QString ImageViewPlugin::toolTip() const
 {
     return "Widget for displaying images";
 }

 QString ImageViewPlugin::whatsThis() const
 {
     return "widgte for displaying images";
 }

 bool ImageViewPlugin::isContainer() const
 {
     return false;
 }

 QString ImageViewPlugin::domXml() const
 {
     return "<widget class=\"ImageView\" name=\"imageview\">\n"
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

 QString ImageViewPlugin::includeFile() const
 {
     return "image_view_widget/ImageView.h";
 }

