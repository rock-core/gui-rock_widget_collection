#include "NewImageViewPlugin.h"
#include "NewImageView.h"

NewImageViewPlugin::NewImageViewPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

NewImageViewPlugin::~NewImageViewPlugin()
{
}

bool NewImageViewPlugin::isContainer() const
{
    return false;
}

bool NewImageViewPlugin::isInitialized() const
{
    return initialized;
}

QIcon NewImageViewPlugin::icon() const
{
    return QIcon("");
}

QString NewImageViewPlugin::domXml() const
{
        return "<ui language=\"c++\">\n"
            " <widget class=\"NewImageView\" name=\"newimageview\">\n"
            "  <property name=\"geometry\">\n"
            "   <rect>\n"
            "    <x>0</x>\n"
            "    <y>0</y>\n"
            "     <width>300</width>\n"
            "     <height>120</height>\n"
            "   </rect>\n"
            "  </property>\n"
//            "  <property name=\"toolTip\" >\n"
//            "   <string>NewImageView</string>\n"
//            "  </property>\n"
//            "  <property name=\"whatsThis\" >\n"
//            "   <string>NewImageView</string>\n"
//            "  </property>\n"
            " </widget>\n"
            "</ui>\n";
}

QString NewImageViewPlugin::group() const {
    return "Rock-Robotics";
}

QString NewImageViewPlugin::includeFile() const {
    return "rock_widget_collection/NewImageView.h";
}

QString NewImageViewPlugin::name() const {
    return "NewImageView";
}

QString NewImageViewPlugin::toolTip() const {
    return whatsThis();
}

QString NewImageViewPlugin::whatsThis() const
{
    return "NewImageView";
}

QWidget* NewImageViewPlugin::createWidget(QWidget *parent)
{
    return new NewImageView(parent);
}

void NewImageViewPlugin::initialize(QDesignerFormEditorInterface *core)
{
     if (initialized)
         return;
     initialized = true;
}
