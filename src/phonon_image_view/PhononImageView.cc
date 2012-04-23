#include <qt4/phonon/mediaobject.h>

#include <iostream>

#include "PhononImageView.h"
#include "FrameDevice.h"

using namespace Phonon;

PhononImageView::PhononImageView(QWidget *parent)
    : QWidget(parent)
{
    
    frameDevice = new FrameDevice;
    
    resize(300,120);

    
    // Set up media graph
    mediaObject = new MediaObject(this);
    mediaObject->setCurrentSource(MediaSource(frameDevice));
    //mediaObject->setCurrentSource(MediaSource("/home/aconquest/dev/logfiles/20100407_DFKI-Robotics_Imagefilm_eng.avi"));
    
    videoWidget = new VideoWidget(this);
    createPath(mediaObject, videoWidget);
    
    
    // Appearence
    QVBoxLayout vbox(this);
    //vbox.addWidget(new QLabel("Phonon Video Widget below:"));
    vbox.addWidget(videoWidget);
    vbox.setAlignment(Qt::AlignVCenter);

}

PhononImageView::~PhononImageView()
{
    delete frameDevice;
}

void PhononImageView::setFrame(const base::samples::frame::Frame &frame)
{
    std::cout << "PhononImageView: Got frame. Send to frame device." << std::endl;
    int state = mediaObject->state();
    printf("State of media object: %d\n", state);
    
    if(state == Phonon::ErrorState)
    {
        printf("Error type: %d\n", mediaObject->errorType());
        std::cout << mediaObject->errorString().toStdString() << std::endl;
    }
    frameDevice->setFrame(frame);
    go(); // TODO always necessary? maybe only when state not playing?
}

void PhononImageView::update2()
{
    QWidget::update();
}

void PhononImageView::go()
{
    mediaObject->play();
}
