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

public:
    Timeline(qreal width, unsigned startIndex, unsigned steps, unsigned stepSize, QWidget* parent = 0);

public slots:
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
    qreal ordered_width;
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
    
    void updateScene();
    void updateScene(QSizeF newsize);
    void setSliderIndex(int idx, Slider* slider);
    
};

#endif // TIMELINE_H
