#ifndef SONARPLOT_H
#define SONARPLOT_H

#include <QtGui>
#include <QFrame>
#include "base/samples/sonar_scan.h"

class SonarPlot : public QFrame
{
    Q_OBJECT

public:
    SonarPlot(QWidget *parent = 0);
    virtual ~SonarPlot();
    void setData(const base::samples::SonarScan scan);
    
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent ( QResizeEvent * event );
    void drawOverlay();
    void fillPoints();
    base::samples::SonarScan mSonarScan;
    bool mChangedSize;
    QList<QPoint> mRawPoints;
    QList<QPoint> mPoints;
    double scaleX;
    double scaleY;
    int number_of_bins; //Number of bins in a single beam
    int range;
    QVector<QColor> colorMap;
    
protected slots:
    void rangeChanged(int);
    
};


#endif
