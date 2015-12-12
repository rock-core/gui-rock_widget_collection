#ifndef SONARPLOT_H
#define SONARPLOT_H

#include <QtGui>
#include <QFrame>
#include <base/samples/SonarScan.hpp>
#include <base/samples/Sonar.hpp>
#include <frame_helper/ColorGradient.h>

#define BASE_WIDTH      1300
#define BASE_HEIGHT      600
#define BINS_REF_SIZE    500

using namespace frame_helper;


class SonarPlot : public QFrame
{
    Q_OBJECT


public:
    SonarPlot(QWidget *parent = 0);
    virtual ~SonarPlot();
    void setData(const base::samples::SonarScan scan);
    void setData(const base::samples::Sonar scan);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent ( QResizeEvent * event );
    void drawOverlay();
    void generateBearingTable(base::samples::SonarScan scan);
    void generateTransferTable(int nbeams, int nbins);
    void applyColormap(ColorGradientType type);
    base::samples::SonarScan lastSonarScan;
    base::samples::Sonar lastSonar;
    bool changedSize;
    double scaleX;
    double scaleY;
    int range;
    QPoint origin;
    std::vector<base::Angle> bearingTable;
    std::vector<int> transfer;
    std::vector<QColor> colorMap;
    ColorGradient heatMapGradient;
    

protected slots:
    void rangeChanged(int);
    void sonarPaletteChanged(int);
    
};


#endif
