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

    sprintf(temp, "%c", manager->getCSVDelimter());
    std::cout << "{" << temp << "}" << std::endl;
    writeNode("csvDelimiter", temp);

    startNode("legend");
    writeAttribute("visible", manager->isDrawLegend() ? "1" : "0");
    const char* position;
    int iPosition = manager->getLegendPosition();
    switch(iPosition)
    {
        case 0:
            position = "left";
            break;
        case 1:
            position = "right";
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

    std::cout << "Writing " << markers.size() << " Borderlines" << std::endl;

    for(int i=0;i<markers.size();i++)
    {
        QwtPlotMarker* marker = markers[i];
        if(marker == NULL)
        {
            continue;
        }
        std::cout << "Have not NULL" << std::endl;
        // begin the borderlines
        startNode("borderline");
        writeAttribute("visible", "1");

        char temp[10];
        sprintf(temp, "%f", marker->xValue());
        writeNode("xposition", temp);
        sprintf(temp, "%f", marker->yValue());
        writeNode("yposition", temp);
        switch(marker->lineStyle())
        {
            case QwtPlotMarker::HLine:
                writeNode("type", "horizontal");
                break;
            case QwtPlotMarker::VLine:
                writeNode("type", "vertical");
                break;
            case QwtPlotMarker::Cross:
                writeNode("type", "cross");
                break;
            case QwtPlotMarker::NoLine:
                writeNode("type", "none");
                break;
        }
        sprintf(temp, "%d", marker->linePen().width());
        writeNode("width", "3");

        QColor lineColor = marker->linePen().color();
        startNode("lineColor");
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

