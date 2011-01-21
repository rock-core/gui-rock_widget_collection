/* 
 * File:   DataManager.h
 * Author: blueck
 *
 * Created on 15. Oktober 2010, 11:36
 */

#ifndef DATAMANAGER_H
#define	DATAMANAGER_H

#include <stddef.h>
#include <qcolor.h>

/**
 * Singleton class which contains various settings needed throughout the widget.
 *
 * <h1>History</h1>
 * 2010-10-22 Initial revision
 *
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

    /**
     * Sets if a legend shall be drawn
     * @param drawLegend if a legend shall be drawn
     */
    void setDrawLegend(bool drawLegend) { this->drawLegend = drawLegend; };

    /**
     * Return swhether a legend shall be drawn
     * @return whether a legend shall be drawn
     */
    bool isDrawLegend() { return drawLegend; };

    /**
     * Sets the legendposition
     * @param legendPosition the legend position
     */
    void setLegendPosition(int legendPosition) { this->legendPosition = legendPosition; };

    /**
     * Returns the legend position
     * @return the legend poistion
     */
    int getLegendPosition() { return legendPosition; };

    /**
     * Sets whether autoscrolling is enabled
     * @param autoscrolling whether autoscrolling is enabled
     */
    void setAutoscrolling(bool autoscrolling) { this->autoscrolling = autoscrolling; };

    /**
     * Return swhether autoscrolling is enabled
     * @return whether autoscrolling is enabled
     */
    bool isAutoscrolling() { return autoscrolling; };

    /**
     * Return sthe decimal size of the visible zoomer
     * @return the number of numbers after the decimal
     */
    int getZoomerDecimalSize() { return zoomerDecimalSize; };

    /**
     * Sets the decimal size of the zoomer
     * @param zoomerDecimalSize the number of characters after the decimal dot
     */
    void setZoomerDecimalSize(int zoomerDecimalSize) { this->zoomerDecimalSize = zoomerDecimalSize; };

    /**
     * Sets the autoscrolling to on or off
     * @param autoscaling true if on, false if off
     */
    void setAutoscaling(bool autoscaling) { this->autoscaling = autoscaling;};

    /**
     * Returns whether autoscaling is turned on
     * @return true id turned on, false otherwise
     */
    bool isAutoscaling() { return autoscaling; };

    /**
     * Returns if a fixed size is used
     * @param fixedSize true if a fixed site shall be used
     */
    void setIsFixedSize(bool fixedSize) { this->fixedSize = fixedSize; };

    /**
     * Returns if a fixed size shall be used
     * @return true if fixed
     */
    bool isFixedSize() { return fixedSize; };

    /**
     * sets the lower x Boundary if fixed or autoscrolling
     * @param minX the lower x boundary
     */
    void setMinX(double minX) { this->minX = minX; };

    /**
     * Returns the lower x boundary, ignored if autoscale is enabled
     * @return the lower x boundary
     */
    double getMinX() { return minX; };

    /**
     * Sets the upper x Boundary if fixed or autoscrolling
     * @param maxX the upper x boundary
     */
    void setMaxX(double maxX) { this->maxX = maxX; };

    /**
     * Returns the upper x boundary, ignored if autoscale equals true
     * @return the upper x boundary
     */
    double getMaxX() { return maxX; };

    /**
     * sets the lower y Boundary if fixed or autoscrolling
     * @param minY the lower y boundary
     */
    void setMinY(double minY) { this->minY = minY; };

    /**
     * Returns the lower y boundary, ignored if autoscale is enabled
     * @return the lower y boundary
     */
    double getMinY() { return minY; };

    /**
     * Sets the upper y Boundary if fixed or autoscrolling
     * @param maxY the upper y boundary
     */
    void setMaxY(double maxY) { this->maxY = maxY; };

    /**
     * Returns the upper y boundary, ignored if autoscale equals true
     * @return the upper y boundary
     */
    double getMaxY() { return maxY; };


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
    /** Whether to draw a legend or not*/
    bool drawLegend;
    /** The position of the legend if any*/
    int legendPosition;
    /** if autoscrolling is turned on*/
    bool autoscrolling;
    /** number of numbers after the decimal point of the zoomer*/
    int zoomerDecimalSize;
    /** Autoscaling*/
    bool autoscaling;
    /** Fixed Size*/
    bool fixedSize;
    /** Min x Boundary*/
    double minX;
    /** Max X Boundary*/
    double maxX;
    /** Min Y Boundary*/
    double minY;
    /** Max Y Boundary*/
    double maxY;

private:
    /** private constructor*/
    DataManager();

};

#endif	/* DATAMANAGER_H */

