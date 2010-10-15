
#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#include <QtGui/QLineEdit>

#include <qwt-qt4/qwt_plot_curve.h>
#include <qwt-qt4/qwt_plot_marker.h>

#include "PlotItemOptionWidget.h"
#include "DataManager.h"

#include <iostream>

class OptionsDialog : public QDialog
{
  
  Q_OBJECT
  
  public:
    OptionsDialog(QWidget* parent, Qt::WindowFlags f = 0);
    void initializeLayout(std::vector<QwtPlotCurve*> curves, std::vector<QwtPlotMarker*> markers);
    std::map<int, QColor> getCurveColorMap();
    std::map<int, QColor> getMarkerColorMap();
    char getCSVDelimiter();
    
  public slots:
    void newCurveColorSelected(int curveId, const QColor& color);
    void newMarkerColorSelected(int curveId, const QColor& color);
    void okPressed();
    void cancelPressed();
  protected:
    QPushButton okButton;
    QPushButton cancelButton;
    QGridLayout gridLayout;
    QLabel curveLabel;
    QLabel borderLineLabel;
    QLabel exportLabel;
    QLabel csvLabel;
    QLineEdit csvEdit;
    std::vector<PlotItemOptionWidget*> curveWidgets;
    std::vector<PlotItemOptionWidget*> markerWidgets;
    std::map<int, QColor> curveColorMap;
    std::map<int, QColor> markerColorMap;
};

#endif // OPTIONSDIALOG_H
