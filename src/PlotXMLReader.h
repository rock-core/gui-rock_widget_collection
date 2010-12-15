/* 
 * File:   PlotXMLReader.h
 * Author: blueck
 *
 * Created on 26. November 2010, 13:50
 */

#ifndef PLOTXMLREADER_H
#define	PLOTXMLREADER_H

#include "XMLConfigReader.h"

#include "DataManager.h"

#include "qwt_plot_marker.h"

#include "xpath.h"

#include <QColor>
#include <QPen>

/**
 * Class which reads xml files for the plotting widget.
 * It extends the XMLConfigReader and adds specifa.
 * <p>
 * <h1>History</h1>
 * 12-15-2010 Created documentation
 * <p>
 * <h1>Todo</h1>
 * <ul>
 * <li>Put all extra data in the data manager</li>
 * </ul>
 *
 */
class PlotXMLReader : public XMLConfigReader
{
public:
    /**
     * Empty constructor
     */
    PlotXMLReader();

    /**
     * Empty deconstructor
     */
    virtual ~PlotXMLReader();

    /**
     * Overwritten method from XMLConfigReader. Teh actual xml reading is here
     * @return true if successfull, false otherwise
     */
    bool readXMLSpecifica();

    /**
     * Returns the markers contained in the xml file
     * @return vector of markers
     */
    std::vector<QwtPlotMarker*> getMarker() { return borderLines; };

    /**
     * min x-axis value in the xml file
     * @return the min x-axis value
     */
    int getMinX() { return minX; };

    /**
     * min y-axis value in the xml file
     * @return the min y-axis value
     */
    int getMinY() { return minY; };

    /**
     * Returns the max x-axis value
     * @return  the max x-axis value
     */
    int getMaxX() { return maxX; };

    /**
     * Returns the max y-axis value
     * @return the max y-axis value
     */
    int getMaxY() { return maxY; };
private:
    /** DataManager pointer to store configuration data*/
    DataManager* manager;
    /** xaxis min value*/
    int minX;
    /** y axis min value*/
    int minY;
    /** xaxis max value*/
    int maxX;
    /** yaxis max value*/
    int maxY;
    /** Vector of all borderlines within the xml file if any (zero length)*/
    std::vector<QwtPlotMarker*> borderLines;

};

#endif	/* PLOTXMLREADER_H */

