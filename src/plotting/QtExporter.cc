/* 
 * File:   QtExporter.cc
 * Author: blueck
 * 
 * Created on 18. August 2010, 15:02
 */

#include "QtExporter.h"

void QtExporter::exportWidgetAsImage(QString filename, QWidget* widget, const char* format)
{
    if(strcmp(format, "PDF") == 0)
    {
        QPrinter printer(QPrinter::ScreenResolution);
        printer.setOutputFileName(filename.toStdString().c_str());
        printer.setCreator("DFKI");
        printer.setOrientation(QPrinter::Landscape);
        widget->render(&printer);
    }
    else if(strcmp(format,"SVG") == 0)
    {
        QSvgGenerator generator;
        generator.setFileName(filename);
        generator.setSize(QSize(800, 600));
        QPixmap pixmap = QPixmap::grabWidget(widget);
        QPainter painter(&generator);
        painter.drawPixmap(0,0,-1,-1,pixmap);
    }
    else
    {
        QPixmap qPix = QPixmap::grabWidget(widget);
        if(qPix.isNull())
        {
            qDebug("Failed to capture the widget for saving");
            return;
        }
        qPix.save(filename, format);
    }
}

void QtExporter::exportWidgetAsImagWithDialog(QWidget* widget)
{
    // Set up the possible graphics formats
    QString types( "JPEG file (*.jpeg);;"
      "Portable Network Graphics file (*.png);;"
      "Bitmap file (*.bmp);;"
      "Scalable Vector Graphics file (*.svg);;"
      "Portable Document Format (*.pdf)");
    // Image type filter
    QString filter;
    // Suffix for the files
    QString jpegExt=".jpeg", pngExt=".png", bmpExt=".bmp", svgExt=".svg", pdfExt=".pdf";
    QString fn = QFileDialog::getSaveFileName(widget, QString("Save Image"), QDir::homePath(), types, &filter);
    if(!fn.isEmpty())
    {
      // If filename is not a null
      // Remove file extension which are already there
      if(fn.contains(jpegExt))
      {
        fn.remove(jpegExt);
      }
      else if(fn.contains(pngExt))
      {
        fn.remove(pngExt);
      }
      else if(fn.contains(bmpExt))
      {
        fn.remove(bmpExt);
      }
      else if(fn.contains(svgExt))
      {
          fn.remove(svgExt);
      }
      else if(fn.contains(pdfExt))
      {
          fn.remove(pdfExt);
      }
      // save according to filter
      if(filter.contains(jpegExt))
      {
        fn += jpegExt;
        exportWidgetAsImage(fn, widget, "JPEG");
      }
      else if(filter.contains(pngExt))
      {
        fn += pngExt;
        exportWidgetAsImage(fn, widget, "PNG");
      }
      else if(filter.contains(bmpExt))
      {
        fn += bmpExt;
        exportWidgetAsImage(fn, widget, "BMP");
      }
      else if(filter.contains(svgExt))
      {
        fn += svgExt;
        exportWidgetAsImage(fn, widget, "SVG");
      }
      else if(filter.contains(pdfExt))
      {
        fn += pdfExt;
        exportWidgetAsImage(fn, widget, "PDF");
      }
    }
}
