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

class ExtendedPlotZoomer : public QwtPlotZoomer
{
public:
    ExtendedPlotZoomer(QwtPlotCanvas* canvas, bool doReplot=true);
    ExtendedPlotZoomer(const ExtendedPlotZoomer& orig);
    virtual ~ExtendedPlotZoomer();

protected:
    QwtText trackerText(const QwtDoublePoint &pos) const;

private:
    DataManager* dataManager;

};

#endif	/* EXTENDEDPLOTZOOMER_H */

