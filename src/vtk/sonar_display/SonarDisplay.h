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
#include <vtkUnsignedCharArray.h>
#include <vtkStructuredGrid.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkXYPlotActor.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkCommand.h>

#include <iostream>
#include <vector>
class SonarDisplay;

class KeyEvent : public vtkCommand
{
    public:
        KeyEvent(SonarDisplay* p){p_sonar_display = p;bin_number=100;};
        virtual void Execute (vtkObject *caller, unsigned long eventId, void *callData);

    private:
        SonarDisplay *p_sonar_display;
        int bin_number;
};

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

        int getActorID(const vtkActor *actor);
        vtkStructuredGridGeometryFilter* getFilter(int id);
        vtkUnsignedCharArray* getData(int id);

        public slots:
            void addSonarBeam(float bearing,int number_of_bins,const char* pbuffer);
        void setUpSonar(int number_of_beams, int number_of_bins,float horizontal_resolution,
                float distance_resolution, float vertical_resolution);
        void setParameterToAuto(bool value);
        void deleteBeams(int from, int to);

        int getNumberOfBins(){return number_of_bins;};
        int getNumberOfBeams(){return number_of_beams;};
        void setPlotData(int id);
        void reset();

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

        KeyEvent key_event;

        vtkSmartPointer<vtkStructuredGrid> sonar_wireframe;
        vtkSmartPointer<vtkRenderWindow> render_window; 
        vtkSmartPointer<vtkRenderer> renderer; 


        std::vector<vtkSmartPointer<vtkUnsignedCharArray> > sonar_data;
        std::vector<vtkSmartPointer<vtkStructuredGrid> > sonar_grid;
        std::vector<vtkSmartPointer<vtkStructuredGridGeometryFilter> > sonar_filter;
        std::vector<vtkSmartPointer<vtkPolyDataMapper> > sonar_mapper;
        std::vector<vtkSmartPointer<vtkActor> > sonar_actor;

        vtkSmartPointer<vtkXYPlotActor> plot_actor;

    protected:
        int normalizeDelata(int delta);
};

#endif	/* IMAGEVIEWWIDGET_H */

