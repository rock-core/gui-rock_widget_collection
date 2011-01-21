/* 
 * File:   DataManager.cc
 * Author: blueck
 * 
 * Created on 15. Oktober 2010, 11:36
 */

#include "DataManager.h"

DataManager* DataManager::INSTANCE;

DataManager* DataManager::getInstance()
{
    if(INSTANCE == NULL)
    {
        INSTANCE = new DataManager();
    }
    return INSTANCE;
}

DataManager::DataManager()
{
    // TODO: load from a configuration file if
    // there is one otherwise set defaults 
    // set defaults
    csvDelimiter = ',';
    showBottomSlider = false;
    showLeftSlider = false;
    drawXGrid = true;
    drawYGrid = true;
    drawLegend = false;
    fixedSize = false;
    autoscrolling = false;
    autoscaling = true;
    legendPosition = 0;
    zoomerDecimalSize = 4;
}

DataManager::~DataManager()
{
    delete INSTANCE;
    INSTANCE = NULL;
}

