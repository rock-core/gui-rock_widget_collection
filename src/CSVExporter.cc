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
      std::fstream file;
      file.open(fn.toStdString().c_str(), std::ios::out);
      QwtArrayData& currentData  = (QwtArrayData&)curve.data();
      for(int i=0;i<currentData.size();i++)
      {
        double xValue = currentData.x(i);
        double yValue = currentData.y(i);
        file << xValue << delimiter << yValue << std::endl;
      }
      file.close();
  }
  return true;
}
