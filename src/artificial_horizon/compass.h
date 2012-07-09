#ifndef _ARTIFICIALCOMPASS_H_
#define _ARTIFICIALCOMPASS_H_

#include <MultiWidget.h>
//#include <QWidget>

class QHBoxLayout;
class QVBoxLayout;
class QwtCompass;
class QwtCompassMagnetNeedle;
class QLabel;
class QLCDNumber;

class Compass : public MultiWidget 
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Matthias Goldhoorn")

public:
    Compass(QWidget *parent=NULL);
//    virtual ~Compass(); //Attention if you comment in this the widgets are not longer visible in designer WTF?

public slots:
    Compass* newInstance();
    void setHeading(double value);

protected:
    QVBoxLayout *layout;
    double heading;
    QwtCompass *compass;		
    QwtCompassMagnetNeedle *needle;
    QLabel *number;
};

#endif //Compass
