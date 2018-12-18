#ifndef SONARWIDGET_H
#define SONARWIDGET_H

#include <QtGui>
#include <base/samples/SonarScan.hpp>
#include <base/samples/Sonar.hpp>

class SonarPlot;

class SonarWidget : public QWidget
{
    Q_OBJECT
protected:
    SonarPlot *plot;
    double scaleX;
    double scaleY;
    void resizeEvent ( QResizeEvent * event );
    QLabel *lbGain;
    QLabel *lbRange;
    QLabel *lbPalette;
    QLabel *lbGrid;
    QLineEdit *edGain;
    QLineEdit *edRange;

public:
    SonarWidget(QWidget *parent = 0);
    virtual ~SonarWidget();
    void createGainComponent();
    void createRangeComponent();
    void createPaletteComponent();
    void createGridComponent();
    QSlider *slGain;
    QSlider *slRange;
    QComboBox *comboPalette;
    QCheckBox *boxGrid;

public slots:
    void setData(const base::samples::SonarScan scan);
    void setData(const base::samples::Sonar sonar);
    void setMotorStep(const base::Angle step);
    void setGain(int);
    void setRange(int);
    void setMaxRange(int);
    void setMinRange(int);
    void setSonarPalette(int);
    void enableAutoRanging(bool);
    int getRange() const;
    int getGain() const;

    // only for scanning sonars
    void setSectorScan(bool continuous, base::Angle left, base::Angle right);

protected slots:
    void onSlGainChanged(int);
    void onSlRangeChanged(int);
    void onComboPaletteChanged(int);
    void onCheckboxGridChanged(bool);

signals:
    void gainChanged(int);
    void rangeChanged(int);
    void sonarPaletteChanged(int);
    void gridChanged(bool);
};

#endif /* SONAR_WIDGET_H */
