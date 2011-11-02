#include "qwaterfalldisplay.h"

#include <QtCore/QDebug>
#include <QtCore/QDate>
#include <QtGui/QPaintEvent>
#include <iostream>

#include <stdint.h>
#include <math.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////
// Constructors
QWaterfallDisplay::QWaterfallDisplay(int rows, int columns, QWidget *parent) :
    QFrame(parent), m_cols(columns), m_rows(rows), m_resizes(ScaleAspect), back_color(QColor("black")),
    m_max(1.0), m_min(0.0),
    dirty_lines(0), widget_resized(true), redraw_all(true), is_drawing(true),
    m_size_hint(0,0), m_minimum_size(0,0),
    m_buffer(QSize(rows,columns),QImage::Format_RGB32), waterfall(QSize(rows,columns),QImage::Format_RGB32), data()
{

    // Init the buffer
    m_buffer.fill(0);

    // Init the watterwafll image
    waterfall.fill(0);

    // Init the data Array
    initData(m_cols, m_rows);

    // calculates the size hints
    calcSizeHint();
}

QWaterfallDisplay::~QWaterfallDisplay()
{
}


//////////////////////////////////////////////////////////////////////////
// Public Functions

QSize QWaterfallDisplay::minimumSizeHint()
{
    return m_minimum_size;
}

QSize QWaterfallDisplay::sizeHint()
{
    return m_size_hint;
}

bool QWaterfallDisplay::isDrawing()
{
    return is_drawing;
}

bool QWaterfallDisplay::isPaused()
{
    return !is_drawing;
}



//////////////////////////////////////////////////////////////////////////
// Public Slots

// Property Handling
void QWaterfallDisplay::setColumns(int cols)
{
    initData(cols,m_rows); // resize the data storage
    m_cols = cols;
    emit columnsChanged(cols);

    // recalc size hints
    calcSizeHint();

    // redraw!
    redrawData();
}

int QWaterfallDisplay::columns() const
{
    return m_cols;
}

void QWaterfallDisplay::setRows(int rows)
{
    initData(m_cols,rows); // resize the data storage
    m_rows = rows;
    emit rowsChanged(rows);

    // recalc size hints
    calcSizeHint();

    // redraw!
    redrawData();
}

int QWaterfallDisplay::rows() const
{
    return m_rows;
}

void QWaterfallDisplay::setColumnsRows(int cols, int rows)
{
    initData(cols,rows); // resize the data storage
    m_cols = cols;
    m_rows = rows;
    emit columnsChanged(cols);
    emit rowsChanged(rows);

    // recalc size hints
    calcSizeHint();

    // redraw!
    redrawData();
}



void QWaterfallDisplay::setResizestyle(Resizestyle resizestyle)
{
    m_resizes = resizestyle;
    // same as geometry change
    widget_resized = true;
    update();
}

QWaterfallDisplay::Resizestyle QWaterfallDisplay::resizestyle() const
{
    return m_resizes;
}



void QWaterfallDisplay::setBackColor(QColor back)
{
    back_color = back;
    redrawData();
}

QColor QWaterfallDisplay::backColor() const
{
    return back_color;
}



void QWaterfallDisplay::setMaxValue(double max)
{
    m_max = max;
    // sanity check
    if (m_min >= m_max) {
        m_min = m_max-1.0;
        emit minChanged(m_min);
    }
    emit maxChanged(m_max);

    // redraw!
    redrawData();
}

double QWaterfallDisplay::maxValue() const
{
    return m_max;
}

void QWaterfallDisplay::setMinValue(double min)
{
    m_min = min;
    // sanity check
    if (m_min >= m_max) {
        m_max = m_min+1.0;
        emit maxChanged(m_max);
    }
    emit minChanged(m_min);

    // redraw!
    redrawData();
}

double QWaterfallDisplay::minValue() const
{
    return m_min;
}

void QWaterfallDisplay::setMaxMinValue(double max, double min)
{
    m_max = max;
    m_min = min;
    // sanity check
    if (m_min >= m_max)
        m_min = m_max-1.0;
    emit maxChanged(m_max);
    emit minChanged(m_min);

    // redraw!
    redrawData();
}

//// Data Handling Slots
void QWaterfallDisplay::pushData(const vector<float> &new_data)
{
    // push it to the std::list
    data.push_front(new_data);

    if (data.front().size() > m_cols) {
        // shorten the thing
        data.front().resize(m_cols);
    }


    // check wether we have grown too much
    while (data.size() > m_rows) {
        data.pop_back();
    }

    // increase the number of lines to be updated
    dirty_lines++;

    // redraw!
    update();
}

void QWaterfallDisplay::pushDataQList(const QList<float> &new_data)
{
    uint i=0;
    vector<float> temp(new_data.size());

    if ((uint)(new_data.size()) > m_cols) {
        temp.resize(m_cols);
        for(i=0;i<m_cols;i++)
            temp[i] = new_data[i];
    }
    else {
        //temp.resize(new_data.size());
        for(i=0;i<temp.size();i++)
            temp[i] = new_data[i];
    }

    // do the real push
    pushData(temp);
}

void QWaterfallDisplay::pushDataFloat(const float *new_data, int length)
{
    vector<float> temp(new_data, new_data + length);
    if ((uint)(length)>m_cols)
        temp.resize(m_cols);

    // do the real push
    pushData(temp);
}


void QWaterfallDisplay::pushDataUint8(const char *new_data, int length)
{
    uint i=0;
    vector<float> temp(length);

    if (length > m_cols) {
        temp.resize(m_cols);
        for(i=0;i<m_cols;i++)
            temp[i] = (float)(new_data[i]);
    }
    else {
        //temp.resize(length);
        for(i=0;i<length;i++)
            temp[i] = (float)(new_data[i]);
    }

    // do the real push
    pushData(temp);
}

void QWaterfallDisplay::pushDataUint16(const char *new_data_in, int length_in)
{
    uint i=0;
    uint16_t* new_data = (uint16_t*)new_data_in;
    uint length = length_in/2;
    vector<float> temp(length);

    if (length > m_cols) {
        temp.resize(m_cols);
        for(i=0;i<m_cols;i++)
            temp[i] = (float)(new_data[i]);
    }
    else {
        //temp.resize(length);
        for(i=0;i<length;i++)
            temp[i] = (float)(new_data[i]);
    }

    // do the real push
    pushData(temp);
}

void QWaterfallDisplay::pushDataUint32(const char *new_data_in, int length_in)
{
    uint i=0;
    uint32_t* new_data = (uint32_t*)new_data_in;
    uint length = length_in/4;
    vector<float> temp(length);

    if (length > m_cols) {
        temp.resize(m_cols);
        for(i=0;i<m_cols;i++)
            temp[i] = (float)(new_data[i]);
    }
    else {
        //temp.resize(length);
        for(i=0;i<length;i++)
            temp[i] = (float)(new_data[i]);
    }

    // do the real push
    pushData(temp);
}

void QWaterfallDisplay::clearData()
{
    data.clear();  // I am not sure if this really deletes the content -> check (jca)

    // redraw!
    redrawData();
}


//// drawing stuff
void QWaterfallDisplay::redrawData()
{
    redraw_all = true; // signals paintEvent to do a complete overhaul
    //if(!(data.isEmpty()))
        update();          // envoces a repaint
}

void QWaterfallDisplay::pauseDrawing(bool onoff)
{
    is_drawing = onoff;

    emit drawing(is_drawing);
    emit paused(!is_drawing);

    if (is_drawing)
        emit drawingStarted();
    else
        emit drawingPaused();
}


//////////////////////////////////////////////////////////////////////////
// Protected Functions
void QWaterfallDisplay::paintEvent(QPaintEvent *event)
{
    //
    // This function is currently the ONE AND ONLY function for PAINTING!!!
    //

    // init the painter for the widget
    QPainter widget_painter(this);

    // this is needed for a lot of copying in the function
    QRect target;
    QRect source;

    // Do we have to redraw all or part of the widget due to changes in data or size
    if (redraw_all || widget_resized || (dirty_lines>0)) {


        // new content and we draw
        if (is_drawing && (redraw_all || (dirty_lines>0))) {
        //if ((redraw_all || (dirty_lines>0))) {
            QSize waterfall_size(m_cols,m_rows);
            QImage new_waterfall(waterfall_size,QImage::Format_RGB32);
            new_waterfall.fill(0);
            QPainter waterfall_painter(&new_waterfall);
            waterfall_painter.fillRect(new_waterfall.rect(),back_color);

            if (!(data.empty())) { // for safety reasons
                QRgb* t_line = NULL;
                list< vector<float> >::const_iterator it;

                // we have to redraw the widget
                if (redraw_all) {
                    int it_count = 0;
                    // draw the widget line-by-line
                    for(it = data.begin(); it != data.end(); it++) {
                        t_line = (QRgb*)(new_waterfall.scanLine(it_count));
                        drawLine(t_line, *it);
                        it_count++;
                    }

                    // there are no dirty lines anymore 8]
                    // neede to prevent a go in the next if, for the case of dirty_lines AND redraw_all
                    dirty_lines = 0;
                }
                else if (dirty_lines>0) {
                    // first copy the first height-dirty_lines from the old
                    // waterfall
                    target.setTopLeft(QPoint(0,dirty_lines));
                    target.setWidth(new_waterfall.width());
                    target.setHeight((new_waterfall.height())-dirty_lines);
                    source.setTopLeft(QPoint(0,0));
                    source.setWidth(waterfall.width());
                    source.setHeight((waterfall.height())-dirty_lines);

                    waterfall_painter.drawImage(target,waterfall,source);

                    int it_count = 0;
                    for(it = data.begin(); ((dirty_lines>0) && (it != data.end())); it++) {
                        t_line = (QRgb*)(new_waterfall.scanLine(it_count));
                        drawLine(t_line, *it);
                        it_count++;
                        dirty_lines--;
                    }
                }

            }

            // end painting
            waterfall_painter.end();


            // switch the waterfall
            waterfall = new_waterfall;

            // clear the flags
            redraw_all = false;
            dirty_lines = 0;

            // force a new paint via resize
            widget_resized = true;
        }


        // widget resized or new content (flag set to dirty above)
        if (widget_resized) {
            QImage new_buffer(size(),QImage::Format_RGB32);
            new_buffer.fill(0);

            QPainter buffer_painter(&new_buffer);
            buffer_painter.fillRect(new_buffer.rect(), back_color);

            QSize copy_size;
            float wf_aspect = 0;
            float bf_aspect = 0;
            int m_y = 0;
            int m_x = 0;
            int m_width = 0;
            int m_height = 0;


            // Here is the big switch for resizing and painting it into the new buffer
            switch(m_resizes) {
            case Scale:
                // this is akward, but easy to implement
                target = new_buffer.rect();
                source = waterfall.rect();
                buffer_painter.drawImage(target,waterfall,source);
                break;

            case ScaleAspect:
                // scales with respect to the aspect ratio of waterfall,
                // looks nicer than Scale but still...
                source = waterfall.rect();
                wf_aspect = (float)(waterfall.width())/(float)(waterfall.height());
                bf_aspect = (float)(new_buffer.width())/(float)(new_buffer.height());
                if (wf_aspect >= bf_aspect) {
                    // scale to width of new_buffer and center in height
                    m_width = new_buffer.width();
                    m_height = (int)((float)((new_buffer.width())*(waterfall.height()))/(float)(waterfall.width()));
                    m_y = ((new_buffer.height()) - m_height) / 2;
                }
                else {
                    // scale to height of new buffer and center in width
                    m_height = new_buffer.height();
                    m_width = wf_aspect * (float)(new_buffer.height());
                    m_x = ((new_buffer.width()) - m_width) / 2;
                }
                target.setTopLeft(QPoint(m_x,m_y));
                target.setHeight(m_height);
                target.setWidth(m_width);
                buffer_painter.drawImage(target,waterfall,source);
                break;

            case ScaleEven:
                // Scales only n times columns while keeping the aspect ratio and the centers:
                // widgetsize > n*cols -> centered
                // is a bit more true to the values painted
                source = waterfall.rect();
                qDebug()<<"QWaterfallDisplay: Resize Type:"<<m_resizes<<"not implemented ATM!";
                break;

            case Cut:
                // just put it into the window and cut of what can not be seen
                target.setTopLeft(QPoint(0,0));
                source.setTopLeft(QPoint(0,0));
                if (new_buffer.width() > waterfall.width())
                    copy_size.setWidth(waterfall.width());
                else
                    copy_size.setWidth(new_buffer.width());
                if (new_buffer.height() > waterfall.height())
                    copy_size.setHeight(waterfall.height());
                else
                    copy_size.setHeight(new_buffer.height());
                target.setSize(copy_size);
                source.setSize(copy_size);

                // draw it
                buffer_painter.drawImage(target,waterfall,source);
                break;

            case CutMixed:
                qDebug()<<"QWaterfallDisplay: Resize Type: "<<m_resizes<<" not implemented ATM!";
                break;

            default:
                qDebug()<<"QWaterfallDisplay: Unkown Resize Type: "<<m_resizes;
                break;
            }

            // end painting
            buffer_painter.end();

            // switch the buffer
            m_buffer = new_buffer;

            // clear the flags
            widget_resized = false;
        }


        // Copy the the buffer onto the widget
        widget_painter.drawImage(event->rect(),m_buffer,event->rect());
    }
    else {
        // we have to copy only the rect of the paint event
        // remember m_buffer is exactly the size of the widget!
        widget_painter.drawImage(event->rect(),m_buffer,event->rect());
    }

    // just to be safe
    widget_painter.end();
}

void QWaterfallDisplay::resizeEvent(QResizeEvent *event)
{
    widget_resized = true; // signals the paintEvent, that a resize has occured
}

//////////////////////////////////////////////////////////////////////////
// Private Functions
void QWaterfallDisplay::initData(uint cols, uint rows)
{
    // Action depends on current state of the system

    // List is empty (for whatever reasons)
    // We have nothing to do
    if(data.empty()) {
        return;
    }

    // The number oc columns hasn't changed, but rows has gotten smaller
    // we have to throw all exessive data away
    if((cols == m_cols) && (rows < m_rows)) {
        while (data.size() > rows) {
            data.pop_back();
        }
    }

    // The number of columns has changed
    // we kill the list, regardless if rows has changed
    if (cols != m_cols) {
        data.clear();  // I am not sure if this really deletes the content -> check (jca)
    }
}

void QWaterfallDisplay::drawLine(QRgb *line, const vector<float> &data_line)
{
    int index = 0;

    // all the following code presumes, that there is no data_line bigger than the image line
    // but this should never happen anyway... *hust*

    for (uint i=0;i<data_line.size();i++) {
        //std::cerr << QString(" %1").arg(t_line[i],6,'g',3).toAscii().data();

        // this all should be just prelimenary
        // Color Selection Begin
        index = (int)( (((data_line[i]+fabs(m_min)) / (m_max+fabs(m_min))) * 255.0) + 0.5 );
        index = (index<0)?0:index;
        index = (index>255)?255:index;
        // Color Selection End

        // write a corresponding RGB value
        line[i] = qRgb(index,index,index);
    }
}


void QWaterfallDisplay::calcSizeHint()
{
    // the size hint
    m_size_hint.setWidth(m_cols);
    m_size_hint.setHeight(m_rows);

    // minimum size hint
    m_minimum_size.setWidth(m_cols/4);
    m_minimum_size.setHeight(m_rows/4);

    // minimum size
    setMinimumWidth(m_cols/8);

    // update the geometry
    updateGeometry();
}
