#include "VirtualJoystick.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication qapp(argc, argv);
    
    VirtualJoystick *bla = new VirtualJoystick(0, std::string("Test"));
    
    bla->show();
    
    qapp.exec();
    
    return 0;
}