/* 
 * File:   XMLConfigWriter.cc
 * Author: blueck
 * 
 * Created on 24. November 2010, 13:37
 */

#include "XMLConfigWriter.h"

static char* encoding = "ISO-8859-1";

XMLConfigWriter::XMLConfigWriter()
{
}

bool XMLConfigWriter::writeConfigFile(char* filename)
{
    // compression deactivated
    // TODO make compression configurable
    writer = xmlNewTextWriterFilename(filename, 0);
    xmlTextWriterSetIndent(writer, 1);
    if (writer == NULL)
    {
        std::cerr << "Could not create an XML Writer to:[" << filename << "]" << std::endl;
        return false;
    }
    // TODO: make encoding configurable
    int retValue = xmlTextWriterStartDocument(writer, NULL, "ISO-8859-1", NULL);
    if(retValue < 0)
    {
        std::cerr << "Could not start writing XML document" << std::endl;
        return false;
    }
    retValue = xmlTextWriterStartElement(writer, BAD_CAST "configuration");
    if (retValue < 0)
    {
        std::cerr << "Could not write xml element <configuration>" << std::endl;
        return false;
    }
    // content goes here, call the abstract method
    writeContent();
    // close everything
    retValue = xmlTextWriterEndDocument(writer);
    if (retValue < 0)
    {
        std::cerr << "Could not end the xml document" << std::endl;
        return false;
    }
    xmlFreeTextWriter(writer);
    return true;
}

bool XMLConfigWriter::startNode(const char* name)
{
    int retValue = xmlTextWriterStartElement(writer, BAD_CAST name);
    if (retValue < 0)
    {
        std::cerr << "Could not write xml element <configuration>" << std::endl;
        return false;
    }
    return true;
}

bool XMLConfigWriter::writeNode(const char* name, const char* content)
{
    int retValue = xmlTextWriterWriteElement(writer, BAD_CAST name, ((content == 0) ? (const xmlChar*)"" : (const xmlChar*)(content)));
    if (retValue < 0)
    {
        std::cerr << "Could not write xml element " << name << std::endl;
        return false;
    }
    return true;
}

bool XMLConfigWriter::writeAttribute(const char* name, const char* value)
{
    return xmlTextWriterWriteAttribute(writer, BAD_CAST name, BAD_CAST value);
}

bool XMLConfigWriter::endNode()
{
    return xmlTextWriterEndElement(writer);
}

bool XMLConfigWriter::writeNodeContent(const char* content)
{
    xmlTextWriterWriteString(writer, BAD_CAST content);
}


XMLConfigWriter::~XMLConfigWriter()
{
}

