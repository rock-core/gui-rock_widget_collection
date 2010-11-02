/* 
 * File:   CurveSelectionDialog.h
 * Author: blueck
 *
 * Created on 28. Oktober 2010, 13:35
 */

#ifndef CURVESELECTIONDIALOG_H
#define	CURVESELECTIONDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QListWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>

#include <iostream>

#include <qwt-qt4/qwt_plot_curve.h>

#include "CurveListItemWidget.h"

class CurveSelectionDialog : public QDialog
{

    Q_OBJECT

public:
    CurveSelectionDialog(QWidget* parent=0);
    virtual ~CurveSelectionDialog();
    void initializeLayout(std::vector<QwtPlotCurve*> curves);
    std::vector<QwtPlotCurve*> getSelectedCurves();
public slots:
    void okPressed();
    void cancelPressed();
    
protected:
    QGridLayout layout;
    QListWidget listWidget;
    QPushButton okButton;
    QPushButton cancelButton;

private:

};

#endif	/* CURVESELECTIONDIALOG_H */

