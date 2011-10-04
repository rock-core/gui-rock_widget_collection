/* 
 * File:   SonarDisplay.h
 * Author: blueck
 *
 * Created on 17. Juni 2010, 14:14
 */

#ifndef SONARDISPLAY_H
#define	SONARDISPLAY_H

#include <MultiWidget.h>
#include <QtGui/QWidget>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QPen>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtGui/qfiledialog.h>
#include <QtGui/QMenu>
#include <QtCore/QString>
#include <QtDesigner/QDesignerExportWidget>

#include <QVTKWidget.h>
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <vtkStructuredGrid.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkUnsignedCharArray.h>

#include <iostream>
#include <vector>


class QDESIGNER_WIDGET_EXPORT SonarDisplay : public QVTKWidget 
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Alexander Duda")

public:
    SonarDisplay(QWidget *parent = NULL);
    virtual ~SonarDisplay(){};

    vtkSmartPointer<vtkStructuredGrid> generateStructuredGrid(int number_of_beams, int number_of_bins ,
                               float horizontal_resolution, float distance_resolution,
                               float vertical_resolution = 0); 

void setUpStructuredGrid(vtkSmartPointer<vtkStructuredGrid> sgrid, 
                         int number_of_beams, int number_of_bins,
                         float horizontal_resolution, float distance_resolution,
                         float vertical_resolution, float start_angle = 0);

public slots:
    void addSonarBeam(float bearing,int number_of_bins,const char* pbuffer);
    void setUpSonar(int number_of_beams, int number_of_bins,float horizontal_resolution,
                    float distance_resolution, float vertical_resolution);
    void setParameterToAuto(bool value);
    void deleteBeams(int from, int to);

private:
    int number_of_beams;
    int number_of_bins;
    float start_bearing;
    float end_bearing;
    float horizontal_resolution;
    float vertical_resolution;
    float distance_resolution;
    bool paramerter_auto;
    bool smooth;
    int last_index;
    unsigned long number_of_writes;
    bool scan_right;
    int left_limit;
    int right_limit;


    vtkSmartPointer<vtkStructuredGrid> sonar_wireframe;
    vtkSmartPointer<vtkRenderWindow> render_window; 
    std::vector<vtkSmartPointer<vtkStructuredGrid> > sonar_grid;
    std::vector<vtkSmartPointer<vtkUnsignedCharArray> > sonar_data;

protected:
    int normalizeDelata(int delta);
};

#endif	/* IMAGEVIEWWIDGET_H */

