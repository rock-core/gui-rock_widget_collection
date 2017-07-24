#pragma once

#include <QtGui>
#include <QGLWidget>

#include <QQuaternion>

#include <Eigen/Geometry>

class QWidget;

class NavBallView : public QGLWidget
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Christoph Hertzberg")

public:
    NavBallView(QWidget *parent=NULL);
    //    virtual ~Orientation(); //Attention if you comment in this the widgets are not longer visible in designer WTF?
    void paintEvent(QPaintEvent *) {paintGL();}
    void resizeEvent ( QResizeEvent * event ) {QGLWidget::resizeEvent(event);}

public slots:

    NavBallView* newInstance() { return new NavBallView; } // TODO why?
    void setQuaternion(const QQuaternion& q);
    void update() const {}; //Nothing needs to be done but needs to be implemented for ruby bindings

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
private:
    void makeSphere();

    GLuint texture;
    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;
    std::vector<unsigned int> indices;

    Eigen::Quaternion<float, Eigen::Unaligned> quat, nwu2gl;
};

