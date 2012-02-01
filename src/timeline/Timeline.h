/****************************************************************************
**
** Timeline Main Widget.
**
****************************************************************************/

#ifndef TIMELINE_H
#define TIMELINE_H

#include <QtGui>
#include "BoundarySlider.h"
#include "SlideBarItem.h"

class Timeline : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(int bookmark_height READ getBookmarkHeight WRITE setBookmarkHeight)
    Q_PROPERTY(int index_slider_height READ getIndexSliderHeight WRITE setIndexSliderHeight)
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
    /* PROPERTIES */
    int getBookmarkHeight() const;
    void setBookmarkHeight(int height);
    int getIndexSliderHeight() const;
    void setIndexSliderHeight(int height);
    int getMarginTopBot();
    void setMarginTopBot(int margin);
    int getMarginLR();
    void setMarginLR(int margin);
    const QColor& getBackgroundColor() const;
    void setBackgroundColor(const QColor & color);
    int getWidth();
    void setWidth(int width);
    unsigned getMinIndex();
    void setMinIndex(unsigned minIndex);
    unsigned getSteps();
    void setSteps(unsigned steps);
    unsigned getStepSize();
    void setStepSize(unsigned stepSize);
    
    int getSliderIndex();
    int getStartMarkerIndex();
    int getEndMarkerIndex();
    void setSliderIndex(int idx);
    void addBookmark(int idx);
    void addBookmarks(QVector<int> bookmarks);
    void removeBookmark(int idx);

signals:
    void bookmarkRemoved(int idx);
    void timeout();

protected:
    void resizeEvent(QResizeEvent * event);

private slots:
    void fireTimeout();
    
private:
    int ordered_width;
    QGraphicsScene *scene;
    SlideBarItem *slidebar;
    BoundarySlider *startmarker;
    BoundarySlider *endmarker;
    QVector<int> bookmarks;
    QTimer *timer;
    
    int bookmarkHeight;
    int indexSliderHeight;
    int marginTopBot;
    int marginLR;
    QColor bgColor;
    unsigned minIndex;
    unsigned steps;
    unsigned stepSize;
    
    void updateScene();
    void updateScene(QSizeF newsize);
    void setSliderIndex(int idx, Slider* slider);
    
};

#endif // TIMELINE_H
