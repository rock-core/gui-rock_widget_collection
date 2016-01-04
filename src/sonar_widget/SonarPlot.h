#ifndef SONARPLOT_H
#define SONARPLOT_H

#include <QtGui>
#include <QFrame>
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
    void setData(const base::samples::Sonar& sonar);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent ( QResizeEvent * event );
    void drawOverlay();
    void generateMultibeamTransferTable(const base::samples::Sonar& sonar);
    void generateScanningTransferTable(const base::samples::Sonar& sonar);
    void applyColormap(ColorGradientType type);
    bool isMotorStepChanged(const base::Angle& bearing);
    void addScanningData(const base::samples::Sonar& sonar);
    base::samples::Sonar lastSonar;
    bool changedSize;
    bool isMultibeamSonar;
    double scaleX;
    double scaleY;
    int range;
    QPoint origin;
    std::vector<int> transfer;
    std::vector<QColor> colorMap;
    ColorGradient heatMapGradient;
    base::Angle motorStep;
    int numSteps;
    std::vector<float> sonarData;

protected slots:
    void rangeChanged(int);
    void sonarPaletteChanged(int);
    
};


#endif
