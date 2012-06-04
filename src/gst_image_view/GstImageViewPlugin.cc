#include "GstImageViewPlugin.h"
#include "GstImageView.h"

GstImageViewPlugin::GstImageViewPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

GstImageViewPlugin::~GstImageViewPlugin()
{
}

bool GstImageViewPlugin::isContainer() const
{
    return false;
}

bool GstImageViewPlugin::isInitialized() const
{
    return initialized;
}

QIcon GstImageViewPlugin::icon() const
{
    return QIcon("");
}

QString GstImageViewPlugin::domXml() const
{
        return "<ui language=\"c++\">\n"
            " <widget class=\"GstImageView\" name=\"gstimageview\">\n"
            "  <property name=\"geometry\">\n"
            "   <rect>\n"
            "    <x>0</x>\n"
            "    <y>0</y>\n"
            "     <width>300</width>\n"
            "     <height>120</height>\n"
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

QString GstImageViewPlugin::group() const {
    return "Rock-Robotics";
}

QString GstImageViewPlugin::includeFile() const {
    return "GstImageView/GstImageView.h";
}

QString GstImageViewPlugin::name() const {
    return "GstImageView";
}

QString GstImageViewPlugin::toolTip() const {
    return whatsThis();
}

QString GstImageViewPlugin::whatsThis() const
{
    return "";
}

QWidget* GstImageViewPlugin::createWidget(QWidget *parent)
{
    return new GstImageView(parent);
}

void GstImageViewPlugin::initialize(QDesignerFormEditorInterface *core)
{
     if (initialized)
         return;
     initialized = true;
}
