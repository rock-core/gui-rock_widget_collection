/* 
 * File:   BorderLineOptionDialog.h
 * Author: blueck
 *
 * Created on 19. Oktober 2010, 14:36
 */

#ifndef BORDERLINEOPTIONDIALOG_H
#define	BORDERLINEOPTIONDIALOG_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>

#include <qwt-qt4/qwt_plot_marker.h>

#include "ColorPickerButton.h"

#include <iostream>

class BorderLineOptionDialog : public QWidget
{

    Q_OBJECT

public:
    BorderLineOptionDialog();
    virtual ~BorderLineOptionDialog();
    void initializeLayout(std::vector<QwtPlotMarker*> markers);
    std::vector<QwtPlotMarker*> getNewMarkers();
    void updateExistingMarkers();

public slots:
    void addMarker();

protected:
    void addMarkerToLayout(QwtPlotMarker* marker, int index);

    QGridLayout layout;
    QPushButton addButton;
    std::vector<ColorPickerButton*> buttons;
    std::vector<QDoubleSpinBox*> xSpinBoxes;
    std::vector<QDoubleSpinBox*> ySpinBoxes;
    std::vector<QComboBox*> comboBoxes;
    std::vector<QComboBox*> styleBoxes;
    std::vector<QSpinBox*> weightBoxes;

    std::vector<QwtPlotMarker*> newMarkers;
    std::vector<QwtPlotMarker*> oldMarkers;
private:
    int yValue;
    int currentIndex;
    int oldSize;
};

#endif	/* BORDERLINEOPTIONDIALOG_H */

