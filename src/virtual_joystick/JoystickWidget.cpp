#include "JoystickWidget.h"
#include <cmath>
#include <iostream>

using namespace std;

JoystickWidget::JoystickWidget(QWidget* parent, const std::string& name) : QWidget(parent)
{
    filled = false;

    title = new QLabel(tr(name.c_str()), this);
    axis1 = new QLabel("Axis 1: ", this);
    axis1->setGeometry(150, 0, axis1->width(), axis1->height());
    axis2 = new QLabel("Axis 2: ", this);
    axis2->setGeometry(150, 13, axis1->width(), axis1->height());
    setFixedSize(250, 250);
    setMouseTracking(true);
    followMouse = false;
    pressed = false;
    setCursor(Qt::CrossCursor);
    widgetCenter = rect().center() + QPointF(0,25);
    painterCenter = widgetCenter - QPointF(0,10);
    painterAngle = 0;
    mousePosition = painterCenter;
    baseGrad = QLinearGradient(widgetCenter-QPoint(38,0), widgetCenter+QPoint(38,0));
    baseGrad.setColorAt(0, QColor(160, 160, 160));
    baseGrad.setColorAt(1, QColor(20, 20, 20));
    baseGrad.setSpread(QGradient::ReflectSpread);
    baseColor = QColor(80, 80, 80);
    update();
    filled = true;
    startTimer(100);
}

JoystickWidget::~JoystickWidget()
{
}


void JoystickWidget::focus(bool in) {
    setVisible(in);
}

void JoystickWidget::paintEvent(QPaintEvent* event)
{
    (void)event;
    QPainter painter(this);
    painter.drawRect(0, 0, 250, 250);
    
    calculateCoordinates();
    drawMouseRects(painter);
    paintBase(painter);
    paintStick(painter);
    
    axis1->setText("Axis1: " + QString().setNum(axisValues.x()));
    axis2->setText("Axis2: " + QString().setNum(axisValues.y()));  
}


void JoystickWidget::calculateCoordinates(void) {
    height = 0; 
    maxHeight = 1;
    if (mousePosition.y() < painterCenter.y()-90)
	maxHeight = 100;
    else if (mousePosition.y() > painterCenter.y()+70)
	maxHeight = 80;
    else
	maxHeight = 88 - (mousePosition.y()-painterCenter.y())/9;

    height = distance(painterCenter, mousePosition);
    if (height > maxHeight)
	height = maxHeight;
}

void JoystickWidget::paintCross(QPainter &painter)
{
    painter.drawLine(0, 125, 250, 125);
    painter.drawLine(125, 0, 125, 250);
}

void JoystickWidget::paintBase(QPainter& painter)
{
    painter.setPen(Qt::transparent);
    painter.setBrush(baseGrad);
    painter.drawEllipse(widgetCenter, 45, 45);
    painter.setBrush(QColor(110, 110, 110));
    painter.drawEllipse(widgetCenter-QPoint(0, 5), 45, 45);
    painter.setBrush(baseColor);
    painter.drawEllipse(widgetCenter-QPoint(0, 5), 42, 42);
    painter.setBrush(QColor(50, 50, 50));
    painter.drawEllipse(widgetCenter-QPoint(0, 10), 16, 16);
}

void JoystickWidget::paintStick(QPainter& painter)
{ 
    painterAngle = angle(painterCenter, mousePosition);

    if (followMouse == false && pressed == false) {
	painterAngle = 0; 
	height = 50;
    } 
	
    painter.translate(painterCenter);
    painter.rotate(painterAngle);  

    QRect cylinder(-10, 10, 20, -height);//cylinderHeight());
    QLinearGradient grad(cylinder.topLeft()+QPoint(5,0), cylinder.topRight()-QPoint(5,0));
    grad.setColorAt(0, QColor(250, 250, 250));
    grad.setColorAt(1, QColor(80, 80, 80));
    grad.setSpread(QGradient::ReflectSpread);
    painter.setBrush(grad);
    painter.drawRoundedRect(cylinder, 9, 9);

    double radius = 80-height/5;
    QRect ball(cylinder.center()+QPoint(0, cylinder.height()/2)-QPoint(radius/2, radius/2),
		QSize(radius, radius));
    QRadialGradient rad(ball.center(), radius, ball.center()-QPoint(10, 10));
    rad.setColorAt(0, QColor(255, 200, 200));
    rad.setColorAt(1, QColor(255, 63, 63));
    painter.setBrush(rad);
    painter.drawEllipse(ball);

}


double JoystickWidget::distance(QPointF A, QPointF B)
{
    return sqrt(pow(A.x()-B.x(), 2) + pow(A.y()-B.y(), 2));
}

double JoystickWidget::angle(QPointF A, QPointF B)
{
    return atan2(B.x()-A.x(), A.y()-B.y()) * 180 / M_PI;
}

void JoystickWidget::drawMouseRects(QPainter &painter)
{
    painter.setPen(Qt::DashLine);
    painter.drawLine(250/3, 0, 250/3, 250);
    painter.drawLine(500/3, 0, 500/3, 250);
    painter.drawLine(0, 250/3, 250, 250/3);
    painter.drawLine(0, 500/3, 250, 500/3);
}

QPointF JoystickWidget::calculateAxis(bool active)
{
    if(!active)
	return QPointF(0,0);
    
    QPointF bc(mousePosition.x() - 125, mousePosition.y() - 125);
    
    double dist = sqrt(pow(bc.x(),2) + pow(bc.y(), 2));

    //normalize the balldistance
    if(dist > 125)
	bc /= dist;
    else
	bc /= 125.0;
    
    return  bc;
}

void JoystickWidget::mouseMoveEvent(QMouseEvent* event)
{
    mousePosition = event->posF();
    axisValues = calculateAxis(followMouse);
    
    update();
}  

void JoystickWidget::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::RightButton:
	followMouse = !followMouse;
	pressed = !pressed;
	axisValues = calculateAxis(followMouse);
	update();
	emit moveSignal(axisValues.x(), axisValues.y());
	break;
	
    case Qt::LeftButton:
	followMouse = true;
	update();
	break;
    
    default:
	break;
    }
    
}


void JoystickWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (pressed)
	return;
    if (event->button() == Qt::LeftButton) {
	followMouse = false;
	axisValues = calculateAxis(followMouse);
	update();
	emit moveSignal(axisValues.x(), axisValues.y());
    }
}

void JoystickWidget::timerEvent(QTimerEvent *event) 
{
    if (pressed == false)
	return;
    else {
	emit moveSignal(axisValues.x(), axisValues.y());
    }
}

