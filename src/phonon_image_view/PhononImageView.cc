#include <qt4/phonon/mediaobject.h>

#include "PhononImageView.h"
#include "FrameDevice.h"

using namespace Phonon;

PhononImageView::PhononImageView(QWidget *parent)
    : QWidget(parent)
{
    resize(300,120);

    
    // Set up media graph
    mediaObject = new MediaObject(this);
    mediaObject->setCurrentSource(MediaSource(&frameDevice));
    
    videoWidget = new VideoWidget(this);
    createPath(mediaObject, videoWidget);
    
    
    // Appearence
    QVBoxLayout vbox(this);
    vbox.addWidget(new QLabel("Phonon Video Widget below:"));
    vbox.addWidget(videoWidget);
    vbox.setAlignment(Qt::AlignVCenter);
    
    mediaObject->play();
}

PhononImageView::~PhononImageView()
{
}

void PhononImageView::setFrame(base::samples::frame::Frame *frame)
{
    frameDevice.setFrame(frame);
}

