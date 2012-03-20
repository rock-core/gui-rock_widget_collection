#include <QtGui/QApplication>

#include "NewImageView.h"

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);

    NewImageView newimageview;
    newimageview.show();

    return app.exec();
}
