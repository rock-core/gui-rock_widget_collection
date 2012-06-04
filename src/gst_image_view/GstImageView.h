#ifndef GSTIMAGEVIEW_H
#define GSTIMAGEVIEW_H

#include <QtGui>

#include <base/samples/frame.h>
#include <frame_helper/FrameQImageConverter.h>

class GstImageView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString pipelineDescription READ getPipelineDescription WRITE setPipelineDescription)
    
public:
    GstImageView(QWidget *parent = 0);
    virtual ~GstImageView();
    
public slots:
    QString getPipelineDescription();
    void setPipelineDescription(QString descr);
    
    void setFrame(const base::samples::frame::Frame &frame);
    void update2();
    
private:
    QString pipelineDescription;
    base::samples::frame::Frame *frame;
};

#endif /* GSTIMAGEVIEW_H */
