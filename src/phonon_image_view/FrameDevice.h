#ifndef FRAMEDEVICE_H
#define	FRAMEDEVICE_H

#include <QIODevice>

#include <frame_helper/FrameQImageConverter.h>

/**
 * This class provides a (read-only) interface between Qt and Rock's base::frame.
 */
class FrameDevice : public QIODevice {
public:
    FrameDevice();
    virtual ~FrameDevice();
    
    qint64 bytesAvailable() const;
    bool isSequential() const;
    void setFrame(const base::samples::frame::Frame &frame);
    
protected:
    qint64 readData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 len);
    
private:
    base::samples::frame::Frame frame;
    char *image_buffer;
    size_t image_bytesize;

};

#endif	/* FRAMEDEVICE_H */

