/* 
 * File:   PlotXMLWriter.h
 * Author: blueck
 *
 * Created on 3. Dezember 2010, 11:09
 */

#ifndef PLOTXMLWRITER_H
#define	PLOTXMLWRITER_H

#include "XMLConfigWriter.h"
#include "DataManager.h"

#include "stdio.h"
#include <vector>
#include "qwt_plot_marker.h"

/**
 * XML writer class to stor plotting widget data to a xml file.
 * It extends the XMLConfigWriter and implements the abstract method writeContent
 * to do so.
 * <p>
 * <h1>History</h1>
 * 12-15-2010 Created documentation
 * <p>
 * <h1>Todo</h1>
 * <ul>
 * <li>Put all extra data to the data manager</li>
 * </ul>
 */
class PlotXMLWriter : public XMLConfigWriter
{
public:
    /** Empty standard constructor*/
    PlotXMLWriter();

    /** Empty standard deconstructor*/
    virtual ~PlotXMLWriter() {};

    /**
     * Write xml content
     * @return true if successfull, false otherwise
     */
    bool writeContent();

    /**
     * Sets the markers to write to the file
     * @param markers the markers to write
     */
    void setMarker(std::vector<QwtPlotMarker*> markers) { this->markers = markers; };

    /**
     * Sets the ranges of the axis of the plot
     * @param xMin xaxis minimum
     * @param xMax x axis maximum
     * @param yMin yaxis minimum
     * @param yMax yaxis maximum
     */
    void setRanges(int xMin, int xMax, int yMin, int yMax);
protected:
    /** Vector with markers*/
    std::vector<QwtPlotMarker*> markers;
    /** xaxis min value*/
    int minX;
    /** xaxis max value*/
    int maxX;
    /** yaxis min value*/
    int minY;
    /** yaxis max value*/
    int maxY;
private:
    /** DataManager where all data shall be stored*/
    DataManager* manager;

};

#endif	/* PLOTXMLWRITER_H */

