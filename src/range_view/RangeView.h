/* 
 * File:   RangeViewGL.cc
 * Author: Matthias Goldhoorn (matthias.goldhoorn@dfki.de) 
 * 
 */

#ifndef RANGEVIEW_H
#define	RANGEVIEW_H

#include "image_view_old/ImageViewOld.h"

class QDESIGNER_WIDGET_EXPORT RangeView : public ImageViewOld
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Matthias Goldhoorn")

public:
    RangeView(QWidget *parent = NULL,bool use_openGL = false);
    virtual ~RangeView();

public slots:    
	void setOpenGL(bool flag);
	void setRangeScan(double *data, int size);
	void setRangeScan3(const QList<double> &data);
protected:
     void keyPressEvent ( QKeyEvent * event );

};

#endif	

