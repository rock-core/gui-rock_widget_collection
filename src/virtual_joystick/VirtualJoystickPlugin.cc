#include "VirtualJoystickPlugin.h"
#include "VirtualJoystick.h"


 VirtualJoystickPlugin::VirtualJoystickPlugin(QObject *parent)
     : QObject(parent)
 {
     initialized = false;
 }

 VirtualJoystickPlugin::~VirtualJoystickPlugin()
 {
 }

 void VirtualJoystickPlugin::initialize(QDesignerFormEditorInterface *formEditor)
 {
     if (initialized)
         return;
     initialized = true;
 }

 bool VirtualJoystickPlugin::isInitialized() const
 {
     return initialized;
 }

 QWidget *VirtualJoystickPlugin::createWidget(QWidget *parent)
 {
     return new VirtualJoystick(parent, name().toStdString());
 }

 QString VirtualJoystickPlugin::name() const
 {
     return "VirtualJoystick";
 }

 QString VirtualJoystickPlugin::group() const
 {
     return "Rock-Robotics";
 }

 QIcon VirtualJoystickPlugin::icon() const
 {
     return QIcon(":/virtual_joystick/icon.png");
 }

 QString VirtualJoystickPlugin::toolTip() const
 {
     return "Widget that emulates a joystick";
 }

 QString VirtualJoystickPlugin::whatsThis() const
 {
     return "widgte that emulates a joystick";
 }

 bool VirtualJoystickPlugin::isContainer() const
 {
     return false;
 }

 QString VirtualJoystickPlugin::domXml() const
 {
     return QString("<widget class=\"VirtualJoystick\" name=\"virtualjoystick\">\n"
            " <property name=\"geometry\">\n"
            "  <rect>\n"
            "   <x>0</x>\n"
            "   <y>0</y>\n"
            "   <width>250</width>\n"
            "   <height>250</height>\n"
            "  </rect>\n"
            " </property>\n"
            " <property name=\"toolTip\" >\n"
	    "  <string>") + toolTip() +
	    QString("</string>\n"
	    "</property>\n"
            " <property name=\"whatsThis\" >\n"
            "  <string>")
	    + whatsThis() + QString("</string>\n"
            " </property>\n"
            "</widget>\n");
 }

 QString VirtualJoystickPlugin::includeFile() const
 {
     return "image_view_widget/VirtualJoystick.h";
 }


