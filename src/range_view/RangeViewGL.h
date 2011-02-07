/*
 * File:   RangeViewGL.h
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de)
 *
 */

#ifndef RANGEVIEWGL_H
#define	RANGEVIEWGL_H

#include "image_view/ImageViewGL.h"
#include <QTimer>
#include <QMutex>
#include <Eigen/Core>

class RangeViewGL : public ImageViewGL
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Matthias Goldhoorn")



public:
	RangeViewGL(ImageView &parent);
	virtual ~RangeViewGL();
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void setPosition(int xRot,int yRot, int zRot, int xMove, int yMove, int zMove, float zoom, int width, int height);
	void setData(const std::vector<Eigen::Vector3d> data);
	void keyPressEvent ( QKeyEvent * event );

public slots:
	void repaintFunc();
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void setxPosition(int value);
	void setyPosition(int value);
	void setzPosition(int value);
	
	void initializeGL();
	void paintGL();
	void resizeGL(int, int);
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void wheelEvent(QWheelEvent*);

protected:

private:

	std::vector<Eigen::Vector3d> data;
	GLuint groundPlane;
	void normalizeAngle(int *angle);
	float zoom;
	static const float ZOOM_MAX = 6000.0;
	static const float ZOOM_MIN = -6000.0;


	inline double max(double a,double b){
	return (a>b)?a:b;
	}


	int xRot;
	int yRot;
	int zRot;
	int xShift;
	int yShift;
	int zShift;
	QPoint lastPos;
	QPoint lastPosTrans;
	QMutex mutex;

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);
	void xPositionChanged(int value);
	void yPositionChanged(int value);
	void zPositionChanged(int value);

};

#endif	

