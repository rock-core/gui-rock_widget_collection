/****************************************************************************
 *
 * Timeline Main Widget.
 * 
 * @author Allan E. Conquest - allan.conquest@dfki.de
 *
 ***************************************************************************/

#ifndef TIMELINE_H
#define TIMELINE_H

#include <QtGui>
#include "BoundarySlider.h"
#include "SlideBarItem.h"

/**
 * This widget provides an improved Slider. Besides the standard slider 
 * functionality you can mark a region on the slide bar to select a certain
 * interval between two indices. There are two markers: the start marker (left)
 * and the end marker (right). The thin vertical slider is the index slider 
 * which is the pendant to the standard slider.
 * 
 * @author Allan E. Conquest - allan.conquest@dfki.de
 */
class Timeline : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(int bookmark_height READ getBookmarkHeight WRITE setBookmarkHeight)
    Q_PROPERTY(int margin_topbot READ getMarginTopBot WRITE setMarginTopBot)
    Q_PROPERTY(int margin_lr READ getMarginLR WRITE setMarginLR)
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(int width READ getWidth WRITE setWidth)
    Q_PROPERTY(unsigned minIndex READ getMinIndex WRITE setMinIndex)
    Q_PROPERTY(unsigned steps READ getSteps WRITE setSteps)
    Q_PROPERTY(unsigned stepSize READ getStepSize WRITE setStepSize)

public:
    Timeline(QWidget* parent = 0);

public slots:
    /* Property functions */
    int getBookmarkHeight() const;
    void setBookmarkHeight(int height);
    int getMarginTopBot();
    void setMarginTopBot(int margin);
    int getMarginLR();
    void setMarginLR(int margin);
    const QColor& getBackgroundColor() const;
    void setBackgroundColor(const QColor & color);
    int getWidth();
    void setWidth(int width);
    int getMinIndex();
    void setMinIndex(int minIndex);
    int getSteps();
    void setSteps(int steps);
    int getStepSize();
    void setStepSize(int stepSize);
    
    /* Other slots */
    
    /**
     * Returns the maximum index, i.e. the highest index on the timeline.
     */
    int getMaxIndex();
    
    /**
     * Returns the current index of the index slider.
     */
    int getSliderIndex();
    
    /**
     * Returns the current index of the start marker.
     */
    int getStartMarkerIndex();
    
    /**
     * Sets the index of the start marker.
     */
    void setStartMarkerIndex(int idx);
    
    /**
     * Returns the current index of the start marker.
     */
    int getEndMarkerIndex();
    
    /**
     * Sets the index of the end marker.
     */
    void setEndMarkerIndex(int idx);
    
    /**
     * Sets the index of the index slider.
     */
    void setSliderIndex(int idx);
    
    /**
     * Adds a bookmark for the specified index. Only one bookmark per index is possible.
     */
    void addBookmark(int idx);
    
    /**
     * Adds multiple bookmarks at once. See addBookmark().
     */
    void addBookmarks(QVector<int> bookmarks);
    
    /**
     * Removes the bookmark at the specified index if there one.
     */
    void removeBookmark(int idx);
    
    /**
     * Removes all bookmarks.
     */
    void removeAllBookmarks();
    
    /**
     * Returns true if there is a bookmark located at the specified index.
     */
    bool hasBookmarkAtIndex(int idx);

signals:

    void indexSliderClicked();
    void endMarkerClicked();
    void startMarkerClicked();
    void indexSliderReleased(int idx);
    void startMarkerMoved(int idx);
    void endMarkerMoved(int idx);
    void indexSliderMoved(int idx);
    void startMarkerReleased(int idx);
    void endMarkerReleased(int idx);

    void bookmarkClicked(int idx);
    void bookmarkRemoved(int idx);
    void timeout();

protected:
    void resizeEvent(QResizeEvent * event);

private slots:
    void bookmarkClickedSlot(int idx);
    void fireTimeout();
    void sliderReleased(Slider* slider, int idx);
    void sliderMoved(Slider* slider, int idx);
    void sliderClicked(Slider* slider);
    
private:
    int ordered_width;
    QGraphicsScene *scene;
    SlideBarItem *slidebar;
    BoundarySlider *startmarker;
    BoundarySlider *endmarker;
    QVector<int> bookmarks;
    QTimer *timer;
    bool reconfigure_slidebar;
    
    int bookmarkHeight;
    int marginTopBot;
    int marginLR;
    QColor bgColor;
    unsigned minIndex;
    unsigned steps;
    unsigned stepSize;
    
    void updateScene();
    void updateScene(QSizeF newsize);
    void setSliderIndex(int idx, Slider* slider);
    void reconfigureSlidebar();
    
};

#endif // TIMELINE_H
