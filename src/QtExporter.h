#ifndef QTEXPORTER_H
#define	QTEXPORTER_H

#include <Qt/qstring.h>
#include <QtGui/QWidget>
#include <QtGui/QPixmap>
#include <QtSvg/QSvgGenerator>
#include <QtGui/QPainter>
#include <QtGui/QPaintEngine>
#include <QtGui/QPaintEvent>
#include <QtGui/QPrintDialog>
#include <Qt/qapplication.h>
#include <QtGui/QFileDialog>
#include <Qt/qprinter.h>

#include <iostream>

/**
 * Simple class with some static methods to export Qt Objects
 * into something different.
 * <h1>Export QWidget as Images</h1>
 * There are two static methods to export a QWidget to an image.<br>
 * The first (exportWidgetAsImage) will take a filename and a format
 * in addition to the widget and save the widget to the specified location.<br>
 * The second one (exportWidgetAsImageWithDialog) simply takes
 * the widget and displays a customized save dialog which will ask the user
 * to give the filename and the format.
 * <p>
 * Supported formats:<br>
 * <ul>
 * <li>"PDF"</li>
 * <li>"SVG"</li>
 * <li>"JPEG"</li>
 * <li>"PNG"</li>
 * <li>"BMP"</li>
 * </ul>
 *
 * <h1>History</h1>
 * 2010-08-18 Initial revision
 *
 * <h1>Known Issues</h1>
 *
 * <h1>To-Do</h1>
 *
 * @author Bjoern Lueck <blueck@dfki.de>
 * @version 0.1
 */
class QtExporter
{

public:
    /**
     * Exports the given widget to an image
     * @param filename the full path and filename ehere the image shall be stored
     * @param widget the widget to export
     * @param format the format, see the class description for options
     */
    static void exportWidgetAsImage(QString filename, QWidget* widget, const char* format);

    /**
     * Exports a widget to an image. A Dialog will appear and ask the user for
     * filename and the format he wishes.
     * @param widget the widget to export
     */
    static void exportWidgetAsImagWithDialog(QWidget* widget);
    

};

#endif	/* QTEXPORTER_H */

