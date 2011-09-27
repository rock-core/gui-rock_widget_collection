/* 
 * File:   SonarDisplay.h
 * Author: blueck
 *
 * Created on 17. Juni 2010, 14:14
 */

#ifndef SONARDISPLAY_H
#define	SONARDISPLAY_H

#include <MultiWidget.h>
#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QPen>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtGui/qfiledialog.h>
#include <QtGui/QMenu>
#include <QtCore/QString>
#include <QtDesigner/QDesignerExportWidget>
#include <QVTKWidget.h>

#include <iostream>


class QDESIGNER_WIDGET_EXPORT SonarDisplay : public QVTKWidget 
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Alexander Duda")

public:
    SonarDisplay(QWidget *parent = NULL);
    virtual ~SonarDisplay();

public slots:
    void addRawImage(const QString &mode, int pixel_size, int width, int height,const char* pbuffer);

protected:
};

#endif	/* IMAGEVIEWWIDGET_H */

