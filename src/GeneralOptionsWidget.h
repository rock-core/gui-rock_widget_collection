/* 
 * File:   GeneralOptionsWidget.h
 * Author: blueck
 *
 * Created on 19. Oktober 2010, 12:20
 */

#ifndef GENERALOPTIONSWIDGET_H
#define	GENERALOPTIONSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>

#include "ColorPickerButton.h"

#include <iostream>

/**
 * Widget which displays general options of the plot.
 * Namingly:
 * <ul>
 * <li>x-axis title</li>
 * <li>y-axis title</li>
 * <li>background color</li>
 * <li>delimiter for csv files</li>
 * </ul>
 */
class GeneralOptionsWidget : public QWidget
{

    Q_OBJECT

public:
    /**
     * Constructor
     */
    GeneralOptionsWidget();

    /**
     * Destructor
     */
    virtual ~GeneralOptionsWidget();

    /**
     * Initializes the layout
     */
    void initializeLayout();

    /**
     * Return the title of the x axis
     * @return the title of the x axis
     */
    QString getXAxisTitle() { return xAxisTextEdit.text(); };

    /**
     * Return the title of the y axis
     * @return the title of the y axis
     */
    QString getYAxisTitle() { return yAxisTextEdit.text(); };

    /**
     * Returns the background color
     * @return the color of the background
     */
    QColor getBGColor() { return bgButton.getDisplayColor(); };

    /**
     * Returns the number of numbers after the decimal point
     * @return number of numbers after the floating poingt
     */
    int getZoomerDecimalPoint() { return decimalSpinBox.value(); };

    /**
     * Returns the csv delimiter
     * @return the delimiter for csv files
     */
    char getCSVDelimiter();

    /**
     * Returns wehther the legend will be drawn
     * @return whether the legend will be drawn
     */
    bool isDrawLegend() { return legendCheckBox.isChecked(); };

    /**
     * Returns the legend position
     * @return the legend position
     */
    int getLegendPosition() { return legendBox.currentIndex(); };

protected:


    /**Button to change the color*/
    ColorPickerButton bgButton;
    /** Layout of the widget*/
    QFormLayout layout;
    /**Line edit for the x axis text*/
    QLineEdit xAxisTextEdit;
    /**Line edit for the y axis text*/
    QLineEdit yAxisTextEdit;
    /**Line edit for a delimiter only 1 character can be entered*/
    QLineEdit csvDelimiterEdit;

    /** Label for the bg color*/
    QLabel bgLabel;
    /** Label for the x axis text*/
    QLabel xAxisText;
    /** Label for the y axis text*/
    QLabel yAxisText;
    /** LAbel for the delimiter*/
    QLabel csvDelimiter;
    /** CheckBox whether legends shall be displayed*/
    QCheckBox legendCheckBox;
    /** ComboBox, where the legend shall be displayed*/
    QComboBox legendBox;
    /** Label to show the decimal number after the dot*/
    QLabel decimalPoint;
    /** SpinBox for the decimal numbers*/
    QSpinBox decimalSpinBox;

private:

};

#endif	/* GENERALOPTIONSWIDGET_H */

