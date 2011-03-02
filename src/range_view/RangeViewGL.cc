/* 
 * File:   RangeViewGL.cc
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de) 
 * 
 */

#include <stdexcept>
#include "RangeViewGL.h"
#include <QtCore/QtPlugin>
#include <QtOpenGL>
#include <math.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>

const float RangeViewGL::ZOOM_MAX;
const float RangeViewGL::ZOOM_MIN;


RangeViewGL::RangeViewGL(ImageView &parent):
	ImageViewGL(parent),
	 zoom( 45.0 )

{
     xRot =0;//180*16;
     yRot = 0;
     zRot = 0;
     xShift = 0;
     yShift = 0;
     zShift = 0;
//	repaintTimer.setSingleShot(false);
//	repaintTimer.setInterval(100);
//	connect(&repaintTimer,SIGNAL(timeout()),this,SLOT(repaintFunc()));
//	repaintTimer.start();
	
}



void RangeViewGL::setData(const std::vector<Eigen::Vector3d> data){
	mutex.lock();
	this->data = data;
	mutex.unlock();
	updateGL();
}

RangeViewGL::~RangeViewGL()
{
    //glDeleteLists(object, 1);
}

QSize RangeViewGL::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize RangeViewGL::sizeHint() const
{
    return QSize(400, 400);
}


void RangeViewGL::keyPressEvent ( QKeyEvent * event ){
	printf("bla taste\n");
	switch (event->key()) {
		case Qt::Key_W:
		return;
	default:
		return;
}
}


void RangeViewGL::repaintFunc(){
	update();
	updateGL();
}


void RangeViewGL::setXRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != xRot) {
		xRot = angle;
		emit xRotationChanged(angle);
	}
}

void RangeViewGL::setYRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != yRot) {
		yRot = angle;
		emit yRotationChanged(angle);
	}
}

void RangeViewGL::setZRotation(int angle)
{
	normalizeAngle(&angle);
	if (angle != zRot) {
		zRot = angle;
		emit zRotationChanged(angle);
	}
}

void RangeViewGL::setxPosition(int value)
{
	xShift = value;
	emit xPositionChanged(value);
}
void RangeViewGL::setyPosition(int value)
{
	yShift = value;
	emit yPositionChanged(value);
}
void RangeViewGL::setzPosition(int value)
{
	zShift = value;
	emit zPositionChanged(value);
}

void RangeViewGL::initializeGL()
{
	//qglClearColor(QColor(0.1,0.1,0.0));
	//qglClearColor(purple.light());
	//qglClearColor(white);
	qglClearColor(QColor(0.0,0.0,0.0));
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	groundPlane = glGenLists(1);
	glNewList(groundPlane, GL_COMPILE);

	#if 1  // draw the grid

	glDisable( GL_LIGHTING );
	glDisable( GL_COLOR_MATERIAL ) ;

	float size = 50.0f;
	float interval = 1.0f;


	glColor4f(0.5f,0.5f,1.0f,0.5f);
	glBegin(GL_LINES);
	float x = - size*0.5f;
	while( x <= size*0.5f ) {
		glVertex3f(-size/2.0f, x, 0.01f);
		glVertex3f(size/2.0f, x, 0.01f);
		glVertex3f(x, -size/2.0f, 0.01f);
		glVertex3f(x, size/2.0f, 0.01f);
		x += interval;
	}
	glEnd();

	// draw concentric circles
	float r;
	for(r=0;r<size/2;r+=interval) {
		glBegin(GL_LINES);
		for(x=0;x<(2*3.14152);x+=(2*3.14152)/(r*100)) {
			glVertex3f(cos(x)*r, sin(x)*r, 0.01f);
		}
		glEnd();
	}
	glEndList();
	#endif
}



void RangeViewGL::paintGL(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslated(0, 0, -zoom);

	glTranslated(double(xShift)/100.0, double(yShift)/100.0, double(zShift)/100.0);
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

	double value=255.0;
	QColor color(value,value,value);
	QColor red(255.0,0,0);
	qglColor(red);
	glBegin(GL_POINTS);
	glPointSize(5);
	glVertex3f(0,0,0);	
	glPointSize(1);
	qglColor(color);
	for(int i=0;i<data.size();i++){
		glVertex3f(data[i][0],data[i][1],data[i][2]);
	}
	glEnd();

}



void RangeViewGL::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0, (float)width/(float)height, 1.0, 1000.0 );
	glMatrixMode(GL_MODELVIEW);
	updateGL();
}

void RangeViewGL::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	lastPosTrans = event->pos();
	updateGL();
}

void RangeViewGL::mouseMoveEvent(QMouseEvent *event)
{
	//int dx = event->x() - lastPos.x();
	//int dy = event->y() - lastPos.y();

	int dxt = event->x() - lastPosTrans.x();
	int dyt = event->y() - lastPosTrans.y();

	if (event->buttons() & Qt::LeftButton) {
		//setXRotation(xRot + 8 * dy);
		//setZRotation(zRot + 8 * dx);
		setxPosition(xShift + dxt);
		setyPosition(yShift - dyt);
	}else if (event->buttons() & Qt::RightButton) {
		zoom += (dyt+dxt)/10.0;
		zoom = std::max<float>( std::min<float>( zoom, ZOOM_MAX ), ZOOM_MIN );
	}

	lastPos = event->pos();
	lastPosTrans = event->pos();

	//printf("Rot: %i,%i,%i, Pos, (%i,%i,%i), zoom: %f\n",xRot,yRot,zRot,xShift,yShift,zShift,zoom);
	updateGL();

}

//Werte für die Anzeige setzen.
void RangeViewGL::setPosition(int xRot,int yRot, int zRot, int xMove, int yMove, int zMove, float zoom, int width, int height){
	resize(width,height);
	this->xRot = xRot;
	this->yRot = yRot;
	this->zRot = zRot;
	this->xShift = xMove;
	this->yShift = yMove;
	this->zShift = zMove;
	this->zoom = zoom;
	updateGL();
}


void RangeViewGL::wheelEvent(QWheelEvent *event)
{
	zoom -= (2.0*(float)event->delta()/120.0)*5;
	zoom = std::max<float>( std::min<float>( zoom, ZOOM_MAX ), ZOOM_MIN );
	event->accept();
	updateGL();
}

void RangeViewGL::normalizeAngle(int *angle)
{
	while (*angle < 0)
		*angle += 360 * 16;
	while (*angle > 360 * 16)
		*angle -= 360 * 16;
}


