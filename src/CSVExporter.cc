#include "CSVExporter.h"

bool CSVExporter::exportCurveAsCSV(const QwtPlotCurve& curve, const char delimiter)
{
  QString types = ("Comma seperated values (*.csv)");
  QString filter;
  QString extension = ".csv";
  QString fn = QFileDialog::getSaveFileName(NULL, QString("Export as CSV"), QDir::homePath(), types, &filter);
  if(!fn.isEmpty())
  {
      if(fn.contains(extension))
      {
        fn.remove(extension);
      }
      if(filter.contains(extension))
      {
        fn += extension;
      }
      QwtArrayData& currentData  = (QwtArrayData&)curve.data();
      return CSVExporter::exportDataAsCSV(fn.toStdString().c_str(),
              currentData.xData().toStdVector(),
              currentData.yData().toStdVector(),
              delimiter);
  }
  return true;
}

bool CSVExporter::exportCurveAsCSV(std::vector<QwtPlotCurve*> curves, const char delimiter)
{
    QString types = ("Comma seperated values (*.csv)");
    QString filter;
    QString extension = ".csv";
    QString fn = QFileDialog::getSaveFileName(NULL, QString("Export as CSV"), QDir::homePath(), types, &filter);
    if(!fn.isEmpty())
    {
      if(fn.contains(extension))
      {
        fn.remove(extension);
      }
      bool addExtension = filter.contains(extension);
      for(int i=0;i<curves.size();i++)
      {
          QwtPlotCurve* curve = curves[i];
          if(curve != NULL)
          {
              QString filename(fn);
              filename += "_";
              filename += curve->title().text();
              if(addExtension)
              {
                filename += extension;
              }
              QwtArrayData& currentData  = (QwtArrayData&)curve->data();
              CSVExporter::exportDataAsCSV(filename.toStdString().c_str(),
                      currentData.xData().toStdVector(),
                      currentData.yData().toStdVector(),
                      delimiter);
          }
      }
    }
    return true;
}

bool CSVExporter::exportDataAsCSV(const char* filename, const std::vector<double> xValues, 
        const std::vector<double> yValues, const char delimiter)
{
    std::fstream file;
    file.open(filename, std::ios::out);
    for(int i=0;i<xValues.size();i++)
    {
        double xValue = xValues[i];
        double yValue = yValues[i];
        file << xValue << delimiter << yValue << std::endl;
    }
    file.close();
    return true;
}
