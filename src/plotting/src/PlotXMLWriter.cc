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
    startTag("plot");

    startTag("title");
    writeAttribute("axis", "x");
    writeTagContent(manager->getXAxisTitle().toStdString().c_str());
    endTag();
    startTag("title");
    writeAttribute("axis", "y");
    writeTagContent(manager->getYAxisTitle().toStdString().c_str());
    endTag();

    QColor color = manager->getBGColor();
    startTag("bgcolor");
    char temp[3];
    sprintf(temp, "%d", color.red());
    writeTag("red", temp);
    sprintf(temp, "%d", color.green());
    writeTag("green", temp);
    sprintf(temp, "%d", color.blue());
    writeTag("blue", temp);
    endTag();

    sprintf(temp, "%c", manager->getCSVDelimter());
    std::cout << "{" << temp << "}" << std::endl;
    writeTag("csvDelimiter", temp);

    startTag("legend");
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
    writeTag("pos", position);
    endTag();

    writeTag("autoscrolling", manager->isAutoscrolling() ? "1" : "0");

    startTag("showGrid");
    writeAttribute("axis", "x");
    writeTagContent(manager->isDrawXGrid() ? "1" : "0");
    endTag();
    startTag("showGrid");
    writeAttribute("axis", "y");
    writeTagContent(manager->isDrawYGrid() ? "1" : "0");
    endTag();

    startTag("showSlider");
    writeAttribute("axis", "x");
    writeTagContent(manager->isShowBottomSlider() ? "1" : "0");
    endTag();
    startTag("showSlider");
    writeAttribute("axis", "y");
    writeTagContent(manager->isShowLeftSlider() ? "1" : "0");
    endTag();

    startTag("axisrange");
    writeAttribute("axis", "x");
    char range[10];
    sprintf(range, "%d", minX);
    writeTag("min", range);
    sprintf(range, "%d", maxX);
    writeTag("max", range);
    endTag();

    startTag("axisrange");
    writeAttribute("axis", "y");
    sprintf(range, "%d", minY);
    writeTag("min", range);
    sprintf(range, "%d", maxY);
    writeTag("max", range);
    endTag();

    char size[2];
    sprintf(size, "%d", manager->getZoomerDecimalSize());
    writeTag("decimalNumbers", size);
    // end plot
    endTag();

    std::cout << "Writing " << markers.size() << " Borderlines" << std::endl;

    for(unsigned int i=0;i<markers.size();i++)
    {
        QwtPlotMarker* marker = markers[i];
        if(marker == NULL)
        {
            continue;
        }
        std::cout << "Have not NULL" << std::endl;
        // begin the borderlines
        startTag("borderline");
        writeAttribute("visible", "1");

        char temp[10];
        sprintf(temp, "%f", marker->xValue());
        writeTag("xposition", temp);
        sprintf(temp, "%f", marker->yValue());
        writeTag("yposition", temp);
        switch(marker->lineStyle())
        {
            case QwtPlotMarker::HLine:
                writeTag("type", "horizontal");
                break;
            case QwtPlotMarker::VLine:
                writeTag("type", "vertical");
                break;
            case QwtPlotMarker::Cross:
                writeTag("type", "cross");
                break;
            case QwtPlotMarker::NoLine:
                writeTag("type", "none");
                break;
        }
        sprintf(temp, "%d", marker->linePen().width());
        writeTag("width", "3");

        QColor lineColor = marker->linePen().color();
        startTag("lineColor");
        sprintf(temp, "%d", lineColor.red());
        writeTag("red", temp);
        sprintf(temp, "%d", lineColor.green());
        writeTag("green", temp);
        sprintf(temp, "%d", lineColor.blue());
        writeTag("blue", temp);
        endTag();
        // end borderline
        endTag();
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

