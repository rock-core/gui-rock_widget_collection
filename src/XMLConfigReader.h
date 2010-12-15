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

/**
 * Abstract xml reader class.<br>
 * The reader will parse a file until it finds a configuration tag. Afterwards the
 * abstract readXMLSpecifa method will be called. This needs o be overwritten
 * in a subclass and all reading related to the nodes after configuration
 * needs to be done there.<br>
 * Additionally there is the validateXMLFile method which will
 * validate a given xml file to a given schema.
 * <p>
 * <h1>History</h1>
 * 15-12-2010 Created documentation
 *
 * <h1>Todo</h1>
 *
 * <ul>
 * <li>Make it work without configuration as top level entry</li>
 * <li>Handle floating points correctly according to encodings</li>
 * <li>Create convenience methods for reading</li>
 * </ul>
 */
class XMLConfigReader
{
public:
    /**
     * Empty standard constructor
     */
    XMLConfigReader();

    /**
     * Emtpy deconstructor
     */
    virtual ~XMLConfigReader();

    /**
     * Tries to read the given xml file. Calls readXMLSpecifica for specific
     * tag reading
     * @param filname the filename of the XML document
     */
    bool readXMLFile(const char* filname);

    /**
     * Abstract method which reads all xml content
     * @return true if successfull, false otherwise
     */
    virtual bool readXMLSpecifica() = 0;

    /**
     * Validates the given xml file to the given schema
     * @param docFilename the xml file to validate
     * @param shemaFilename teh schema file to validate against
     * @return true if validation was succssefull, false otehrwise
     */
    bool validateXMLFile(const char* docFilename, const char* shemaFilename);
protected:
    /** Document for reading purposes*/
    xmlDocPtr document;
};

#endif	/* XMLCONFIGREADER_H */

