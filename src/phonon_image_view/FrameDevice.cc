#include "FrameDevice.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

FrameDevice::FrameDevice(QObject *parent) : QIODevice(parent)
{
    image_buffer = NULL;
    image_bytesize = 0;
}

FrameDevice::~FrameDevice()
{
    free(image_buffer);
}

void FrameDevice::setFrame(const base::samples::frame::Frame &frame)
{
    this->frame = frame;
    
    size_t bytes = frame.getNumberOfBytes();
    
    printf("Frame Device got frame:\n");
    printf("Frame width: %d, height: %d\n", frame.getWidth(), frame.getHeight());
    printf("Frame number_of_bytes: %zu\n", bytes);
    printf("Emitting 'readyRead'\n");
    
    
    
    if(bytes <= 0)
    {
        printf("Rejecting empty frame.\n");
        return;
    }
    
    // Image buffer big enough?
    if(image_bytesize < bytes) {
        // (Re-)Allocate image buffer
        printf("Trying to (re-)allocate image buffer of %zu bytes.\n", bytes);
        if(!(image_buffer = (char*) realloc((void*)image_buffer, bytes)))
        {
            perror("frame device setFrame() method: problem allocating image buffer");
            exit(EXIT_FAILURE);
        }
        printf("Done.\n");
        image_bytesize = bytes;
        
    }
    
    // Copy frame to image buffer
    // TODO should be optimized
    printf("Trying to copy frame to image buffer.\n");
    memcpy((void*)image_buffer, (void*)frame.getImageConstPtr(), bytes);
    printf("Done.\n");
    
    emit readyRead();
}


qint64 FrameDevice::bytesAvailable() const
{
    return image_bytesize + QIODevice::bytesAvailable();
}

bool FrameDevice::isSequential() const
{
    return true;
}

qint64 FrameDevice::readData(char *data, qint64 maxSize)
{
    // TODO analyze how this method gets called (which size, when, etc.)
    
    printf("FrameDevice::readData called. maxSize: %llu\n", maxSize);
    
    size_t copybytes = 0;
    if(maxSize > image_bytesize)
    {
        copybytes = image_bytesize;
    }
    else
    {
        copybytes = maxSize;
    }
    
    memcpy((void*)data, (void*) image_buffer, copybytes);
    
    printf("FrameDevice:: copied %zu bytes to buffer.\n", copybytes);
    return copybytes;
}

qint64 FrameDevice::writeData(const char *data, qint64 len)
{   
    printf("FrameDevice::writeData called. len: %llu\n", len);
    return -1;
}

