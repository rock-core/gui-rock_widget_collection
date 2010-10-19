/* 
 * File:   DataManager.h
 * Author: blueck
 *
 * Created on 15. Oktober 2010, 11:36
 */

#ifndef DATAMANAGER_H
#define	DATAMANAGER_H

#include <stddef.h>
#include <qt4/Qt/qcolor.h>

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
    QColor getBGColor() { return bgColor; };
    void setBGColor(QColor color) { bgColor = color; };
    QString getXAxisTitle() { return xAxisTitle; };
    void setXAxisTitle(QString newTitle) { xAxisTitle = newTitle; };
    QString getYAxisTitle() { return yAxisTitle; };
    void setYAxisTitle(QString newTitle) { yAxisTitle = newTitle; };


protected:
    static DataManager* INSTANCE;
    char csvDelimiter;
    bool showLeftSlider;
    bool showBottomSlider;
    bool drawXGrid;
    bool drawYGrid;
    QColor bgColor;
    QString xAxisTitle;
    QString yAxisTitle;

private:

    DataManager();
    DataManager(const DataManager& orig);
    virtual ~DataManager();

};

#endif	/* DATAMANAGER_H */

