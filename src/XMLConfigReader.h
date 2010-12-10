/* 
 * File:   XMLConfigReader.h
 * Author: blueck
 *
 * Created on 23. November 2010, 10:49
 */

#ifndef XMLCONFIGREADER_H
#define	XMLCONFIGREADER_H

#include <iostream>
#include <parser.h>
#include <xmlschemas.h>
#include <xmlmemory.h>

#include <stdio.h>
#include <iostream>
#include <locale>

class XMLConfigReader
{
public:
    XMLConfigReader();
    virtual ~XMLConfigReader();
    void readXMLFile(const char* filname);
    virtual bool readXMLSpecifica() = 0;
    bool validateXMLFile(const char* docFilename, const char* shemaFilename);
protected:
    xmlDocPtr document;
};

#endif	/* XMLCONFIGREADER_H */

