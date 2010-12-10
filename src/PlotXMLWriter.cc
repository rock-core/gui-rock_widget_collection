/* 
 * File:   PlotXMLWriter.cc
 * Author: blueck
 * 
 * Created on 3. Dezember 2010, 11:09
 */

#include <QtCore/qstring.h>
#include <QtGui/qcolor.h>
#include <qwt-qt4/qwt_plot_marker.h>
#include <QtGui/qpen.h>

#include "PlotXMLWriter.h"

PlotXMLWriter::PlotXMLWriter()
{
    manager = DataManager::getInstance();
}

bool PlotXMLWriter::writeContent()
{
    startNode("plot");

    startNode("title");
    writeAttribute("axis", "x");
    writeNodeContent(manager->getXAxisTitle().toStdString().c_str());
    endNode();
    startNode("title");
    writeAttribute("axis", "y");
    writeNodeContent(manager->getYAxisTitle().toStdString().c_str());
    endNode();

    QColor color = manager->getBGColor();
    startNode("bgcolor");
    char temp[3];
    sprintf(temp, "%d", color.red());
    writeNode("red", temp);
    sprintf(temp, "%d", color.green());
    writeNode("green", temp);
    sprintf(temp, "%d", color.blue());
    writeNode("blue", temp);
    endNode();

    char delim[1] = {manager->getCSVDelimter()};
    writeNode("csvDelimiter", delim);

    startNode("legend");
    writeAttribute("visible", manager->isDrawLegend() ? "1" : "0");
    const char* position;
    int iPosition = manager->getLegendPosition();
    switch(iPosition)
    {
        case 0:
            position = "horizontal";
            break;
        case 1:
            position = "vertical";
            break;
        case 2:
            position = "bottom";
            break;
        default:
            position = "top";

    }
    writeNode("pos", position);
    endNode();

    writeNode("autoscrolling", manager->isAutoscrolling() ? "1" : "0");

    startNode("showGrid");
    writeAttribute("axis", "x");
    writeNodeContent(manager->isDrawXGrid() ? "1" : "0");
    endNode();
    startNode("showGrid");
    writeAttribute("axis", "y");
    writeNodeContent(manager->isDrawYGrid() ? "1" : "0");
    endNode();

    startNode("showSlider");
    writeAttribute("axis", "x");
    writeNodeContent(manager->isShowBottomSlider() ? "1" : "0");
    endNode();
    startNode("showSlider");
    writeAttribute("axis", "y");
    writeNodeContent(manager->isShowLeftSlider() ? "1" : "0");
    endNode();

    startNode("axisrange");
    writeAttribute("axis", "x");
    char range[10];
    sprintf(range, "%d", minX);
    writeNode("min", range);
    sprintf(range, "%d", maxX);
    writeNode("max", range);
    endNode();

    startNode("axisrange");
    writeAttribute("axis", "y");
    sprintf(range, "%d", minY);
    writeNode("min", range);
    sprintf(range, "%d", maxY);
    writeNode("max", range);
    endNode();

    // end plot
    endNode();

    for(int i=0;i<markers.size();i++)
    {
        // begin the borderlines
        startNode("borderline");
        writeAttribute("visible", "1");
        writeNode("xposition", "25");
        writeNode("yposition", "12");
        writeNode("type", "horizontal");
        writeNode("width", "3");

        QColor lineColor = markers[i]->linePen().color()

        startNode("linecolor");
        char temp[3];
        sprintf(temp, "%d", lineColor.red());
        writeNode("red", temp);
        sprintf(temp, "%d", lineColor.green());
        writeNode("green", temp);
        sprintf(temp, "%d", lineColor.blue());
        writeNode("blue", temp);
        endNode();
        // end borderline
        endNode();
    }
    
    return true;
}


void PlotXMLWriter::setRanges(int xMin, int xMax, int yMin, int yMax)
{
    minX = xMin;
    minY = yMin;
    maxX = xMax;
    maxY = yMax;
}

