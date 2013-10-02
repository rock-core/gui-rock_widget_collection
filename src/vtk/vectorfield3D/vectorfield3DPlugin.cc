#include "vectorfield3DPlugin.h"
#include "vectorfield3D.h"


Vectorfield3DPlugin::Vectorfield3DPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

Vectorfield3DPlugin::~Vectorfield3DPlugin()
{
}

bool Vectorfield3DPlugin::isContainer() const
{
    return false;
}

bool Vectorfield3DPlugin::isInitialized() const
{
    return initialized;
}

QIcon Vectorfield3DPlugin::icon() const
{
    return QIcon("");
}

QString Vectorfield3DPlugin::domXml() const
{
        return "<ui language=\"c++\">\n"
            " <widget class=\"Vectorfield3D\" name=\"Vectorfield3d\">\n"
            "  <property name=\"geometry\">\n"
            "   <rect>\n"
            "    <x>0</x>\n"
            "    <y>0</y>\n"
            "     <width>300</width>\n"
            "     <height>120</height>\n"
            "   </rect>\n"
            "  </property>\n"
//            "  <property name=\"toolTip\" >\n"
//            "   <string>Vectorfield3D</string>\n"
//            "  </property>\n"
//            "  <property name=\"whatsThis\" >\n"
//            "   <string>Vectorfield3D</string>\n"
//            "  </property>\n"
            " </widget>\n"
            "</ui>\n";
}

QString Vectorfield3DPlugin::group() const {
    return "Rock-Robotics";
}

QString Vectorfield3DPlugin::includeFile() const {
    return "Vectorfield3D/Vectorfield3D.h";
}

QString Vectorfield3DPlugin::name() const {
    return "Vectorfield3D";
}

QString Vectorfield3DPlugin::toolTip() const {
    return whatsThis();
}

QString Vectorfield3DPlugin::whatsThis() const
{
    return "";
}

QWidget* Vectorfield3DPlugin::createWidget(QWidget *parent)
{
    return new Vectorfield3D(parent);
}

void Vectorfield3DPlugin::initialize(QDesignerFormEditorInterface *core)
{
     if (initialized)
         return;
     initialized = true;
}
