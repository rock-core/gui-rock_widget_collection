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

bool ImageViewPlugin::isContainer() const
{
    return false;
}

bool ImageViewPlugin::isInitialized() const
{
    return initialized;
}

QIcon ImageViewPlugin::icon() const
{
    return QIcon("");
}

QString ImageViewPlugin::domXml() const
{
        return "<ui language=\"c++\">\n"
            " <widget class=\"ImageView\" name=\"imageview\">\n"
            "  <property name=\"geometry\">\n"
            "   <rect>\n"
            "    <x>0</x>\n"
            "    <y>0</y>\n"
            "     <width>320</width>\n"
            "     <height>240</height>\n"
            "   </rect>\n"
            "  </property>\n"
//            "  <property name=\"toolTip\" >\n"
//            "   <string>GstImageView</string>\n"
//            "  </property>\n"
//            "  <property name=\"whatsThis\" >\n"
//            "   <string>GstImageView</string>\n"
//            "  </property>\n"
            " </widget>\n"
            "</ui>\n";
}

QString ImageViewPlugin::group() const {
    return "Rock-Robotics";
}

QString ImageViewPlugin::includeFile() const {
    return "rock_widget_collection/ImageView.h";
}

QString ImageViewPlugin::name() const {
    return "ImageView";
}

QString ImageViewPlugin::toolTip() const {
    return whatsThis();
}

QString ImageViewPlugin::whatsThis() const
{
    return "";
}

QWidget* ImageViewPlugin::createWidget(QWidget *parent)
{
    return new ImageView(parent);
}

void ImageViewPlugin::initialize(QDesignerFormEditorInterface *core)
{
     if (initialized)
         return;
     initialized = true;
}
