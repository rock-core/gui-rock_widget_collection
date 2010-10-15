/* 
 * File:   CSVImporter.cc
 * Author: blueck
 * 
 * Created on 11. August 2010, 14:38
 */

#include "CSVImporter.h"

CSVImporter::CSVImporter()
{
}


CSVImporter::~CSVImporter()
{
}

std::vector< std::vector<double> > CSVImporter::getDoubleArrayFromFile(std::string filename, char delimiter)
{
    std::ifstream inFile (filename.c_str());
    std::string line;
    int linenum = 0;
    std::vector< std::vector<double> > result(500);
    while (getline (inFile, line))
    {
        linenum++;
        std::istringstream linestream(line);
        std::string item;
        int itemnum = 0;
        std::vector<double> lineResult(10);
        while (getline (linestream, item, delimiter))
        {
            lineResult[itemnum] = atof(item.c_str());
            itemnum++;
        }
        result[linenum-1] = lineResult;
    }
    return result;
}

