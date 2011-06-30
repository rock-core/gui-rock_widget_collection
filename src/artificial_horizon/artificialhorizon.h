#ifndef ARTIFICIALHORIZON_H
#define ARTIFICIALHORIZON_H


#include <QtGui/QWidget>
#include <QtGui/QPixmap>

class ArtificialHorizon:public QWidget
{
    Q_OBJECT

public:
    ArtificialHorizon(QWidget *parent=NULL);
    ~ArtificialHorizon();

    void paintEvent(QPaintEvent *);
		void resizeEvent ( QResizeEvent * event );

public slots:
    void setDataDir(QString const& data_dir);
    ArtificialHorizon* newInstance();
    void setRollAngle(double myvalue);
    void setPitchAngle(double myval);

private:
    double rollangle;
    double pitchangle;

    QPixmap background;
    QImage stationary;
    QImage rotate;
    QImage dial;   
    QPixmap background_o;
    QImage stationary_o;
    QImage rotate_o;
    QImage dial_o;   
};
#endif // ARTIFICIALHORIZON_H
