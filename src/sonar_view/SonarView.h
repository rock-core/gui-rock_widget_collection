/*
 * File:   SonarView.h
 * Author: Alexander DUda
 *
 * Created on 18. January 2011, 14:14
 */

#ifndef SONARVIEW_H
#define	SONARVIEW_H

#include "image_view/ImageView.h"

class QDESIGNER_WIDGET_EXPORT SonarView : public ImageView
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Alexander Duda")

public:
    SonarView(QWidget *parent = NULL,bool use_openGL = false);
    virtual ~SonarView();

};

#endif	

