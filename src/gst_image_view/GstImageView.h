#ifndef GSTIMAGEVIEW_H
#define GSTIMAGEVIEW_H

#include <QtGui>

class GstImageView : public QWidget
{
    Q_OBJECT
public:
    GstImageView(QWidget *parent = 0);
    virtual ~GstImageView();
};

#endif /* GSTIMAGEVIEW_H */
