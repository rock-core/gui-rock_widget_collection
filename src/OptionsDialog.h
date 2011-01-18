
#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QTabWidget>

#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>

#include "DataManager.h"
#include "GeneralOptionsWidget.h"
#include "BorderLineOptionDialog.h"
#include "CurveOptionWidget.h"
#include "AxisOptionsWidget.h"

#include <iostream>

/**
 * Options widget which is merely a container for three other wodgets.
 * Those are displayed within a tab widget
 */
class OptionsDialog : public QDialog
{
  
  Q_OBJECT
  
  public:
      /**
       * Constructor
       * @param parent parent of the widget if any
       * @param f flags, defaults to 0
       */
    OptionsDialog(QWidget* parent, Qt::WindowFlags f = 0);

    /**
     * Initializes the widget with the currently existing curves and markers
     * @param curves teh existing curves
     * @param markers the existing markers
     */
    void initializeLayout(std::vector<QwtPlotCurve*> curves, std::vector<QwtPlotMarker*> markers);

    /**
     * Retunr snew markers, created by an underlying widget. Returns an empty
     * vector if there are no markers.
     * @return the new markers, if any
     */
    std::vector<QwtPlotMarker*> getNewMarkers();

    /**
     * Returns a vector of amrkers which shall be deleted
     * @return vector conatining all markers that shall be deleted
     */
    std::vector<QwtPlotMarker*> getDeletedMarkers();
    
  public slots:
      /**
       * Updates all data with the content of the widgets, when the ok button
       * was pressed.
       */
    void okPressed();

    /**
     * Closes the dialog, but does not save anything
     */
    void cancelPressed();
  protected:
    /** Button to confirm options*/
    QPushButton okButton;
    /** Button to cancel options*/
    QPushButton cancelButton;
      /** The tab widget to display the option widgets*/
    QTabWidget tabWidget;
    /** The layout of the widget*/
    QGridLayout gridLayout;
    /** Generla widget to display general options*/
    GeneralOptionsWidget* generalWidget;
    /** Widget to display the Borderlines*/
    BorderLineOptionDialog* borderWidget;
    /** Widget to display the curves*/
    CurveOptionWidget* curveWidget;
    /** widget for axis configuration*/
    AxisOptionsWidget* axisWidget;
};

#endif // OPTIONSDIALOG_H
