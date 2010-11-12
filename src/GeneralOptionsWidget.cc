/* 
 * File:   GeneralOptionsWidget.cc
 * Author: blueck
 * 
 * Created on 19. Oktober 2010, 12:20
 */

#include <QtGui/qwidget.h>
#include <QtGui/qcombobox.h>

#include "GeneralOptionsWidget.h"
#include "DataManager.h"

GeneralOptionsWidget::GeneralOptionsWidget() : 
    bgButton(QColor(0, 0, 0)), layout(this), bgLabel(tr("Background color")),
        xAxisText(tr("X Title")),
        yAxisText(tr("Y Title")),
        csvDelimiter(tr("CSV Delimiter")),
        legendCheckBox(tr("Draw Legend"))
{
    legendBox.addItem("Left", QVariant(0));
    legendBox.addItem("Right", QVariant(1));
    legendBox.addItem("Bottom", QVariant(2));
    legendBox.addItem("Top", QVariant(3));
    connect(&legendCheckBox, SIGNAL(clicked(bool)), &legendBox, SLOT(setEnabled(bool)));
}


void GeneralOptionsWidget::initializeLayout()
{
    csvDelimiterEdit.setMaxLength(1);
    DataManager* dataManager = DataManager::getInstance();
    bgButton.setDisplayColor(dataManager->getBGColor());
    csvDelimiterEdit.setText(QString(dataManager->getCSVDelimter()));
    layout.setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    xAxisTextEdit.setText(dataManager->getXAxisTitle());
    yAxisTextEdit.setText(dataManager->getYAxisTitle());
    legendBox.setCurrentIndex(dataManager->getLegendPosition());
    legendCheckBox.setChecked(dataManager->isDrawLegend());
    layout.addRow(&xAxisText, &xAxisTextEdit);
    layout.addRow(&yAxisText, &yAxisTextEdit);
    layout.addRow(&bgLabel, &bgButton);
    layout.addRow(&csvDelimiter, &csvDelimiterEdit);
    layout.addRow(&legendCheckBox, &legendBox);
    setLayout(&layout);
}

char GeneralOptionsWidget::getCSVDelimiter()
{
    if(csvDelimiterEdit.text().size() > 0)
    {
        return csvDelimiterEdit.text().at(0).toAscii();
    }
    else
    {
        return ' ';
    }
}

GeneralOptionsWidget::~GeneralOptionsWidget() { }

