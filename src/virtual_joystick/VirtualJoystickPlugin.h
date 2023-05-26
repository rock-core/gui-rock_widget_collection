#ifndef VIRTUALJOYSTICKPLUGIN_H
#define VIRTUALJOYSTICKPLUGIN_H

#include <QtGui/QtGui>
#if QT_VERSION >= 0x050000
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#else
#include <QtDesigner/QDesignerCustomWidgetInterface>
#endif

class VirtualJoystickPlugin : public QObject , public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)

    public:
	VirtualJoystickPlugin(QObject *parent = 0);
	virtual ~VirtualJoystickPlugin();

	bool isContainer() const;
	bool isInitialized() const;
	QIcon icon() const;
	QString domXml() const;
	QString group() const;
	QString includeFile() const;
	QString name() const;
	QString toolTip() const;
	QString whatsThis() const;
	QWidget *createWidget(QWidget *parent);
	void initialize(QDesignerFormEditorInterface *core);

    private:
	bool initialized; 
};

#endif // VIRTUALJOYSTICKPLUGIN_H
