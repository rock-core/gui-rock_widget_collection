#include <QtGui/QApplication>

#include "PhononImageView.h"

#include <phonon/MediaObject>

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);
    app.setApplicationName("PhononImageView Test");
    
    PhononImageView *phonon_image_view = new PhononImageView;
    phonon_image_view->show();
    
    //Phonon::MediaObject *mediaObject = new Phonon::MediaObject(phonon_image_view);
    phonon_image_view->go();
    

    return app.exec();
}
