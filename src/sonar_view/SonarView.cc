/* 
 * File:   SonarView.cc
 * Author: blueck
 * 
 * Created on 17. Juni 2010, 14:14
 */

#include <stdexcept>
#include "SonarView.h"
#include <QtCore/QtPlugin>

using namespace base::samples::frame;

SonarView::SonarView(QWidget *parent,bool use_openGL):
ImageView(parent,use_openGL)
{

}


SonarView::~SonarView()
{
}

