#ifndef _ORIENTATIONVIEW_H_
#define _ORIENTATIONVIEW_H_

#include <MultiWidget.h>
//#include <QWidget>

class QHBoxLayout;
class Compass;
class ArtificialHorizon;

class OrientationView : public MultiWidget 
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Matthias Goldhoorn")

public:
    OrientationView(QWidget *parent=NULL);
//    virtual ~Orientation(); //Attention if you comment in this the widgets are not longer visible in designer WTF?

public slots:
		
    OrientationView* newInstance();
    void setHeading(double value);
    void setPitch(double value);
    void setRoll(double value);
    void setHeadingAngle(double value){setHeading(value);}
    void setPitchAngle(double value){setPitch(value);}
    void setRollAngle(double value){setRoll(value);}
		void update(){}; //Nothig needs to be done but needs to be implemnted for ruby bindings
protected:
		QHBoxLayout *layout;
		Compass *compass;
		ArtificialHorizon *horizon;
};

#endif //Orientation
