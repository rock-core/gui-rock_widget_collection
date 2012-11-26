#ifndef SONARWIDGET_H
#define SONARWIDGET_H

#include <QtGui>
#include "base/samples/sonar_scan.h"

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
    QComboBox* comboRange;
    QLineEdit *edGain;
      
public:
    SonarWidget(QWidget *parent = 0);
    virtual ~SonarWidget();
    QSlider *slGain;

public slots:
    void setData(const base::samples::SonarScan scan);
    void setGain(int);
    void setRange(int);

protected slots:
    void onSlGainChanged(int);
    void onComboRangeChanged(QString);

signals:
    void gainChanged(int);
    void rangeChanged(int);
    
};

#endif /* SONAR_WIDGET_H */
