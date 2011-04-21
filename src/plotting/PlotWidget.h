/* 
 * File:   PlotWidget.h
 * Author: blueck
 *
 * Created on 17. August 2010, 12:24
 */

#ifndef PLOTWIDGET_H
#define	PLOTWIDGET_H

#include "PlottingWidget.h"

#include <QResizeEvent>
#include <QPushButton>
#include <qtimer.h>
#include <qlist.h>
#include <QGridLayout>
#include <QMenuBar>
#include <QStyleOptionMenuItem>
#include <QIcon>
#include <QWaitCondition>
#include <QMutex>


#include <qwt_slider.h>
#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <map>


//Forward Declaration
class ExtendedPlotZoomer;
class CurveSelectionDialog;
class OptionsDialog;
class DataManager;

// the following defines are neded
// as ruby can't work properly with
// enums at the moment

// AXIS DEFINES

#define X_BOTTOM 0
#define Y_LEFT 1
#define X_TOP 2
#define Y_RIGHT 3

// STYLE DEFINES

#define NO_CURVE 0
#define LINES 1
#define STICKS 2
#define STEPS 3
#define DOTS 4
#define USER_CURVE 100


/**
 * Widget which displays a plot. When simply constructing the plot
 * will have one x and one y axis (the left y axis and the bottom x axis)<br>
 * <h1>Zooming</h1>
 * Via selecting a rectangle with the left mouse button, one can zoom to the
 * rectangle selected. The axis descriptions will change accordingly.<br>
 * Pressing the right mouse button will unzoom to the axis given at start, or
 * the dimensions automatically calculated if none were specified. Currently
 * only zooming the xBottom and yLeft axis is supported. Other axis will not be
 * zoomed or changed in any way. Using the mouse wheel zooming further in or away
 * from the current point can be done. The axis will change accordingly.<br>
 * When zooming via the rectangle multple times, the middle mouse button will
 * go one zoom step back. So you can zoom in multiple times and "undo" the
 * selections one at a time.
 * <h1>Border Lines</h1>
 * One can add borderlines to the plot. Those can be vertical or horizontal
 * and have various style components, managed via QPen Objects. The underlying
 * QwtMarker object can be retrieved to use all of the QwtMArker functionality
 * even though it may not be wrapped here.
 * <h1>Axis</h1>
 * There are possibly 4 Axis in the plot. Generally two are shown by default, which
 * are the left y axis and the bottom x axis. Axis can have titles which can
 * be set manually. The boundaries and step sizes of axis can be set by the
 * setAxisBoundaries methods. Note that setting very small step sizes in
 * relation to the lower and upper boundaries will result in a very ugly visual.<br>
 * If you don't specify any boundaries the plot will try to specify them by itself
 * according to the data given to the plot. In most cases this is the preferred solution.
 * <h1>Data</h1>
 * Data can be added via the addData method. This will create a curve on the plot
 * on the specified axis. The method returns an id with which the curve can afterwards be altered, be
 * it style, via setDataStyle or enabling it via setDataEnabled. One can also retrieve the QwtPlotCurve
 * object for a data id, if for any reason more functionality for the plot curves is required,<br>
 * With addData data can also be added. simply call addData with an existing dataId and the data will
 * be added to the already existing curve.
 * which is not wrapped.
 * <h1>Autoscrolling</h1>
 * One can set autoscrolling to true. If data goes beyond the current x or y axis
 * scope, the axis will scroll to the maximum value automatically. While autoscrolling
 * is enabled, neither zooming nor using the sliders is available. Set autoscrolling to false
 * when your data is complete and you can freely scroll and zoom the plot as usual.
 * <h1>Grid</h1>
 * One can add a grid via the setDrawGrid method, which will take a boolean to enable
 * and two optional parameters for the x and y axis. So its possible to
 * have an x or y grid only. The default s to have one with both.
 * <h1>Exporting the Plot</h1>
 * The plot can easily be exported via exportAsImage. This will open a
 * save dialog with the option to select a fileformat and location. For more information
 * #see QtExporter
 * @todo Zooming of all Axis, xRight and yTop are not scrollable at the moment
 *
 * @author Bjoern Lueck <blueck@dfki.de>
 * @version 0.1
 */
class PlotWidget : public QWidget
{

    Q_OBJECT
    Q_PROPERTY(bool autoscrolling READ isAutoscrolling WRITE setAutoscrolling)
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(bool drawXGrid READ isDrawXGrid WRITE setDrawXGrid)
    Q_PROPERTY(bool drawYGrid READ isDrawYGrid WRITE setDrawYGrid)
    Q_PROPERTY(QString xAxisTitle READ getXAxisTitle WRITE setXAxisTitle)
    Q_PROPERTY(QString yAxisTitle READ getYAxisTitle WRITE setYAxisTitle)
    Q_PROPERTY(bool showXSlider READ isEnableXSlider WRITE setEnableXSlider)
    Q_PROPERTY(bool showYSlider READ isEnableYSlider WRITE setEnableYSlider)
    Q_PROPERTY(int zoomerDecimalSize READ getNumberOfDecimals WRITE setNumberOfDecimals)
    Q_PROPERTY(bool showLegend READ isEnableLegend WRITE setEnableLegend)
    



public:
    /**
     * Standard constructor
     * @param parent the parent widget, defaults to 0
     */
    PlotWidget(QWidget* parent=0);

    /**
     * Standard destructor
     */
    virtual ~PlotWidget();

    // --> Exporting functions

public slots:

    void showCurve(QwtPlotItem* item, bool checekd);

    /**
     * Returns an instance of teh PlotWidget
     * @return  a new instance
     */
    QObject* newInstance()
    {
       return new PlotWidget();
    };
    
    // --> Widget related methods

    /**
     * Adds all menu entries
     */
    void addMenu();

    void testMethod();

    /**
     * Sets the number of decimals shown after a dot
     * @param decimals the number of decimals shown after the dot
     */
    void setNumberOfDecimals(int decimals);

    /**
     * Returns the number of deciamls after a dot
     * @return the number of decimals after the dot
     */
    int getNumberOfDecimals();

    // --> legend related methods

    /**
     * Sets whether the legend shall be shown
     * @param enable whether the legend shall be hidden or visible
     */
    void setEnableLegend(bool enable);

    /**
     * Returns whether the legend will be shown
     * @return true if visible, false if hidden
     */
    bool isEnableLegend();

    /**addDataWithTime
     * Sets the position of the legend
     * @param position the position of the legend
     */
    void setLegendPosition(QwtPlot::LegendPosition position);

    /**
     * Returns the position of the legend
     * @return the position of the legend
     */
    QwtPlot::LegendPosition getLegendPosition();

    /**
     * Sets the anme of the curve which is displayed in the legend
     * @param name the name for the curve in the legend
     * @param curveId the id of the curve whose name shall be changed
     */
    void setLegendNameForCurve(const QString name, int curveId);

    /**
     * Clears all points of the curve specified by the curveId. Any specified
     * style changes will still be associated with the curveId but all points
     * will be erased from the curve
     * @param curveId the id of the curve
     */
    void clearCurveData(int curveId);


    // --> Grid related methods

    /**
     * Sets if a grid shall be drawn on the plot
     * @param drawGrid if a grid shall be drawn, if false the next arguments will be ignored
     * @param enableX if the x grid will be drawn, defaults to true
     * @param enableY if the y grid will be drawn, defaults to true
     */
    void setDrawGrid(bool drawGrid, bool enableX=true, bool enableY=true);
    
    /**
     * Sets whether the x grid is drawn
     * @param drawXGrid whether the x grid is drawn
     */
    void setDrawXGrid(bool drawXGrid);

    /**
     * Sets whether the y grid is drawn
     * @param drawYGrid whether the y grid is drawn
     */
    void setDrawYGrid(bool drawYGrid);

    /**
     * Returns if the x grid is drawn
     * @return whether the x grid is drawn
     */
    bool isDrawXGrid();

    /**
     * Returns whether the y grid is drawn
     * @return whether the y grid is drawn
     */
    bool isDrawYGrid();

    // --> Slider realted methods

    /** Enables or disables a slider
     * @param axisId the slider of which axis to enable/disable
     * @param enable if the slider shall be anabled or disabled, defaults to true
     */
    void enableSlider(int axisId, bool enable=true);

    /**
     * Sets whether the x slider shall be enabled
     * @param enable whether the x slider shall be enabled
     */
    void setEnableXSlider(bool enable);
    /**
     * Returns whether the x slider is enabled
     * @return whether the x slider is enabled
     */
    bool isEnableXSlider();

    /**
     * Sets whether the y slider is enabled
     * @return whether the y slider is enabled
     */
    void setEnableYSlider(bool enable);

    /**
     * Returns wehether the y slider is enabled
     * @return whether the y slider is enabled
     */
    bool isEnableYSlider();

    // --> Border Line related methods

    /**
     * Adds a fixed line to the plot
     * @param value the value at which the line will be drawn. y Value for horizontal lines, x value for vertical ones
     * @param orientation If its a horizontal or vertical line, either Qt::Horizontal or Qt::Vertical, default is Qt::Horizontak
     * @param pen a pen used to onfigure the view of teh line, defaults to a red dashed line with width 2
     * @return the id of the border line to access it later on. The id will be unique.
     */
    int addBorderLine(double value, Qt::Orientation orientation = Qt::Horizontal, QPen pen=QPen(QColor(255, 0, 0), 2, Qt::DashLine));

    /**
     * Sets if a borderLine is enabled (shown). Defaults to true.
     * @param borderLineId the id of the borderLine which shall be en-/disabled
     * @param enable if the borderLine shall be enabled or disabled, defaults to true
     */
    void enableBorderLine(int borderLineId, bool enable=true);
    /**
     * Changes the look of an existing line. One can reset the line to teh default look by simply
     * calling the method with the id.
     * @param borderLineId the id of an existing border line
     * @param pen the pen with the new look for the line, defaults to a red dashed line with width 2
     */
    void setBorderLineStyle(int borderLineId, QPen pen=QPen(QColor(255, 0, 0), 2, Qt::DashLine));
    /**
     * Returns a QwtMarker Pointer to a marker on the plot. This method can be used if
     * qwt methods on the marker which are not wrapped are needed.
     * @param dataId the id of an existing marker
     * @return pointer to a QwtMarker object
     */
    QwtPlotMarker* getBorderLineForId(int dataId) {return markers[dataId];};

    // --> Axis related methods

    /**
     * Sets the title of the x and y Axis in a two dimensional grid.
     * Giving an empty string will result in no title.
     * @param xAxisTitle the title of the xAxis
     * @param yAxisTitle the title of the yAxis
     */
    void setAxisTitles(QString xAxisTitle, QString yAxisTitle);

    /**
     * Sets the title of the x axis
     * @param title the title
     */
    void setXAxisTitle(QString title);

    /**
     * Sets the title of the y axis
     * @param title the y axis
     */
    void setYAxisTitle(QString title);

    /**
     * Returns the title of the x axis
     * @return the title of the x axis
     */
    QString getXAxisTitle();

    /**
     * Returns the y axis title
     * @return the y axis title
     */
    QString getYAxisTitle();

    /**
     * @param axisId the id of axis to set the boundaries
     * @param lower lower value of the axis
     * @param upper upper value of the axis
     * @param step step size shown, 0 indicating automatic step size.  Defaults to 0.
     */
    void setAxisBoundaries(int axisId, double lower, double upper, double step=0);

    /** TEST*/
    void setFastAxisBoundaries(int axisId, double lower, double upper, double step=0);

    /**
     * Sets if the specified axis shall be shown or hidden
     * @param axisId the axis to be shown or hidden
     * @param enable true if the axis shall be shown, false otherwise
     */
    void setAxisShown(int axisId, bool enable=true);

    /**
     * Sets the axs to auto scaling. Whnever data is added the axis
     * may reorganize itself. Calling setAxisBoundaries will stop the
     * automatic scaling. as will setAxisAutoScale(false)
     * @param axisId the id of the axis
     * @param enable enable enable or disable auto scaling, defaults to true
     */
    void setAxisAutoScale(int axisId, bool enable=true);

    // -->Actual Data related methods

    /**
     * Adds data to the plot
     * @param xPoints an xpoint
     * @param yPoints an y point
     * @param dataId id of an existing line or -1 otherwise, defaults to -1
     * @param xAxisId the id of the xaxis, defaults to X_BOTTOM
     * @param yAxisId the id of the y axis, defaults to Y_LEFT
     * @return the id of the newly created line, or the id of the existing one
     */
    int addData(const double xPoints,const double yPoints, int dataId=-1,
		int xAxisId=X_BOTTOM, int yAxisId=Y_LEFT) throw(std::exception);

    /**
     * Adds data to the plot the x value will be automatically calculated by time.
     * The first add to the curve will be 0 and everything thereafter the delta
     * of tehe first time minus the time data was added.
     * @param xPoints an xpoint
     * @param yPoints an y point
     * @param dataId id of an existing line or -1 otherwise, defaults to -1
     * @param xAxisId the id of the xaxis, defaults to X_BOTTOM
     * @param yAxisId the id of the y axis, defaults to Y_LEFT
     * @return the id of the newly created line, or the id of the existing one
     */
    int addDataWithTime(double yPoint, int dataId=-1,
		int xAxisId=X_BOTTOM, int yAxisId=Y_LEFT) throw(std::exception);

    /**
     * Adds Data as a curve to the plot. The curve will be painted as black dots by default
     * but can be changed via setDataSytle with the returned int id.<br>
     * If the dataId is given data will be added to an existing data set.<br>
     * The x and y Axis which shall be used for the data can be specified too.
     * @param xPoints the x coordinates of the points
     * @param yPoints the y coordinates of the points
     * @param dataId the dataid of an existing data set, defaults to 0
     * @param xAxisId the x axis the data refers to, defaults to the constant X_BOTTOM
     * @param yAxisId the y axis the data refers to, defaults to the constant Y_LEFT
     * @return a unique id identifying the data. if existing data was modifyied this will be the same as the dataId given
     */
    int addPoints(const QList<double> &xPoints,const QList<double> &yPoints, int dataId=-1,
                int xAxisId=X_BOTTOM, int yAxisId=Y_LEFT) throw(std::exception);

    /**
     * Show or hides the data specified by dataId
     * @param dataId the id of teh data to hide or show
     * @param enable if the data shall be shown, defaults to true
     */
    void enableData(int dataId, bool enable=true);
    /**
     * Sets the style of data.
     * @param dataId the id of the data which shall be changed
     * @param pen the pen to use for the data
     * @param curveStlye the curveStyle for the data
     */
    void setDataStyle(int dataId, QPen pen, int curveStlye=DOTS);
    /**
     * Returns the QwtPlotCurve for the id given. this is only usefull if any
     * methods available to QwtPlotCurve are needed which are not wrapped here.
     * @param dataId the id of the data set to return
     * @return the QwtPlotCurve for the given id
     */
    QwtPlotCurve* getDataForId(int dataId) { return curves[dataId];};

    /**
     * Returns whether autoscrolling is enabled
     * @return true if enabled, false otherwise
     */
    bool isAutoscrolling() {return autoscrolling;};


    /**
     * Exports the plot as an image. A save dialog will be displayed where
     * one can choose the filename and the type of image.
     * Currently supported image formats can be viewed in the QtExporter class
     */
    void exportPlotAsImage();

    /**
     * Sets autoscrolling on or off. By default it enables
     * autoscrolling.
     * @param enable enable autoscrolling, defaults to true
     */
    void setAutoscrolling(bool enable=true);

    /**
     * Sets the plot to exactly fit the currently visible lines
     */
    void fitPlotToGraph();

    /**
     * Displays the options dialog
     */
    void showOptionsDialog();

    /**
     * Exports the currently visible curve as csv
     */
    void exportAsCSV();

    /**
     * Called when the grid is changed
     */
    void gridChanged();

    /**
     * Sets the canvas background color
     * @param color the color of the canvas background
     */
    void setBackgroundColor(QColor color);

    /**
     * Returns the background color
     * @return the canvas background color
     */
    QColor getBackgroundColor();

    /**
     * Method for an automated Test
     */
    void doTesting();

signals:
    void notifyUpdate(int type=0);

    
protected slots:


     /**
     * Refreshes the complete plot with the data currently stored within the datamanager
     */
    void refreshFromDataManager();

    /**
     * Slot handling change of the xBottom slider. You should
     * never need to call this manually
     * @param newValue the new value of the slider
     */
    void xBottomSliderValueChanged(double newValue);

    /**
     *  Slot handling change of the yLeft slider. You should
     * never need to call this manually
     * @param newValue the new value of the slider
     */
    void yLeftSliderValueChanged(double newValue);

    /**
     * Slot when the plot was zoomed. this will automatically
     * set all sliders and such. You should never need to call this manually
     * @param rect  teh rect to which the plot was zoomed
     */
    void zoomed(const QwtDoubleRect& rect);

    /**
     * Called when the options dialog ok button was clicked
     */
    void optionsChanged();

    /**
     * Called when the slider menu items are clicked
     */
    void sliderActionChecked();

    /**
     * Imports a curve from a csv file, displays a dialog to
     * choose a csv file from
     */
    void importFromCSV();

    /**
     * Removes all borderlines
     */
    void clearBorderLines();

    /**
     * Removes all curves
     */
    void clearCurves();

    /**
     * Removes all borderlines and all curves, simply
     * calls @see clearCurves and @see clearBorderLines
     */
    void clearAll();

    /** Called when curves were selected to be exorted*/
    void curvesSelected();

    /**
     * Loads a profile
     */
    void loadProfile();

    /**
     * Saves a Profile
     */
    void saveProfile();

    /**
     * Sets autoscaling, if enabled disables autoscrolling and fixed size
     * @param autoscale true if autoscaling shall be used
     */
    void setAutoscale(bool autoscale);

    /**
     * Returns whether autoscale is enabled
     * @return true if enabled, false otherwise
     */
    bool isAutoscale();

    /**
     * Sets if fixed size shall be enabled. If it is enabled
     * disables autoscale and autoscrolling
     * @param fixedSize if ficedSize shall be enabled
     */
    void setFixedSize(bool fixedSize);

    /**
     * Returns whether fixed size shall be used
     * @return true if fixed size is used, false otherwise
     */
    bool isFixedSize();


protected:

    /**
     * Adds Data as a curve to the plot. The curve will be painted as black dots by default
     * but can be changed via setDataSytle with the returned int id.<br>
     * If the dataId is given data will be added to an existing data set.<br>
     * The x and y Axis which shall be used for the data can be specified too.
     * @param xPoints the x coordinates of the points
     * @param yPoints the y coordinates of the points
     * @param length the length of the x and y points
     * @param dataId the dataid of an existing data set, defaults to 0
     * @param xAxisId the x axis the data refers to, defaults to the constant X_BOTTOM
     * @param yAxisId the y axis the data refers to, defaults to the constant Y_LEFT
     * @return a unique id identifying the data. if existing data was modifyied this will be the same as the dataId given
     */
    int addData(const double* xPoints,const  double* yPoints, int length, int dataId=-1,
		int xAxisId=X_BOTTOM, int yAxisId=Y_LEFT) throw(std::exception);


    // --> helper methods

    /**
     * Returns the axis constant for an internal constant, this
     * is necessary as ruby cannnot call the qwtplot enums directly
     * @param axis the constant defined in this class
     * @return the QwtPlot::Axis 
     */
    QwtPlot::Axis getAxisForInt(int axis);

    /**
     * Sets the zoom base of the zoomer to the actual boundary values.
     */
    void setZoomBase();
    /**
     * Sets the min and max values of the sliders according to the current grid
     */
    void setSliderValues();

    /**
     * Sets the minimum and maximum point if the given points are max or min
     * @param xPoint the x point to check
     * @param yPoint the y point to check
     */
    void setMinMaxPoints(double xPoint, double yPoint);


    /** Slider in x direction*/
    QwtSlider xBottomSlider;
    /** Slider in y direction*/
    QwtSlider yLeftSlider;
    /** layout used*/
    QGridLayout layout;
    /** the plot widget*/
    PlottingWidget plottingWidget;
    /** the grid*/
    QwtPlotGrid grid;
    /** general zoomer zooming in via selecting a rect with the mouse*/
    ExtendedPlotZoomer *zoomer;
    /** vector containing all border lines*/
    std::vector<QwtPlotMarker*> markers;
    /** the currentid of the marker*/
    int plotMarkerId;
    /** vector containing all curves*/
    std::vector<QwtPlotCurve*> curves;
    /** currentId of the curve*/
    int curveId;
    /** The span of the x axis*/
    double xSpan;
    /** The span of the y axis*/
    double ySpan;
    /** the minimum value of a point in x direction*/
    double minXBottom;
    /** the maximum value of a point in x direction*/
    double maxXBottom;
    /** the minimum value of a point in y direction*/
    double minYLeft;
    /** the maximum value of a point in y direction*/
    double maxYLeft;
    /** Whether autoscrolling is turned on*/
    bool autoscrolling;
    /** If scrolling in x direction is enabled*/
    bool scrollX;
    /** If scrolling in y direction is enabled*/
    bool scrollY;
    /** If autoscaling of the axis is enabled*/
    bool autoScale;
    /** If the plot is currently zoomed*/
    bool isZoomed;
    /** the xSpan when zoomed*/
    double zoomXSpan;
    /** the y span when zoomed*/
    double zoomYSpan;
    /** The rect which was initially used to display the plot*/
    QwtDoubleRect initialRect;
    /** MenuBar of the widget*/
    QMenuBar menuBar;
    /** Dialog with options*/
    OptionsDialog *optionsDialog;
    /** File menu*/
    QMenu fileMenu;
    /** Plot menu*/
    QMenu plotMenu;
    /** Export menu*/
    QMenu exportMenu;
    /** Slider menu*/
    QMenu sliderMenu;
    /** Grid menu*/
    QMenu gridMenu;
    /** Import menu*/
    QMenu importMenu;
    /** Clear menu*/
    QMenu clearMenu;
    /** Size Menu*/
    QMenu sizeMenu;
    /** Testing Menu*/
    QMenu testMenu;

    /** Action to export the plot as image*/
    QAction exportImageAction;
    /** Action to toggle autoscrolling*/
    QAction autoscrollAction;
    /** Action to fit the plot to the currently visible graphs*/
    QAction fitAction;
    /** Action to open the options dialog*/
    QAction optionsAction;
    /** Action to show/hide the left slider*/
    QAction leftSliderAction;
    /** Action to rexport the curve as csv*/
    QAction exportCSVAction;
    /** Action to show/hide the bottom slider*/
    QAction bottomSliderAction;
    /** Action to show or hide the x grid*/
    QAction xGridAction;
    /** Action to show or hide the y grid*/
    QAction yGridAction;
    /** Action to import a curve from a csv file*/
    QAction importCSVAction;
    /** Action to delete all BorderLines*/
    QAction clearBorderLineAction;
    /** Action to delete all curves*/
    QAction clearCurveAction;
    /** Action to delete all curves and borderlines*/
    QAction clearAllAction;
    /** Action to retrieve stored sessions*/
    QAction loadProfileAction;
    /** Action to save the profile*/
    QAction saveProfileAction;
    /** Action to set autoscaling*/
    QAction autoscaleAction;
    /** Action to set fixed size*/
    QAction fixedAction;
    /** Test Action*/
    QAction testAction;
    /** Action Group for Sizes*/
    QActionGroup sizeGroup;

    /** Datamanager containing all variables*/
    DataManager *dataManager;
    /** Dialog to select which curves shall be exported*/
    CurveSelectionDialog *curveSelectionDialog;
    /** Plot legend*/
    QwtLegend legend;
    /** Map containing the QTime for specific curves*/
    std::map<int, QTime*> timeMap;
    QTimer testTimer;
};

#endif	/* PLOTWIDGET_H */

