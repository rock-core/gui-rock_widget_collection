#ifndef SONARPLOT_H
#define SONARPLOT_H

#include <QtGui>
#include <QFrame>
#include "base/samples/sonar_scan.h"


#define BASE_WIDTH      1000
#define BASE_HEIGHT      600

struct SonarPlotPixel{
    QPoint point;
    uint index;
};

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
    base::samples::SonarScan sonarScan;
    base::samples::SonarScan lastSonarScan;
    bool changedSize;
    double scaleX;
    double scaleY;
    int range;
    QVector<QColor> colorMap;
    QList<SonarPlotPixel> pixelList;
    QPoint origin;
    
protected slots:
    void rangeChanged(int);
    
};


#endif