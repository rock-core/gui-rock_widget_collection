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
#include <locale>

/**
 * Class which contains static methods to import data from a
 * csv file.
 *
 * <h2>History</h2>
 * 2010-10-22 Initial revision
 *
 * <h2>Known Issues</h2>
 * Usually decimal values will be exported with the '.' character, so using
 * the '.' character as a delimiter will result in undesired behaviour or
 * not working at all.
 *
 *
 * @author Bjoern Lueck <blueck@dfki.de>
 * @version 0.1
 */
class CSVImporter
{
public:
    /**
     * Takes the input csv file and returns a vector of double vectors which contain the
     * data within the csv file. Each containing vector contains one column of
     * the source csv file. Note that floating point data will use the
     * '.' character.
     * @param filename location of the file to be read
     * @param delimiter the delimiter in the csv file, ',' being the default
     * @return a vector containing each column as double vector
     */
    static std::vector< std::vector<double> > getDoubleArrayFromFile(std::string filename, char delimiter=',');
private:

};

#endif	/* CSVIMPORTER_H */

