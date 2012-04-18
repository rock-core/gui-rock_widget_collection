#ifndef PHONONIMAGEVIEW_H
#define PHONONIMAGEVIEW_H

#include <QtGui>
#include <Phonon/VideoWidget>
#include <Phonon/MediaObject>
#include <Phonon/MediaSource>

#include <frame_helper/FrameQImageConverter.h>
#include "FrameDevice.h"

class PhononImageView : public QWidget
{
    Q_OBJECT
public:
    PhononImageView(QWidget *parent = 0);
    virtual ~PhononImageView();
    
public slots:
    void setFrame(base::samples::frame::Frame *frame);
    
private:
    Phonon::MediaObject *mediaObject;
    Phonon::VideoWidget *videoWidget;
    FrameDevice frameDevice;
    
};

#endif /* PHONONIMAGEVIEW_H */
