#include "JoystickWidget.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication qapp(argc, argv);
    
    JoystickWidget *bla = new JoystickWidget(0, std::string("Test"));
    
    bla->show();
    
    qapp.exec();
    
    return 0;
}