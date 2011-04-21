#include "OptionsDialog.h"
#include "DataManager.h"

OptionsDialog::OptionsDialog(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f),
  okButton("Ok"), cancelButton("Cancel"),
        tabWidget(this)
{
  setMinimumSize(400, 400);
  setFixedSize(400, 400);
  setWindowTitle(tr("Options"));
  generalWidget = new GeneralOptionsWidget();
  borderWidget = new BorderLineOptionDialog();
  curveWidget = new CurveOptionWidget();
  axisWidget = new AxisOptionsWidget();
//  csvEdit.setMaxLength(1);
  connect(&okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
  connect(&cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
}

void OptionsDialog::okPressed()
{
    DataManager* dataManager = DataManager::getInstance();
    dataManager->setBGColor(generalWidget->getBGColor());
    dataManager->setXAxisTitle(generalWidget->getXAxisTitle());
    dataManager->setYAxisTitle(generalWidget->getYAxisTitle());
    dataManager->setCSVDelimiter(generalWidget->getCSVDelimiter());
    dataManager->setDrawLegend(generalWidget->isDrawLegend());
    dataManager->setLegendPosition(generalWidget->getLegendPosition());
    dataManager->setZoomerDecimalSize(generalWidget->getZoomerDecimalPoint());
    dataManager->setTimeDivisor((DataManager::TimeDivisor)generalWidget->getTimeDivisor());
    borderWidget->updateExistingMarkers();
    curveWidget->updateExistingCurves();
    axisWidget->updateDataManager();
    setVisible(false);
    emit accepted();
}

void OptionsDialog::cancelPressed()
{
    setVisible(false);
}

std::vector<QwtPlotMarker*> OptionsDialog::getNewMarkers()
{
    return borderWidget->getNewMarkers();
}

std::vector<QwtPlotMarker*> OptionsDialog::getDeletedMarkers()
{
    return borderWidget->getDeletedMarkers();
}

void OptionsDialog::initializeLayout(std::vector<QwtPlotCurve*> curves, std::vector<QwtPlotMarker*> markers)
{
    delete(borderWidget);
    delete(curveWidget);
    delete(generalWidget);
    delete(axisWidget);
    borderWidget = new BorderLineOptionDialog();
    curveWidget = new CurveOptionWidget();
    generalWidget = new GeneralOptionsWidget();
    axisWidget = new AxisOptionsWidget();
    tabWidget.clear();
    curveWidget->initializeLayout(curves);
    generalWidget->initializeLayout();
    borderWidget->initializeLayout(markers);
    axisWidget->initializeLayout();
    tabWidget.addTab(generalWidget, tr("General"));
    tabWidget.addTab(axisWidget, tr("Axis"));
    tabWidget.addTab(borderWidget, tr("Border Lines"));
    tabWidget.addTab(curveWidget, tr("Curves"));
    gridLayout.addWidget(&tabWidget, 0, 0, 1, 2);
    gridLayout.addWidget(&okButton, 1, 0, 1, 1);
    gridLayout.addWidget(&cancelButton, 1, 1, 1, 1);
    setLayout(&gridLayout);
}

