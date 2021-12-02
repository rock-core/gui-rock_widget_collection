#ifndef _MULTI_VIEW_WIDGET_H
#define _MULTI_VIEW_WIDGET_H

#include <PaintWidget.h>
#include <QtDesigner/QDesignerExportWidget>


class QDESIGNER_WIDGET_EXPORT MultiWidget : public PaintWidget 
{
	Q_OBJECT
  Q_CLASSINFO("Author", "Matthias Goldhoorn")
	Q_PROPERTY(bool hideWhenMinimized READ isHiddenWhenMinimized WRITE hideWhenMin USER false)
	Q_PROPERTY(QString minimizedLabel READ getMinimizedLabel WRITE setMinimizedLabel)

signals:
	void activityChanged(bool);
public:
	MultiWidget(QWidget *parent=0);
        virtual ~MultiWidget();

public slots:

	void setActive(bool b);
	QString getMinimizedLabel();
	void setMinimizedLabel(QString label);


    /**
     * Returns whether this is a container widget
     * @return will always return true
     */
    
    void childEvent(QChildEvent* event);

    bool event(QEvent* event);

    bool isHiddenWhenMinimized() const;

    void hideWhenMin(bool b);
	

protected:
	/** True if this Widget is not Minimized */
	bool isActive;
        bool hideWhenMinimized;
	bool wasHidden;
	QString minimizedLabel;
};


#endif
