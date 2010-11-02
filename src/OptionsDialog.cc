#include "OptionsDialog.h"

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
    borderWidget->updateExistingMarkers();
    curveWidget->updateExistingCurves();
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

void OptionsDialog::initializeLayout(std::vector<QwtPlotCurve*> curves, std::vector<QwtPlotMarker*> markers)
{
    delete(borderWidget);
    delete(curveWidget);
    delete(generalWidget);
    borderWidget = new BorderLineOptionDialog();
    curveWidget = new CurveOptionWidget();
    generalWidget = new GeneralOptionsWidget();
    tabWidget.clear();
    curveWidget->initializeLayout(curves);
    generalWidget->initializeLayout();
    borderWidget->initializeLayout(markers);
    tabWidget.addTab(generalWidget, tr("General"));
    tabWidget.addTab(borderWidget, tr("Border Lines"));
    tabWidget.addTab(curveWidget, tr("Curves"));
    gridLayout.addWidget(&tabWidget, 0, 0, 1, 2);
    gridLayout.addWidget(&okButton, 1, 0, 1, 1);
    gridLayout.addWidget(&cancelButton, 1, 1, 1, 1);
    setLayout(&gridLayout);
}

