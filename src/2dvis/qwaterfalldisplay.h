#ifndef QWATERFALLDISPLAY_H
#define QWATERFALLDISPLAY_H

#include <QtGui/QFrame>
#include <QtGui/QColor>
#include <QtGui/QImage>
#include <QtCore/QSize>
#include <QtCore/QList>
#include <QtGui/QPainter>

#include <QtDesigner/QDesignerExportWidget>

#include <list>
#include <vector>

/*

 In General: Pretty straight forward (I think)

 Remarks:
 - Data is internally sored as a queue of QVarLengthArrays. This is done purely for performance reasons, so
   that we don't encoounter some performance issues due to Qts implicit sharing
 - Currently this class is NOT REENTRANT per se! I had to find that out the hard way.
 - Drawing is olely done within paintEvent, the data handling is decoupled.

 */
class QDESIGNER_WIDGET_EXPORT QWaterfallDisplay : public QFrame
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Jan Albiez")
    Q_PROPERTY(uint rows READ rows WRITE setRows NOTIFY rowsChanged DESIGNABLE true)
    Q_PROPERTY(uint columns READ columns WRITE setColumns NOTIFY columnsChanged DESIGNABLE true)
    Q_PROPERTY(Resizestyle resizestyle READ resizestyle WRITE setResizestyle DESIGNABLE true)
    Q_PROPERTY(QColor backColor READ backColor WRITE setBackColor DESIGNABLE true)
    Q_PROPERTY(double maxValue READ maxValue WRITE setMaxValue NOTIFY maxChanged DESIGNABLE true)
    Q_PROPERTY(double minValue READ minValue WRITE setMinValue NOTIFY minChanged DESIGNABLE true)
    Q_ENUMS(Resizestyle)

public:
    enum Resizestyle {
            Scale,       // Just Scales (could be akward)
            ScaleAspect, // Scales, keeping the aspect ratio and centers
            ScaleEven,   // Scales only n times where widgetsize > n*rows and then centers for n<1 scales like ScaleAspect
            Cut,         // Just cuts if the widget is smaller and centers if bigger
            CutMixed     // Scales cols even and cuts rows
    };

    explicit QWaterfallDisplay(int rows=1, int columns=1, QWidget *parent = 0);
    ~QWaterfallDisplay();

    QSize minimumSizeHint();
    QSize sizeHint();

    // quering the drawing status
    bool isPaused();
    bool isDrawing();


signals:

    void rowsChanged(uint);
    void columnsChanged(uint);

    void minChanged(double);
    void maxChanged(double);

    void drawingPaused();
    void drawingStarted();
    void drawing(bool);
    void paused(bool);


public slots:

    // General Behavior and Display stuff

    // Setting the Properties columns and rows
    // Warning: Throws away the current data and start with a blank display!
    void setColumns(int cols);
    void setRows(int rows);
    void setColumnsRows(int cols, int rows); // for conveniance
    int columns() const;
    int rows() const;

    // Changes the resizestyle. Just makes the thing more ugly or nicer
    void setResizestyle(Resizestyle resizestyle);
    Resizestyle resizestyle() const;

    // changes the backgroundcolor -> the area where no waterfalldisplay is within the widget (default == black)
    void setBackColor(QColor back);
    QColor backColor() const;

    // Setting and Getting the properties for max and min values
    // Warning: forces a repaint of the whole thingy
    void setMaxValue(double max);
    void setMinValue(double min);
    void setMaxMinValue(double max, double min); // for conveniance
    double maxValue() const;
    double minValue() const;


    // Data Handling

    // Setting new Data
    // These functions push a new data set into the display queue. If the cueue is longer than rows, the last
    // line will be discarded (FIFO)

    // The main data setting function
    // Uses the values in the vector. The vector will be hardcore truncated to the current columns value,
    // in the case of the shorter vector, the end of the line will be filled with the background color
    // while drawing. The vector is copied.
    void pushData(const std::vector<float> &new_data);

    // the following functions are implemented for onvenience (expand if you like).
    // They all follow the same principle: Transform the input into a std::vector and then call
    // pushData(std::vector<float>)

    // Uses the values in the double array. the new_data array has elements elements
    // The data will be copied.
    void pushDataFloat(const float *new_data, int elements);

    // Uses the values in the list. The rules of pushData(std::vector<float>) apply here as well
    void pushDataQList(const QList<float> &new_data);

    // Interprets the data in the char* as uint_8. new_data has the length (memory) of the parameter lenthg
    // The data will be copied.
    void pushDataUint8(const char *new_data, int length);

    // Interprets the data in the char* as uint_16. new_data has the length (memory) of the parameter lenthg
    // The data will be copied.
    void pushDataUint16(const char *new_data, int length);

    // Interprets the data in the char* as uint_32. new_data has the length (memory) of the parameter lenthg
    // The data will be copied.
    void pushDataUint32(const char *new_data, int length);


    // clear the data
    void clearData();


    // Drawing stuff

    // redraws the complete display, using the current data
    void redrawData();

    // pauses the drawing of new data
    // emits:
    // - drawingPaused() when stopped
    // - drawigStarted() when started again
    // - drawing(bool), paused(bool) when toggeled
    void pauseDrawing(bool onoff);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:

    // Functions

    // (re)init the datastorage, you should force a redraw after calling this function!
    void initData(uint cols, uint rows);

    // draws one line to the image
    void drawLine(QRgb *line, const std::vector<float> &data_line);

    // calculates the size hint variables and calls updateGeometry
    void calcSizeHint();

    // Members
    uint m_cols, m_rows;

    Resizestyle m_resizes; // internal storage resize style

    QColor back_color;    // internal storage of the back_color

    double m_max, m_min;  // internal storage for max and min

    int  dirty_lines;     // used to signal paintEvent the lines to change (quick draw)
    bool widget_resized;  // flag from resizeEvent to paintEvent
    bool redraw_all;      // flag to force a complete redraw

    bool is_drawing;      // flag for storing the drawing state

    // geometry handling
    QSize m_size_hint;
    QSize m_minimum_size;

    // the image buffers
    QImage m_buffer;      // stores the widget state for quick repainting when no data has changed
    QImage waterfall;     // stores the unscaled waterfall

    std::list< std::vector<float> > data;
};

#endif // QWATERFALLDISPLAY_H
