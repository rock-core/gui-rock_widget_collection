/* 
 * File:   GeneralOptionsWidget.h
 * Author: blueck
 *
 * Created on 19. Oktober 2010, 12:20
 */

#ifndef GENERALOPTIONSWIDGET_H
#define	GENERALOPTIONSWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QFormLayout>

#include "ColorPickerButton.h"

#include <iostream>

class GeneralOptionsWidget : public QWidget
{

    Q_OBJECT

public:
    GeneralOptionsWidget(QColor currentBGColor);
    virtual ~GeneralOptionsWidget();
    void initializeLayout();
    QString getXAxisTitle() { return xAxisTextEdit.text(); };
    QString getYAxisTitle() { return yAxisTextEdit.text(); };
    QColor getBGColor() { return bgButton.getDisplayColor(); };
    char getCSVDelimiter();

protected:
    QLabel bgLabel;
    QLabel xAxisText;
    QLabel yAxisText;
    QLabel csvDelimiter;

    ColorPickerButton bgButton;
    QLineEdit xAxisTextEdit;
    QLineEdit yAxisTextEdit;
    QLineEdit csvDelimiterEdit;

    QFormLayout layout;

private:

};

#endif	/* GENERALOPTIONSWIDGET_H */

