/* 
 * File:   XMLConfigWriter.h
 * Author: blueck
 *
 * Created on 24. November 2010, 13:37
 */

#ifndef XMLCONFIGWRITER_H
#define	XMLCONFIGWRITER_H

#include <iostream>
#include <encoding.h>
#include <xmlwriter.h>


class XMLConfigWriter
{
public:
    XMLConfigWriter();
    virtual ~XMLConfigWriter();
    bool writeConfigFile(char* filename);
    virtual bool writeContent() = 0;
    bool startNode(const char* name);
    bool writeNode(const char* name, const char* content = 0);
    bool writeNodeContent(const char* content);
    bool writeAttribute(const char* name, const char* value);
    bool endNode();
protected:
    xmlTextWriterPtr writer;
private:
    static char* encoding;

};

#endif	/* XMLCONFIGWRITER_H */

