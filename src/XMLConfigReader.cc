/* 
 * File:   XMLConfigReader.cpp
 * Author: blueck
 * 
 * Created on 23. November 2010, 10:49
 */

#include "XMLConfigReader.h"

XMLConfigReader::XMLConfigReader()
{ }


XMLConfigReader::~XMLConfigReader()
{ }


bool XMLConfigReader::validateXMLFile(const char* docFilename, const char* schemaFilename)
{
    char* currentLocale = setlocale(LC_ALL, NULL);
    setlocale(LC_ALL, "C");
    xmlDocPtr document;
    document = xmlParseFile(docFilename);
    if(document == NULL)
    {
        std::cerr << "Could not parse XML document:[" << docFilename << "]" << std::endl;
        return false;
    }
    xmlDocPtr schemaDoc = xmlReadFile(schemaFilename, NULL, XML_PARSE_NONET);
    if (schemaDoc == NULL)
    {
        std::cerr << "The schema is not well formed or could not be read." << std::endl;
        return false;
    }
    xmlSchemaParserCtxtPtr parserCtxt = xmlSchemaNewDocParserCtxt(schemaDoc);
    if (parserCtxt == NULL)
    {
        std::cerr << "Unable to craete a context for the parse" << std::endl;
        xmlFreeDoc(schemaDoc);
        return false;
    }
    xmlSchemaPtr schema = xmlSchemaParse(parserCtxt);
    if (schema == NULL)
    {
        std::cerr << "The schema is not valid" << std::endl;
        xmlSchemaFreeParserCtxt(parserCtxt);
        xmlFreeDoc(schemaDoc);
        return false;
    }
    xmlSchemaValidCtxtPtr validCtxt = xmlSchemaNewValidCtxt(schema);
    if (validCtxt == NULL)
    {
        std::cerr << "Could not create a validation context" << std::endl;
        xmlSchemaFree(schema);
        xmlSchemaFreeParserCtxt(parserCtxt);
        xmlFreeDoc(schemaDoc);
        return false;
    }
    int isValid = (xmlSchemaValidateDoc(validCtxt, document) == 0);
    xmlSchemaFreeValidCtxt(validCtxt);
    xmlSchemaFree(schema);
    xmlSchemaFreeParserCtxt(parserCtxt);
    xmlFreeDoc(schemaDoc);
    setlocale(LC_ALL, currentLocale);
    /* force the return value to be non-negative on success */
    return isValid;
}

void XMLConfigReader::readXMLFile(const char* filename)
{
    xmlNodePtr rootNode;
    document = xmlParseFile(filename);
    if(document == NULL)
    {
        std::cout << "Could not parse document" << std::endl;
        return;
    }
    rootNode = xmlDocGetRootElement(document);
    if (rootNode == NULL)
    {
        std::cout << "The document is empty and contains no root element" << std::endl;
	xmlFreeDoc(document);
	return;
    }
    if (xmlStrcmp(rootNode->name, (const xmlChar *) "configuration"))
    {
        std::cout << "Wrong document type, does not contain <configuration> as root element" << std::endl;
	xmlFreeDoc(document);
	return;
    }
    
    
    if(!readXMLSpecifica())
    {
        std::cerr << "Error while reading xml documents content" << std::endl;
    }

    
    xmlFreeDoc(document);
}

