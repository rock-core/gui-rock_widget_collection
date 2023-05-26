#include "ProgressIndicatorPlugin.h"
#include "ProgressIndicator.h"

ProgressIndicatorPlugin::ProgressIndicatorPlugin(QObject *parent)
    : QObject(parent)
{
    Q_INIT_RESOURCE(resources);

    initialized = false;
}

ProgressIndicatorPlugin::~ProgressIndicatorPlugin()
{
}

bool ProgressIndicatorPlugin::isContainer() const
{
    return false;
}

bool ProgressIndicatorPlugin::isInitialized() const
{
    return initialized;
}

QIcon ProgressIndicatorPlugin::icon() const
{
    return QIcon(":/progress_indicator/icon");
}

QString ProgressIndicatorPlugin::domXml() const
{
        return "<ui language=\"c++\">\n"
            " <widget class=\"ProgressIndicator\" name=\"progressindicator\">\n"
            "  <property name=\"geometry\">\n"
            "   <rect>\n"
            "    <x>0</x>\n"
            "    <y>0</y>\n"
            "     <width>100</width>\n"
            "     <height>100</height>\n"
            "   </rect>\n"
            "  </property>\n"
//            "  <property name=\"toolTip\" >\n"
//            "   <string>ProgressIndicator</string>\n"
//            "  </property>\n"
//            "  <property name=\"whatsThis\" >\n"
//            "   <string>ProgressIndicator</string>\n"
//            "  </property>\n"
            "  <property name=\"displayedWhenStopped\">\n"
            "   <bool>true</bool>\n"
            "  </property>\n"
            " </widget>\n"
            "</ui>\n";
}

QString ProgressIndicatorPlugin::group() const {
    return "Rock-Robotics";
}

QString ProgressIndicatorPlugin::includeFile() const {
    return "ProgressIndicator/ProgressIndicator.h";
}

QString ProgressIndicatorPlugin::name() const {
    return "ProgressIndicator";
}

QString ProgressIndicatorPlugin::toolTip() const {
    return whatsThis();
}

QString ProgressIndicatorPlugin::whatsThis() const
{
    return "";
}

QWidget* ProgressIndicatorPlugin::createWidget(QWidget *parent)
{
    return new ProgressIndicator(parent);
}

void ProgressIndicatorPlugin::initialize(QDesignerFormEditorInterface *core)
{
     if (initialized)
         return;
     initialized = true;
}
