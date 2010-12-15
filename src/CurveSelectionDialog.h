/* 
 * File:   CurveSelectionDialog.h
 * Author: blueck
 *
 * Created on 28. Oktober 2010, 13:35
 */

#ifndef CURVESELECTIONDIALOG_H
#define	CURVESELECTIONDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>

#include <iostream>

#include <qwt_plot_curve.h>

#include "CurveListItemWidget.h"

/**
 * Dialog to select curves to export
 */
class CurveSelectionDialog : public QDialog
{

    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent parent widget of the widget
     */
    CurveSelectionDialog(QWidget* parent=0);

    /**
     * Destructor
     */
    virtual ~CurveSelectionDialog();

    /**
     * Initializes the layout of the dialog with the given curves
     * @param curves teh curves currently available
     */
    void initializeLayout(std::vector<QwtPlotCurve*> curves);

    /**
     * Returns teh curves which are currently selected
     * @return vector of QwtPlotCurve objects
     */
    std::vector<QwtPlotCurve*> getSelectedCurves();
public slots:
    /**
     * Called when the okButton was pressed
     */
    void okPressed();

    /**
     * Called when the cancel button was pressed
     */
    void cancelPressed();
    
protected:
    /** Layout of the dialog*/
    QGridLayout layout;
    /** List widget to select the curves*/
    QListWidget listWidget;
    /** Button to accept*/
    QPushButton okButton;
    /** Button to cancel*/
    QPushButton cancelButton;

private:

};

#endif	/* CURVESELECTIONDIALOG_H */

