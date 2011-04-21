
#include <QtGui/qwidget.h>
#include <QtDesigner/abstractformeditor.h>
#include <QtDesigner/abstractformwindowmanager.h>
#include <QtDesigner/abstractformwindowcursor.h>
#include <QtGui/qabstractbutton.h>

#include "MultiViewPlugin.h"
//#include <QtPlugin>


MultiViewPlugin::MultiViewPlugin(QObject* parent) : QObject(parent)
{
    std::cout << "MultiViewPlugin created" << std::endl;
    widget = NULL;
    initialized = false;
}

MultiViewPlugin::~MultiViewPlugin()
{
    delete widget;
}

bool MultiViewPlugin::isContainer() const
{
    return true;
}

bool MultiViewPlugin::isInitialized() const
{
    return initialized;
}

QIcon MultiViewPlugin::icon() const
{
    return QIcon();
}

QString MultiViewPlugin::group() const
{
    return "Rock-Robotics";
}

QString MultiViewPlugin::includeFile() const
{
    return "MultiViewWidget.h";
}

QString MultiViewPlugin::name() const
{
    return "MultiViewWidget";
}

QString MultiViewPlugin::toolTip() const
{
    return "Widget for displaying multiple widgets";
}

QString MultiViewPlugin::whatsThis() const
{
    return "Container widget for other widgets";
}

void MultiViewPlugin::initialize(QDesignerFormEditorInterface* core)
{
    if (initialized)
    {
        return;
    }
    formInterface = core;
    connect(formInterface->formWindowManager(), SIGNAL(activeFormWindowChanged(QDesignerFormWindowInterface*)),
            this, SLOT(activeFormWindowChanged(QDesignerFormWindowInterface*)));
    initialized = true;
}

void MultiViewPlugin::widgetUnmanaged(QWidget* widget)
{
    std::cout << "Unmanaged widget" << std::endl;
}

void MultiViewPlugin::widgetRemoved(QWidget* widget)
{
    std::cout << "Removed widget" << std::endl;
}

void MultiViewPlugin::widgetManaged(QWidget* widget)
{
    std::cout << "Managed Widget" << std::endl;
}

void MultiViewPlugin::manageWidget(QWidget* widget)
{
    std::cout << (formInterface == NULL) << std::endl;
    // when designer loads a ui file it will call manage widget BEFORE
    // setting the active Form Window, save what shall be added
    // and wait for the activeFormWindow signal and add everything up
    // to that point
    if(formInterface->formWindowManager()->activeFormWindow() == NULL)
    {
        lastWidgets.push_back(widget);
        return;
    }
    formInterface->formWindowManager()->activeFormWindow()->manageWidget(widget);
    WidgetButton* button = dynamic_cast<WidgetButton*>(widget);
    if(button != NULL)
    {
        button->setIconAlternative(QIcon(), true);
        std::cout << "Managing Designer stuff" << std::endl;
        formInterface->formWindowManager()->activeFormWindow()->manageWidget(button->getWidget());
    }
    std::cout << "Connecting signals" << std::endl;
    connect(formInterface->formWindowManager()->activeFormWindow(), SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
    connect(formInterface->formWindowManager()->activeFormWindow(), SIGNAL(widgetUnmanaged(QWidget*)), this, SLOT(widgetUnmanaged(QWidget*)));
    connect(formInterface->formWindowManager()->activeFormWindow(), SIGNAL(widgetRemoved(QWidget*)), this, SLOT(widgetRemoved(QWidget*)));
    connect(formInterface->formWindowManager()->activeFormWindow(), SIGNAL(widgetManaged(QWidget*)), this, SLOT(widgetManaged(QWidget*)));
}

void MultiViewPlugin::activeFormWindowChanged(QDesignerFormWindowInterface* formWindow)
{
    // needs to be done for reloading as widgets will be added before the
    // active form window is set by the designer
    if(formWindow != NULL && lastWidgets.size() > 0)
    {
        for(int i=0;i<lastWidgets.size();i++)
        {
            manageWidget(lastWidgets[i]);
        }
        lastWidgets.clear();
    }

}

void MultiViewPlugin::selectionChanged()
{
    std::cout << "Selection Changed" << std::endl;
    QWidget* selected = formInterface->formWindowManager()->activeFormWindow()->cursor()->selectedWidget(0);
    WidgetButton* button = dynamic_cast<WidgetButton*>(selected);
    if(button != NULL)
    {
        button->click();
    }
    else
    {
        button = widget->getButtonForWidget(selected);
        if(button != NULL)
        {
            button->click();
        }
    }
    if(button != NULL)
    {
        QList<WidgetButton*> buttons = widget->getAllWidgetButtons();
        for(int i=0;i<buttons.size();i++)
        {
            if(button != buttons[i])
            {
                buttons[i]->getWidget()->setMaximumSize(0, 0);
            }
            else
            {
                buttons[i]->getWidget()->setMaximumSize(1000000, 1000000);
            }

        }
    }
}

QWidget* MultiViewPlugin::createWidget(QWidget* parent)
{
    widget = new MultiViewWidget(parent);
    connect(widget, SIGNAL(widgetButtonAdded(QWidget*)), this, SLOT(manageWidget(QWidget*)));
    std::cout << "Create" << std::endl;
    return widget;
}

QString MultiViewPlugin::domXml() const
{
    return "<widget class=\"MultiViewWidget\" name=\"MultiViewWidget\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>400</width>\n"
        "   <height>300</height>\n"
        "  </rect>\n"
        " </property>\n"
        " <property name=\"toolTip\" >\n"
        "  <string>MultiView</string>\n"
        " </property>\n"
        " <property name=\"whatsThis\" >\n"
        "  <string>Displays multiple widgets in a container.</string>\n"
        " </property>\n"
        "</widget>\n";

}

// Q_EXPORT_PLUGIN2(MultiViewPlugin, MultiViewPlugin)