#include "PhononImageViewPlugin.h"
#include "PhononImageView.h"

PhononImageViewPlugin::PhononImageViewPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

PhononImageViewPlugin::~PhononImageViewPlugin()
{
}

bool PhononImageViewPlugin::isContainer() const
{
    return false;
}

bool PhononImageViewPlugin::isInitialized() const
{
    return initialized;
}

QIcon PhononImageViewPlugin::icon() const
{
    return QIcon("");
}

QString PhononImageViewPlugin::domXml() const
{
        return "<ui language=\"c++\">\n"
            " <widget class=\"PhononImageView\" name=\"phononimageview\">\n"
            "  <property name=\"geometry\">\n"
            "   <rect>\n"
            "    <x>0</x>\n"
            "    <y>0</y>\n"
            "     <width>300</width>\n"
            "     <height>120</height>\n"
            "   </rect>\n"
            "  </property>\n"
//            "  <property name=\"toolTip\" >\n"
//            "   <string>PhononImageView</string>\n"
//            "  </property>\n"
//            "  <property name=\"whatsThis\" >\n"
//            "   <string>PhononImageView</string>\n"
//            "  </property>\n"
            " </widget>\n"
            "</ui>\n";
}

QString PhononImageViewPlugin::group() const {
    return "NewImageView";
}

QString PhononImageViewPlugin::includeFile() const {
    return "PhononImageView/PhononImageView.h";
}

QString PhononImageViewPlugin::name() const {
    return "PhononImageView";
}

QString PhononImageViewPlugin::toolTip() const {
    return whatsThis();
}

QString PhononImageViewPlugin::whatsThis() const
{
    return "";
}

QWidget* PhononImageViewPlugin::createWidget(QWidget *parent)
{
    return new PhononImageView(parent);
}

void PhononImageViewPlugin::initialize(QDesignerFormEditorInterface *core)
{
     if (initialized)
         return;
     initialized = true;
}
