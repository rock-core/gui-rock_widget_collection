/* 
 * File:   DataManager.h
 * Author: blueck
 *
 * Created on 15. Oktober 2010, 11:36
 */

#ifndef DATAMANAGER_H
#define	DATAMANAGER_H

#include <stddef.h>

class DataManager
{

public:
    static DataManager* getInstance();
    char getCSVDelimter() { return csvDelimiter; };
    void setCSVDelimiter(char delimiter) { csvDelimiter = delimiter; };
    bool isShowLeftSlider() { return showLeftSlider;};
    void setShowLeftSlider(bool showLeftSlider) { this->showLeftSlider = showLeftSlider;};
    bool isShowBottomSlider() { return showBottomSlider;};
    void setShowBottomSlider(bool showBottomSlider) { this->showBottomSlider = showBottomSlider; };
    bool isDrawXGrid() { return drawXGrid; };
    void setDrawXGrid(bool drawXGrid) { this->drawXGrid = drawXGrid; };
    bool isDrawYGrid() { return drawYGrid; };
    void setDrawYGrid(bool drawYGrid) { this->drawYGrid = drawYGrid; };

protected:
    static DataManager* INSTANCE;
    char csvDelimiter;
    bool showLeftSlider;
    bool showBottomSlider;
    bool drawXGrid;
    bool drawYGrid;

private:

    DataManager();
    DataManager(const DataManager& orig);
    virtual ~DataManager();

};

#endif	/* DATAMANAGER_H */

