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


class PlotXMLReader : public XMLConfigReader
{
public:
    PlotXMLReader();
    virtual ~PlotXMLReader();
    bool readXMLSpecifica();
    std::vector<QwtPlotMarker*> getMarker() { return borderLines; };
    int getMinX() { return minX; };
    int getMinY() { return minY; };
    int getMaxX() { return maxX; };
    int getMaxY() { return maxY; };
private:
    DataManager* manager;
    int minX;
    int minY;
    int maxX;
    int maxY;
    std::vector<QwtPlotMarker*> borderLines;

};

#endif	/* PLOTXMLREADER_H */

