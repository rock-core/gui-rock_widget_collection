#ifndef _JOYSTICK_WIDGET_H
#define _JOYSTICK_WIDGET_H

#include <QtGui>
#include <cstring>

class JoystickWidget : public QWidget {

Q_OBJECT

    public:

	JoystickWidget(QWidget *parent, const std::string &name);
	~JoystickWidget();

    signals:
	void moveSignal(double, double); // axis 1, axis 2 

    private:
	bool filled;

	void focus(bool in);

	QPointF calculateAxis(bool active);
	
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void timerEvent(QTimerEvent* event);
	void paintEvent(QPaintEvent* event);
	void paintBase(QPainter &painter);
	void paintStick(QPainter &painter);
	void paintCross(QPainter &painter);

	void drawMouseRects(QPainter &painter);
	void calculateCoordinates(void);

	double distance(QPointF A, QPointF B);
	double angle(QPointF A, QPointF B);

	QPointF mousePosition;
	double painterAngle;
	QPointF painterCenter;
	QPointF widgetCenter;
	QPointF axisValues;
	
	QLinearGradient baseGrad;
	QColor baseColor;
	double height, maxHeight;
	bool followMouse;
	bool pressed;

	QLabel *title;
	QLabel *axis1;
	QLabel *axis2;
	QLabel *str_title;
};

#endif // _JOYSTICK_WIDGET_H
