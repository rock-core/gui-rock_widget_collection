/* 
 * File:   CSVImporter.h
 * Author: blueck
 *
 * Created on 11. August 2010, 14:38
 */

#ifndef CSVIMPORTER_H
#define	CSVIMPORTER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>

class CSVImporter
{
public:
    CSVImporter();
    virtual ~CSVImporter();
    static std::vector< std::vector<double> > getDoubleArrayFromFile(std::string filename);
private:

};

#endif	/* CSVIMPORTER_H */

