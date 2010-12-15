/* 
 * File:   PlotXMLReader.cc
 * Author: blueck
 * 
 * Created on 26. November 2010, 13:50
 */

#include <qwt-qt4/qwt_plot_marker.h>
#include <vector>
#include <QtGui/qpen.h>

#include "PlotXMLReader.h"

PlotXMLReader::PlotXMLReader() : XMLConfigReader()
{
    manager = DataManager::getInstance();
}

bool PlotXMLReader::readXMLSpecifica()
{
    // currently quite messy clean up
    borderLines.clear();
    xmlNodePtr currentNode;
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;
    context = xmlXPathNewContext(document);
    result = xmlXPathEvalExpression((const xmlChar*)"//plot", context);
    if(xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        xmlXPathFreeObject(result);
        std::cerr << "Could not get plot node via xpath, missing plot node maybe?" << std::endl;
        return false;
    }
    else
    {
        xmlNodeSetPtr nodeset;
        nodeset = result->nodesetval;
        // there must be exactly one plot node
        if(nodeset->nodeNr == 1)
        {
            xmlNodePtr currentNode = nodeset->nodeTab[0];
            xmlNodePtr childNode = currentNode->xmlChildrenNode;
            // next the children and if there is an element we know about handle it
            xmlNodePtr nextNode = childNode;
            while(nextNode != NULL && nextNode != childNode->last)
            {
                const xmlChar* name = nextNode->name;
                if(!xmlStrcmp(name, (const xmlChar*)"title"))
                {
                    const xmlChar* property = xmlGetProp(nextNode, (const xmlChar*)"axis");
                    const xmlChar* contents;
                    if(!xmlStrcmp(property, (const xmlChar*)"x"))
                    {
                        contents = xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                        manager->setXAxisTitle((const char*)contents);
                    }
                    else if(!xmlStrcmp(property, (const xmlChar*)"y"))
                    {
                        contents = xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                        manager->setYAxisTitle((const char*)contents);
                    }
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"csvDelimiter"))
                {
                    const char* contents = (const char*)xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                    manager->setCSVDelimiter(contents[0]);
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"autoscrolling"))
                {
                    const char* contents = (const char*)xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                    manager->setAutoscrolling((bool)contents);
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"showGrid"))
                {
                    const xmlChar* property = xmlGetProp(nextNode, (const xmlChar*)"axis");
                    const char* contents;
                    if(!xmlStrcmp(property, (const xmlChar*)"x"))
                    {
                        contents = (const char*)xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                        manager->setDrawXGrid((bool)atoi(contents));
                    }
                    else if(!xmlStrcmp(property, (const xmlChar*)"y"))
                    {
                        contents = (const char*)xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                        manager->setDrawYGrid((bool)atoi(contents));
                    }
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"showSlider"))
                {
                    const xmlChar* property = xmlGetProp(nextNode, (const xmlChar*)"axis");
                    const char* contents;
                    if(!xmlStrcmp(property, (const xmlChar*)"x"))
                    {
                        contents = (const char*)xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                        manager->setShowBottomSlider((bool)atoi(contents));
                    }
                    else if(!xmlStrcmp(property, (const xmlChar*)"y"))
                    {
                        contents = (const char*)xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                        manager->setShowLeftSlider((bool)atoi(contents));
                    }
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"legend"))
                {
                    const xmlChar* property = xmlGetProp(nextNode, (const xmlChar*)"visible");
                    const xmlChar* contents;
                    xmlNodePtr subNode = nextNode->xmlChildrenNode;
                    contents = xmlNodeListGetString(document, subNode->xmlChildrenNode, 1);
                    manager->setDrawLegend((bool)atoi((const char*)property));
                    if(!xmlStrcmp(contents, (const xmlChar*)"top"))
                    {
                        manager->setLegendPosition(3);
                    }
                    else if(!xmlStrcmp(contents, (const xmlChar*)"left"))
                    {
                        manager->setLegendPosition(0);
                    }
                    else if(!xmlStrcmp(contents, (const xmlChar*)"bottom"))
                    {
                        manager->setLegendPosition(2);
                    }
                    else if(!xmlStrcmp(contents, (const xmlChar*)"right"))
                    {
                        manager->setLegendPosition(1);
                    }
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"axisrange"))
                {
                    const xmlChar* property = xmlGetProp(nextNode, (const xmlChar*)"axis");
                    const xmlChar* contents;
                    const xmlChar* sContents;
                    xmlNodePtr subNode = nextNode->xmlChildrenNode->next;
                    contents = xmlNodeListGetString(document, subNode->xmlChildrenNode, 1);

                    subNode = subNode->next->next;
                    sContents = xmlNodeListGetString(document, subNode->xmlChildrenNode, 1);
                    if(!xmlStrcmp(property, (const xmlChar*)"x"))
                    {
                        minX = atoi((const char*)contents);
                        maxX = atoi((const char*)sContents);
                    }
                    else
                    {
                        minY = atoi((const char*)contents);
                        maxY = atoi((const char*)sContents);
                    }
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"bgcolor"))
                {
                    const xmlChar* rContents;
                    const xmlChar* gContents;
                    const xmlChar* bContents;
                    xmlNodePtr subNode = nextNode->xmlChildrenNode->next;
                    rContents = xmlNodeListGetString(document, subNode->xmlChildrenNode, 1);
                    subNode = subNode->next->next;
                    gContents = xmlNodeListGetString(document, subNode->xmlChildrenNode, 1);
                    subNode = subNode->next->next;
                    bContents = xmlNodeListGetString(document, subNode->xmlChildrenNode, 1);

                    QColor color(atoi((const char*)rContents), atoi((const char*)gContents), atoi((const char*)bContents));
                    manager->setBGColor(color);
                }
                nextNode = nextNode->next;
            }
        }
        else
        {
            std::cerr << "There are " << nodeset->nodeNr << " plot nodes. There must be exactly 1" << std::endl;
            return false;
        }
    }
    //now get all the borderlines
    result = xmlXPathEvalExpression((const xmlChar*)"//borderline", context);
    if(xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        xmlXPathFreeObject(result);
        std::cout << "There are no borderlines configured." << std::endl;
    }
    else
    {
        xmlNodeSetPtr nodeset;
        nodeset = result->nodesetval;
        for(int i=0;i<nodeset->nodeNr;i++)
        {
            QwtPlotMarker* marker = new QwtPlotMarker();
            QPen linePen = marker->linePen();
            xmlNodePtr currentNode = nodeset->nodeTab[i];
            std::cout << currentNode->name << std::endl;
            xmlNodePtr childNode = currentNode->xmlChildrenNode;
            // next the children and if there is an element we know about handle it
            xmlNodePtr nextNode = childNode;
            while(nextNode != NULL && nextNode != childNode->last)
            {
                const xmlChar* name = nextNode->name;
                if(!xmlStrcmp(name, (const xmlChar*)"xposition"))
                {
                    const char* contents = (const char*)xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                    marker->setXValue(atof((const char*)contents));
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"yposition"))
                {
                    const char* contents = (const char*)xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                    marker->setYValue(atof((const char*)contents));
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"type"))
                {
                    const xmlChar* contents = xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                    if(!xmlStrcmp(contents, (const xmlChar*)"horizontal"))
                    {
                        marker->setLineStyle(QwtPlotMarker::HLine);
                    }
                    else if(!xmlStrcmp(contents, (const xmlChar*)"vertical"))
                    {
                        marker->setLineStyle(QwtPlotMarker::VLine);
                    }
                    else if(!xmlStrcmp(contents, (const xmlChar*)"cross"))
                    {
                        marker->setLineStyle(QwtPlotMarker::Cross);
                    }
                    else if(!xmlStrcmp(contents, (const xmlChar*)"none"))
                    {
                        marker->setLineStyle(QwtPlotMarker::NoLine);
                    }
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"width"))
                {
                    const char* contents = (const char*)xmlNodeListGetString(document, nextNode->xmlChildrenNode, 1);
                    linePen.setWidth(atoi((const char*)contents));
                }
                else if(!xmlStrcmp(name, (const xmlChar*)"lineColor"))
                {
                    const xmlChar* rContents;
                    const xmlChar* gContents;
                    const xmlChar* bContents;
                    xmlNodePtr subNode = nextNode->xmlChildrenNode->next;
                    rContents = xmlNodeListGetString(document, subNode->xmlChildrenNode, 1);
                    subNode = subNode->next->next;
                    gContents = xmlNodeListGetString(document, subNode->xmlChildrenNode, 1);
                    subNode = subNode->next->next;
                    bContents = xmlNodeListGetString(document, subNode->xmlChildrenNode, 1);
                    QColor color(atoi((const char*)rContents), atoi((const char*)gContents), atoi((const char*)bContents));
                    linePen.setColor(color);
                }
                nextNode = nextNode->next;
            }
            marker->setLinePen(linePen);
            borderLines.push_back(marker);
        }
    }
    return true;
}

PlotXMLReader::~PlotXMLReader() { }

