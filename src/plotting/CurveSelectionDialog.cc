/* 
 * File:   CurveSelectionDialog.cc
 * Author: blueck
 * 
 * Created on 28. Oktober 2010, 13:35
 */

#include "CurveSelectionDialog.h"

CurveSelectionDialog::CurveSelectionDialog(QWidget* parent) : QDialog(parent),
        okButton(tr("Ok")), cancelButton(tr("Cancel"))
{
    setWindowTitle(tr("Select Curves to export"));
    listWidget.setSelectionMode(QAbstractItemView::MultiSelection);
    QObject::connect(&okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
    QObject::connect(&cancelButton, SIGNAL(clicked()), this, SLOT(cancelPressed()));
}

void CurveSelectionDialog::initializeLayout(std::vector<QwtPlotCurve*> curves)
{
    int count = 0;
    listWidget.clear();
    for(unsigned int i=0;i<curves.size();i++)
    {
        QwtPlotCurve* curve = curves[i];
        if(curve != NULL)
        {
            count++;
            CurveListItemWidget* widget = new CurveListItemWidget(curve, QString(curve->title().text()));
            listWidget.addItem(widget);
        }
    }
    layout.addWidget(&listWidget, 0, 0, 1, 2);
    layout.addWidget(&okButton, 2, 0, 1, 1);
    layout.addWidget(&cancelButton, 2, 1, 1, 1);
    setLayout(&layout);
}

std::vector<QwtPlotCurve*> CurveSelectionDialog::getSelectedCurves()
{
    std::vector<QwtPlotCurve*> selectedCurves;
    QList<QListWidgetItem*> items = listWidget.selectedItems();
    for(int i=0;i<items.size();i++)
    {
        CurveListItemWidget* item = (CurveListItemWidget*)items[i];
        selectedCurves.push_back(item->getCurve());
    }
    return selectedCurves;
}

void CurveSelectionDialog::okPressed()
{
    setVisible(false);
    emit accepted();
}

void CurveSelectionDialog::cancelPressed()
{
    setVisible(false);
}

CurveSelectionDialog::~CurveSelectionDialog() { }

