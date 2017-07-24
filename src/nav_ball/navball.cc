#include "navball.h"
#include <QHBoxLayout>
#include <iostream>

#include<GL/glut.h>


NavBallView::NavBallView(QWidget* parent) : QGLWidget(parent), texture(0)
{
    setMinimumWidth(100);
    setMinimumHeight(100);

    Eigen::Matrix3f NWU;
    NWU <<
            +0,+1,+0,
            +0,+0,+1,
            +1,+0,+0;
    nwu2gl = NWU.transpose();
//    std::cout << "NWU2GL: " << nwu2gl.coeffs().transpose() << '\n';
}

void NavBallView::initializeGL()
{
    makeSphere();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    // optional: (but light position should be fixed relative to camera not to object)
//    glShadeModel(GL_SMOOTH);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_MULTISAMPLE);
//    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void NavBallView::resizeGL(int width, int height)
{
    int side = std::min(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, +1, -1, +1, 4.0, 4.9);
    glMatrixMode(GL_MODELVIEW);
}

void NavBallView::paintGL()
{
    qglClearColor(Qt::black);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Eigen::Quaternionf res = quat * nwu2gl;
    res.x()*=-1.f; // invert tilt rotation
    Eigen::Transform<float, 3, Eigen::Affine, Eigen::ColMajor> trafo(res);
    trafo.translation() = Eigen::Vector3f::UnitZ() * -5.f;
    glLoadMatrixf(trafo.data());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glIndexPointer(GL_UNSIGNED_INT, 0, indices.data());
    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.constData());
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
}

void NavBallView::setQuaternion(const QQuaternion& q)
{
//    std::cout << "setQuaternion([" << q.x() << ' ' << q.y() << ' ' << q.z() << ", " << q.scalar() << "]\n";
    quat.w() = q.scalar();
    quat.x() = q.x();
    quat.y() = q.y();
    quat.z() = q.z();
    updateGL();
}


void NavBallView::makeSphere()
{
    QPixmap pixmap(QString(":/nav_ball/navball.png"));
//    std::cout << "Image: " << pixmap.size().height() << " x " << pixmap.size().width() << '\n';
    texture = bindTexture(pixmap, GL_TEXTURE_2D);
    const int meridians = 32, latitudes = 15;
    vertices.reserve((meridians+1) * (latitudes+2));
    texCoords.reserve((meridians+1) * (latitudes+2));

    for (int i = 0; i < meridians + 1; ++i) {
        for (int j = 0; j < latitudes + 2; ++j) {
            // texture coordinates in [0,1] x [0,1]:
            texCoords.append(QVector2D(double(i)/meridians, double(j)/(latitudes+1)));

            // theta = longitude from 0 to 2*pi
            // phi = latitude from -pi/2 to pi/2
            double theta = 2*M_PI * (texCoords.back().x());
            double phi = M_PI * (texCoords.back().y() - 0.5);
            vertices.append( QVector3D(
                    std::cos(phi) * std::cos(theta),
                    std::cos(phi) * std::sin(theta),
                    std::sin(phi)
            ));
        }
    }

    for (int i = 0; i < meridians; ++i)
    {
        // Construct triangles between successive meridians
        for (int j = 0; j < latitudes + 1; ++j)
        {
            indices.push_back((i+1) * (latitudes+2) + j+1);
            indices.push_back(i * (latitudes+2) + j+1);
            indices.push_back(i * (latitudes+2) + j);

            indices.push_back(i * (latitudes+2) + j);
            indices.push_back((i+1) * (latitudes+2) + j);
            indices.push_back((i+1) * (latitudes+2) + j+1);
        }
    }
//    std::cout << "Made sphere, vertices: " << vertices.size() << ", indices: " << indices.size() << '\n';
}

