#include "ImageViewOldPlugin.h"
#include "ImageViewOld.h"


 ImageViewOldPlugin::ImageViewOldPlugin(QObject *parent)
     : QObject(parent)
 {
     initialized = false;
 }

 ImageViewOldPlugin::~ImageViewOldPlugin()
 {
 }

 void ImageViewOldPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool ImageViewOldPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *ImageViewOldPlugin::createWidget(QWidget *parent)
 {
     return new ImageViewOld(parent);
 }

 QString ImageViewOldPlugin::name() const
 {
     return "ImageViewOld";
 }

 QString ImageViewOldPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon ImageViewOldPlugin::icon() const
 {
     return QIcon(":/image_view/icon.png");
 }

 QString ImageViewOldPlugin::toolTip() const
 {
     return "Old widget for displaying images. This one is deprecated, use ImageView instead.";
 }

 QString ImageViewOldPlugin::whatsThis() const
 {
     return "Old widget for displaying images. This one is deprecated, use ImageView instead.";
 }

 bool ImageViewOldPlugin::isContainer() const
 {
     return false;
 }

 QString ImageViewOldPlugin::domXml() const
 {
     return "<widget class=\"ImageViewOld\" name=\"imageviewold\">\n"
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

 QString ImageViewOldPlugin::includeFile() const
 {
     return "image_view_widget/ImageView.h";
 }

