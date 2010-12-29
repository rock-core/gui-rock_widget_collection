/* 
 * File:   ExtendedPlotZoomer.h
 * Author: blueck
 *
 * Created on 29. Dezember 2010, 11:17
 */

#ifndef EXTENDEDPLOTZOOMER_H
#define	EXTENDEDPLOTZOOMER_H

#include "qwt_plot_zoomer.h"
#include "DataManager.h"

#include <iostream>

/**
 * Class which derives from QwtPlotZoomer. The only changed behaviour is the
 * length of the trackerText. One can configure the number of digits shown after
 * a decimal point via the DataManager class.
 */
class ExtendedPlotZoomer : public QwtPlotZoomer
{
public:
    /**
     * Overwritten constructor, initializing the DataManager
     * @param canvas teh canvas the zoomer belongs to. The parameters
     * are passed to the super class constructor.
     * @param doReplot if a replot should be done
     */
    ExtendedPlotZoomer(QwtPlotCanvas* canvas, bool doReplot=true);

    /**
     * Destructor
     */
    virtual ~ExtendedPlotZoomer();

protected:
    /**
     * Returns the tracker Text for the given position. The number
     * of digits after the decimal point is taken from the DataManager.
     * @param pos the position for which text shall be returned
     * @return the text matching the current postion
     */
    QwtText trackerText(const QwtDoublePoint &pos) const;

private:
    /** Pointer to the DataManager singelton */
    DataManager* dataManager;

};

#endif	/* EXTENDEDPLOTZOOMER_H */

