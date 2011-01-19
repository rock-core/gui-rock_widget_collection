#ifndef CSVEXPORTER_H
#define CSVEXPORTER_H

#include <qwt_plot_curve.h>
#include <fstream>
#include <iostream>
#include <string>
#include <QFileDialog>

/**
 * Class which contains static methods to export data to a csv file.
 *
 * <h2>History</h2>
 * 2010-10-22 Initial revision
 *
 * <h2>Known Issues</h2>
 * Decimal values will be exported with the '.' character, so using
 * the '.' character as a delimiter will result in undesired behaviour or
 * not working at all.
 *
 * @todo Remove the qwt specific method and put them in a derived/different class, so the exporter can be used even without any QT/QWT linking
 *
 * @author Bjoern Lueck <blueck@dfki.de>
 * @version 0.1
 */
class CSVExporter
{
  
  public:
      /**
       * Exports a QwtPlotCurve as a csv file. All points whithin the
       * plot will be exported, visible or not.
       * Note that doubles will be exported with a dot as floating point. Thus
       * exporting with the '.' character as a delimiter will NOT work properly
       * with dloating point values.
       * The method will display a FileDialog to choose a filename from. It
       * therefore won't work with console applications.
       * @param curve The curve to export
       * @param delimiter the delimiter which shall be used within the csv file, ',' being the default
       * @return true if successfull, false otherwise
       */
      static bool exportCurveAsCSV(const QwtPlotCurve& curve, const char delimiter=',');

      /**
       * Exports multiple QwtPlotCurve as multiple csv files. All points whithin the
       * plot will be exported, visible or not.
       * Note that doubles will be exported with a dot as floating point. Thus
       * exporting with the '.' character as a delimiter will NOT work properly
       * with dloating point values.
       * The method will display a FileDialog to choose a filename from. It
       * therefore won't work with console applications. The choosen file will
       * be extended with the curves name. For example if the curves titles are
       * temperature and height and the filename given was log, then this would result in
       * two files written, one called log_temperature.csv and one called log_height.csv.
       * @param curves The curves to export
       * @param delimiter the delimiter which shall be used within the csv file, ',' being the default
       * @return true if successfull, false otherwise
       */
      static bool exportCurveAsCSV(std::vector<QwtPlotCurve*> curves, const char delimiter=',');

      /**
       * Exports the specified x/y data to a file. Note that floating point
       * data will use the '.' character. So using a '.' character as delimiter will
       * NOT work. Note that the xVectors size is used in determining the length of the
       * list, e.g. if there are more y Values than x values, these will be ignored.
       * @param filename the file to be written
       * @param xValues the x values
       * @param yValues the y Values
       * @param delimiter the delimiter to use ',' being the default
       * @return true if successfull, false otherwise
       */
      static bool exportDataAsCSV(const char* filename, const std::vector<double> xValues, const std::vector<double> yValues, const char delimiter=',');
  
};

#endif // CSVEXPORTER_H
