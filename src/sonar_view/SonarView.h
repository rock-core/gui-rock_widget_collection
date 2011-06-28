/* 
 * File:   SonarViewGL.cc
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de) 
 * 
 */

#ifndef SONARVIEW_H
#define	SONARVIEW_H

#include "image_view/ImageView.h"
#include <base/samples/sonar_scan.h>

class QDESIGNER_WIDGET_EXPORT SonarView : public ImageView
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Matthias Goldhoorn")

public:
    SonarView(QWidget *parent = NULL,bool use_openGL = true);
    virtual ~SonarView();

public slots:    
	void setOpenGL(bool flag);
        void setSonarScan2(base::samples::SonarScan *scan);
        void setSonarScan(const char *data, int size, double angle,double timeBetweenBins ,bool fromBearing=false);
	void setPosition(double posX, double posY, double sigmaX, double sigmaY);
     	void setOrientation(const double orientation);
	void setDistance(double distance, double angle);

protected:
     void keyPressEvent ( QKeyEvent * event );
     double lastScale;

};

#endif	

