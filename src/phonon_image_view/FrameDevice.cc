#include "FrameDevice.h"

FrameDevice::FrameDevice()
{
    
}

FrameDevice::~FrameDevice()
{
    
}

void FrameDevice::setFrame(base::samples::frame::Frame *frame)
{
    this->frame = frame;
    emit readyRead();
}


qint64 FrameDevice::bytesAvailable() const
{
    return 4096 + QIODevice::bytesAvailable(); // TODO use correct buffer size instead of 4096!
}

bool FrameDevice::isSequential() const
{
    return true;
}

qint64 FrameDevice::readData(char *data, qint64 maxSize)
{
    return -1;
}

qint64 FrameDevice::writeData(const char *data, qint64 len)
{    
    return -1;
}

