/* 
 * File:   RangeViewGL.cc
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de) 
 * 
 */

#ifndef RANGEVIEW_H
#define	RANGEVIEW_H

#include "image_view/ImageView.h"
#include <base/samples/laser_scan.h>

class QDESIGNER_WIDGET_EXPORT RangeView : public ImageView
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Matthias Goldhoorn")

public:
    RangeView(QWidget *parent = NULL,bool use_openGL = false);
    virtual ~RangeView();

public slots:    
	void setOpenGL(bool flag);
        void setRangeScan2(base::samples::LaserScan *scan);
	void setRangeScan(double *data, int size);
	void setRangeScan3(const QList<double> &data);
protected:
     void keyPressEvent ( QKeyEvent * event );

};

#endif	

