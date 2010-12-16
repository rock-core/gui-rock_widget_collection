#ifndef BORDERLINEOPTIONDIALOG_H
#define	BORDERLINEOPTIONDIALOG_H


#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include <QIcon>
#include <QCheckBox>
#include <QMessageBox>

#include <qwt_plot_marker.h>

#include "ColorPickerButton.h"

#include <iostream>

/**
 * Widget which displays all Properties of a QwtPlotMarker.
 * Markers can be added via the add button.
 * The following properties can be configured:<br>
 * <ul>
 * <li> Color </li>
 * <li> xValue </li>
 * <li> yValue </li>
 * <li> Line Style </li>
 * <li> Line Orientation </li>
 * <li> Line width </li>
 * </ul>
 *
 * <h2>History</h2>
 * 2010-10-22 Initial revision
 *
 * <h2>Known Issues</h2>
 *
 * <h2>To-Do</h2>
 *
 * @author Bjoern Lueck <blueck@dfki.de>
 * @version 0.1
 */
class BorderLineOptionDialog : public QWidget
{

    Q_OBJECT

public:
    /**
     * Standard constructor
     */
    BorderLineOptionDialog();

    /**
     * Destructor freeing and clearing all vectors
     */
    virtual ~BorderLineOptionDialog();

    /**
     * Initializes the layout with all existing markers, if any. The vector
     * may be zero size.
     * @param markers The currently available markers
     */
    void initializeLayout(std::vector<QwtPlotMarker*> markers);

    /**
     * Returns the newly created markers
     * @return the new markers
     */
    std::vector<QwtPlotMarker*> getNewMarkers();

    /**
     * Returns a vector of amrkers which shall be deleted
     * @return vector conatining all markers that shall be deleted
     */
    std::vector<QwtPlotMarker*> getDeletedMarkers();

    /**
     * Updates the existing markers with the values changed withn the dialog
     */
    void updateExistingMarkers();

public slots:
    /**
     * Adds a new Marker to the dialog, displaying all information.
     * The marker will then be available in the vector returned by
     * @see#getNewMarkers()
     */
    void addMarker();

    /**
     * Called when the link on the close label is activated
     */
    void linkActivated();

protected:
    /**
     * Helper method which adds a Marker to the dialogs layout
     * @param marker pointer to a marker to add
     * @param index the index at which the marker shall be displayed
     */
    void addMarkerToLayout(QwtPlotMarker* marker, int index);
    /** The Layout of the dialog*/
    QGridLayout layout;
    /** Button to add a new marker*/
    QPushButton addButton;
    /** Buttons to change the colors*/
    std::vector<ColorPickerButton*> buttons;
    /** SpinBoxes to change the xValue*/
    std::vector<QDoubleSpinBox*> xSpinBoxes;
    /** SpinBoxes to change the y Boxes*/
    std::vector<QDoubleSpinBox*> ySpinBoxes;
    /** Comboboxes to change the Line Orientation*/
    std::vector<QComboBox*> comboBoxes;
    /** Comboboxes to change the Line Style*/
    std::vector<QComboBox*> styleBoxes;
    /** SpinBoxes to cahnge the markers width*/
    std::vector<QSpinBox*> weightBoxes;
    /** All exit Labels*/
    std::vector<QLabel*> exitLabels;
    /** Contains whether a Marker was deleted*/
    std::vector<bool> markerDeleted;
    /** Vector with all checkboxes to simply hide the Border Line*/
    std::vector<QCheckBox*> hiddenBoxes;
    /** All Frames*/
    std::vector<QFrame*> frames;

    /** Vector containing newly created Markers*/
    std::vector<QwtPlotMarker*> newMarkers;
    /** Vector containing all existing markers*/
    std::vector<QwtPlotMarker*> oldMarkers;
    /** Scroll Area if more markers are available*/
    QScrollArea scrollArea;
    /** The layot of the top level frame*/
    QFrame layoutFrame;
    /** Top Level frame to put into the scroll area*/
    QGridLayout topLevelLayout;
private:
    /** yValue to put widgets within the layout to*/
    int yValue;
    /** The current index of markers*/
    int currentIndex;
    /** Size of the old Markers*/
    int oldSize;
};

#endif	/* BORDERLINEOPTIONDIALOG_H */

