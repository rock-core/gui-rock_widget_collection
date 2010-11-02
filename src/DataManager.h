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

/**
 * Singleton class which contains various settings needed throughout the widget.
 *
 * <h1>History</h1>
 * 2010-10-22 Initial revision
 *
 * <h1>Known Issues</h1>
 *
 * <h1>To-Do</h1>
 * Add the ability to store all or most of those settings to a file (QSettings or
 * key-value pair or ...)
 *
 * @author Bjoern Lueck <blueck@dfki.de>
 * @version 0.1
 */
class DataManager
{

public:

     /** deconstructor*/
    ~DataManager();
    /**
     * static method which returns the singleton instance
     * @return the singleton instance
     */
    static DataManager* getInstance();

    /**
     * Returns the csv delimiter which shall be used wehn im- or exporting
     * csv data.
     * @return the csv delimiter
     */
    char getCSVDelimter() { return csvDelimiter; };

    /**
     * Sets the delimiter used for im- or exporting csv data
     * @param delimiter the new delimiter to use
     */
    void setCSVDelimiter(char delimiter) { csvDelimiter = delimiter; };

    /**
     * Retunrs whether the left slider shall be shown
     * @return if the left slider shall be shown
     */
    bool isShowLeftSlider() { return showLeftSlider;};

    /**
     * Sets whether the left slider shall be shown
     * @param showLeftSlider if the left slider shall be shown
     */
    void setShowLeftSlider(bool showLeftSlider) { this->showLeftSlider = showLeftSlider;};

    /**
     * Returns whether the bottom slider is shown
     * @return whether the bottom slide shall be shown
     */
    bool isShowBottomSlider() { return showBottomSlider;};

    /**
     * Sets whether the bottom slider shall be shown
     * @param showBottomSlider if the bottom slider shall be shown
     */
    void setShowBottomSlider(bool showBottomSlider) { this->showBottomSlider = showBottomSlider; };

    /**
     * Returns whether the x grid shall be drawn
     * @return whether the x grid shall be drawn
     */
    bool isDrawXGrid() { return drawXGrid; };

    /**
     * Sets whether the x grid shall be drawn
     * @param drawXGrid whether the x grid shall be drawn
    */
    void setDrawXGrid(bool drawXGrid) { this->drawXGrid = drawXGrid; };

    /**
     * Returns whether the y grid is drawn
     * @return whether teh y grid is drawn
     */
    bool isDrawYGrid() { return drawYGrid; };

    /**
     * Sets whether the y grid is drawn
     * @param drawYGrid whether the y grid is drawn
     */
    void setDrawYGrid(bool drawYGrid) { this->drawYGrid = drawYGrid; };

    /**
     * Returns the color of the plots background
     * @return the color of the plots background
     */
    QColor getBGColor() { return bgColor; };

    /**
     * Sets the color of the plots background
     * @param color the color of the plots background
     */
    void setBGColor(QColor color) { bgColor = color; };

    /**
     * Returns the x-axis title
     * @return  the x-axis title
     */
    QString getXAxisTitle() { return xAxisTitle; };

    /**
     * Sets the x axis title
     * @param newTitle the new x axis title
     */
    void setXAxisTitle(QString newTitle) { xAxisTitle = newTitle; };

    /**
     * Returns the y axis title
     * @return  the y axis title
     */
    QString getYAxisTitle() { return yAxisTitle; };

    /**
     * Sets the y axis title
     * @param newTitle the new y axis title
     */
    void setYAxisTitle(QString newTitle) { yAxisTitle = newTitle; };


protected:
    /** The singleton instance */
    static DataManager* INSTANCE;
    /** the csv delimiter*/
    char csvDelimiter;
    /** whether the left slider shall be shown*/
    bool showLeftSlider;
    /** whether the right slider shall be shown*/
    bool showBottomSlider;
    /*** whether the x grid will be drawn*/
    bool drawXGrid;
    /** whether the y grid will be drawn*/
    bool drawYGrid;
    /** the background color of the plot*/
    QColor bgColor;
    /** the title of the x axis*/
    QString xAxisTitle;
    /** the title of the y axis*/
    QString yAxisTitle;

private:
    /** private constructor*/
    DataManager();

};

#endif	/* DATAMANAGER_H */

