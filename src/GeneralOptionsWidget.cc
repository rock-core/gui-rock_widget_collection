/* 
 * File:   GeneralOptionsWidget.cc
 * Author: blueck
 * 
 * Created on 19. Oktober 2010, 12:20
 */

#include "GeneralOptionsWidget.h"
#include "DataManager.h"

GeneralOptionsWidget::GeneralOptionsWidget() : 
    bgButton(QColor(0, 0, 0)), layout(this), xAxisText(tr("X Title")),
        yAxisText(tr("Y Title")), bgLabel(tr("Background color")),
        csvDelimiter(tr("CSV Delimiter"))
{
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
    layout.addRow(&xAxisText, &xAxisTextEdit);
    layout.addRow(&yAxisText, &yAxisTextEdit);
    layout.addRow(&bgLabel, &bgButton);
    layout.addRow(&csvDelimiter, &csvDelimiterEdit);
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

