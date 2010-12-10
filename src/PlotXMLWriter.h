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

class PlotXMLWriter : public XMLConfigWriter
{
public:
    PlotXMLWriter();
    virtual ~PlotXMLWriter() {};
    bool writeContent();
    void setMarker(std::vector<QwtPlotMarker*> markers) { this->markers = markers; };
    void setRanges(int xMin, int xMax, int yMin, int yMax);
protected:
    std::vector<QwtPlotMarker*> markers;
    int minX;
    int maxX;
    int minY;
    int maxY;
private:
    DataManager* manager;

};

#endif	/* PLOTXMLWRITER_H */

