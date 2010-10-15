#include "OptionsDialog.h"

OptionsDialog::OptionsDialog(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f),
  okButton("Ok"), cancelButton("Cancel"), curveWidgets(500), markerWidgets(500),
        exportLabel(QString("Export")), csvLabel(QString("CSV Delimiter"))
{
  setMinimumSize(300, 400);
  setFixedSize(300, 400);
  csvEdit.setMaxLength(1);
  connect(&okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
  connect(&cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
}

void OptionsDialog::newCurveColorSelected(int curveId, const QColor& color)
{
  curveColorMap[curveId] = color;
}

void OptionsDialog::newMarkerColorSelected(int curveId, const QColor& color)
{
  markerColorMap[curveId] = color;
}

std::map<int, QColor> OptionsDialog::getCurveColorMap()
{
  return curveColorMap;
}

std::map<int, QColor> OptionsDialog::getMarkerColorMap()
{
  return markerColorMap;
}

void OptionsDialog::okPressed()
{
  setVisible(false);
  emit accepted();
}

void OptionsDialog::cancelPressed()
{
  setVisible(false);
}

char OptionsDialog::getCSVDelimiter()
{
    if(csvEdit.text().size() > 0)
    {
        return csvEdit.text().at(0).toAscii();
    }
    return ' ';
}

void OptionsDialog::initializeLayout(std::vector<QwtPlotCurve*> curves, std::vector<QwtPlotMarker*> markers)
{
  int yValue = 0;
  DataManager* dataManager = DataManager::getInstance();
  csvEdit.setText(QString(dataManager->getCSVDelimter()));

  gridLayout.addWidget(&exportLabel, yValue, 0, 1, 2);
  yValue++;
  gridLayout.addWidget(&csvLabel, yValue, 0, 1, 1);
  gridLayout.addWidget(&csvEdit, yValue, 1, 1, 1);
  yValue++;

  borderLineLabel.setText(QString("Border Lines:"));
  gridLayout.addWidget(&borderLineLabel, yValue, 0, 1, 2);
  yValue++;
  for(int i=0;i<markers.size();i++)
  {
    QwtPlotMarker* marker = markers[i];
    if(marker == NULL)
    {
      continue;
    }
    markerColorMap[i] = marker->linePen().color();
    PlotItemOptionWidget* widget = new PlotItemOptionWidget();
    widget->layoutWidget(i, marker->linePen());
    gridLayout.addWidget(widget, yValue, 0, 1, 2);
    markerWidgets[i] = widget;
    connect(widget, SIGNAL(colorSelected(int, const QColor&)), this, SLOT(newMarkerColorSelected(int, const QColor&)));
    yValue++;
  }
  curveLabel.setText(QString("Curves:"));
  gridLayout.addWidget(&curveLabel, yValue, 0, 1, 2);
  yValue++;
  for(int i=0;i<curves.size();i++)
  {
    QwtPlotCurve* curve = curves[i];
    if(curve == NULL)
    {
      continue;
    }
    curveColorMap[i] = curve->pen().color();
    PlotItemOptionWidget* widget = new PlotItemOptionWidget();
    widget->layoutWidget(i, curve->pen());
    gridLayout.addWidget(widget, yValue, 0, 1, 2);
    curveWidgets[i] = widget;
    connect(widget, SIGNAL(colorSelected(int, const QColor&)), this, SLOT(newCurveColorSelected(int, const QColor&)));
    yValue++;
  }
  gridLayout.addWidget(&okButton, yValue, 0);
  gridLayout.addWidget(&cancelButton, yValue, 1);
  setLayout(&gridLayout);
}

