
rock_find_qt5(OPTIONAL Widgets PrintSupport OpenGL Gui Designer UiPlugin)
rock_find_qt4(OPTIONAL QtCore QtGui QtOpengl QtDesigner)

option(USE_VTK "Use VTK for additional Widgets" FALSE)
option(USE_GST "Use GStreamer in GstImageView" OFF)

## If you want to use GStreamer, uncomment the following
#set(USE_GST ON)

if (USE_GST)
    # TODO Using GStreamer requires some reconstruction and currently 
    #      the installation of a self compiled library (QtGStreamer 0.10.2).
    #      Using this is currently not recommended!
    
    #include(/usr/local/lib/QtGStreamer/QtGStreamerConfig.cmake)
    find_package(QtGStreamer PATHS /usr/local/lib/QtGStreamer NO_DEFAULT_PATH)
    include_directories(/usr/local/include/QtGStreamer)
    #include_directories(${QTGSTREAMER_INCLUDES})
    
    add_definitions(-DUSE_GST)
endif()


include_directories(multi_view/ generic_widgets/)
#include_directories(${QT_INCLUDE_DIR})
#include_directories(${QT_QTCORE_INCLUDE_DIR})
#include_directories(${QT_QTGUI_INCLUDE_DIR})


set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(COLLECTION_MOC_HDRS
  image_view_old/ImageViewOld.h
  image_view_old/ImageViewOldGL.h
  image_view_old/ImageViewOldPlugin.h
  generic_widgets/DrawItem.h
  generic_widgets/RectangleItem.h
  generic_widgets/FillItem.h
  generic_widgets/LineItem.h
  generic_widgets/TextItem.h
  generic_widgets/PointItem.h
  generic_widgets/EllipseItem.h
  generic_widgets/PolygonItem.h
  generic_widgets/PolylineItem.h
  sonar_view/SonarView.h
  sonar_view/SonarViewGL.h
  sonar_view/SonarViewPlugin.h
  range_view/RangeView.h
  range_view/RangeViewGL.h
  range_view/RangeViewPlugin.h
  RockWidgetCollection.h
  multi_view/MultiViewWidget.h
  multi_view/WidgetButton.h
  multi_view/MultiViewPlugin.h
  multi_view/MultiWidget.h
  multi_view/MultiWidgetPlugin.h
  multi_view/WidgetButtonPlugin.h
  virtual_joystick/VirtualJoystick.h  
  virtual_joystick/VirtualJoystickPlugin.h
  generic_widgets/PaintWidget.h
  generic_widgets/RockSlider.h
  generic_widgets/RockSliderPlugin.h
  artificial_horizon/artificialhorizon.h
  artificial_horizon/artificialhorizonplugin.h
  plot2d/Plot2d.h
  plot2d/qcustomplot.h
  plot2d/Plot2dPlugin.h
  2dvis/qwaterfalldisplay.h
  2dvis/WaterwallDisplayPlugin.h
  timeline/Timeline.h
  timeline/TestWidget.h
  timeline/TimelinePlugin.h
  timeline/Slider.h
  timeline/SlideBarItem.h
  image_view/ImageView.h
  image_view/ImageViewPlugin.h
  image_view/ContextMenuGraphicsWidget.h
  progress_indicator/ProgressIndicator.h
  progress_indicator/ProgressIndicatorPlugin.h
  sonar_widget/SonarPlot.h
  sonar_widget/SonarWidget.h
  sonar_widget/SonarWidgetPlugin.h
)

SET(COLLECTION_HDRS
  image_view_old/ImageViewOld.h
  image_view_old/ImageViewOldGL.h
  generic_widgets/DrawItem.h
  generic_widgets/LineItem.h
  generic_widgets/FillItem.h
  generic_widgets/TextItem.h
  generic_widgets/PointItem.h
  generic_widgets/EllipseItem.h
  generic_widgets/RectangleItem.h
  generic_widgets/PolygonItem.h
  generic_widgets/PolylineItem.h
  sonar_view/SonarView.h
  sonar_view/SonarViewGL.h
  range_view/RangeView.h
  range_view/RangeViewGL.h
  RockWidgetCollection.h
  multi_view/MultiViewWidget.h
  multi_view/WidgetButton.h
  multi_view/MultiWidget.h
  virtual_joystick/VirtualJoystick.h  
  generic_widgets/PaintWidget.h
  generic_widgets/RockSlider.h
  plot2d/Plot2d.h
  plot2d/qcustomplot.h
  artificial_horizon/artificialhorizon.h
  2dvis/qwaterfalldisplay.h
  generic_widgets/RockSliderPlugin.h
  timeline/Timeline.h
  timeline/TestWidget.h
  timeline/Slider.h
  timeline/BoundarySlider.h
  timeline/IndexSlider.h
  timeline/SlideBarItem.h
  image_view/ImageView.h
  progress_indicator/ProgressIndicator.h
  sonar_widget/SonarPlot.h
  sonar_widget/SonarWidget.h
  sonar_widget/SonarWidgetPlugin.h
)

FILE(GLOB COLLECTION_SOURCES 
    range_view/*.cc 
    image_view_old/*.cc 
    sonar_view/*.cc 
    multi_view/*.cc 
    generic_widgets/*.cc
    virtual_joystick/*.cc
    plot2d/*.cc
    2dvis/*.cc
    timeline/*.cc
    image_view/*.cc
    progress_indicator/*.cc
    sonar_widget/*.cc
    *.cc)

list(APPEND COLLECTION_SOURCES
    artificial_horizon/artificialhorizon.cc
    artificial_horizon/artificialhorizonplugin.cc)
    

if (USE_VTK)
    find_package(VTK)
endif()

SET(QtApp_RCCS resources.qrc)
if(ROCK_QT_VERSION_5)
    QT5_ADD_RESOURCES(Qt5App_RCC_SRCS ${QtApp_RCCS})

    find_package(QWT)

    set(QT5_EXTRA_DEPS ${EXTRA_DEPS})
    set(QT5_EXTRA_LIBS)
    set(QT5_COLLECTION_SOURCES ${COLLECTION_SOURCES})
    set(QT5_COLLECTION_MOC_HDRS ${COLLECTION_MOC_HDRS})
    set(QT5_COLLECTION_HDRS ${COLLECTION_HDRS})

    if (QWT_FOUND)
        list(APPEND QT5_EXTRA_DEPS QWT)
        list(APPEND QT5_COLLECTION_SOURCES
            artificial_horizon/compass.cc
            artificial_horizon/compassplugin.cc
            artificial_horizon/orientation.cc
            artificial_horizon/orientationplugin.cc)
        list(APPEND QT5_COLLECTION_MOC_HDRS
            artificial_horizon/compass.h
            artificial_horizon/compassplugin.h
            artificial_horizon/orientation.h
            artificial_horizon/orientationplugin.h)
        list(APPEND QT5_COLLECTION_HDRS
            artificial_horizon/orientation.h
            artificial_horizon/compass.h)
    endif()

    if (VTK_FOUND AND VTK_QT_VERSION EQUAL 5)
        list(APPEND QT5_EXTRA_DEPS VTK)
        list(APPEND QT5_EXTRA_LIBS)
        list(APPEND QT5_COLLECTION_MOC_HDRS
            vtk/sonar_display/SonarDisplay.h
            vtk/sonar_display/SonarDisplayPlugin.h
            vtk/vectorfield3D/vectorfield3D.h
            vtk/vectorfield3D/vectorfield3DPlugin.h
        )
        list(APPEND QT5_COLLECTION_HDRS
            vtk/sonar_display/SonarDisplay.h
            vtk/vectorfield3D/vectorfield3D.h
        )
        list(APPEND QT5_COLLECTION_SOURCES
            vtk/sonar_display/SonarDisplay.cc
            vtk/sonar_display/SonarDisplayPlugin.cc
            vtk/vectorfield3D/vectorfield3D.cc
            vtk/vectorfield3D/vectorfield3DPlugin.cc
        )
    endif()

    rock_vizkit_widget(rock_widget_collection-qt5 SHARED
        SOURCES ${QT5_COLLECTION_SOURCES} ${Qt5App_RCC_SRCS}
        HEADERS ${QT5_COLLECTION_HDRS}
        MOC5 ${QT5_COLLECTION_MOC_HDRS}
        DEPS_PKGCONFIG base-types base-lib frame_helper
        DEPS_CMAKE ${VTK_DEPS} ${QT5_EXTRA_DEPS}
        LIBS ${QT5_EXTRA_LIBS} ${VTK_LIBS} GLU Qt5::Widgets Qt5::PrintSupport Qt5::OpenGL Qt5::Designer Qt5::UiPlugin Qt5::Gui
        )

    if (QWT_FOUND)
        target_compile_definitions(rock_widget_collection-qt5 PRIVATE -DUSE_QWT)
    endif()
    if (VTK_FOUND AND VTK_QT_VERSION EQUAL 5)
        target_compile_definitions(rock_widget_collection-qt5 PRIVATE -DUSE_VTK)
    endif()
endif()

if(ROCK_QT_VERSION_4)
    QT4_ADD_RESOURCES(Qt4App_RCC_SRCS ${QtApp_RCCS})

    find_package(QWT_QT4)

    set(QT4_EXTRA_DEPS ${EXTRA_DEPS})
    set(QT4_EXTRA_LIBS)
    set(QT4_COLLECTION_SOURCES ${COLLECTION_SOURCES})
    set(QT4_COLLECTION_MOC_HDRS ${COLLECTION_MOC_HDRS})
    set(QT4_COLLECTION_HDRS ${COLLECTION_HDRS})

    if (QWT_QT4_FOUND)
        list(APPEND QT4_EXTRA_DEPS QWT_QT4)
        list(APPEND QT4_COLLECTION_SOURCES
            artificial_horizon/compass.cc
            artificial_horizon/compassplugin.cc
            artificial_horizon/orientation.cc
            artificial_horizon/orientationplugin.cc)
        list(APPEND QT4_COLLECTION_MOC_HDRS
            artificial_horizon/compass.h
            artificial_horizon/compassplugin.h
            artificial_horizon/orientation.h
            artificial_horizon/orientationplugin.h)
        list(APPEND QT4_COLLECTION_HDRS
            artificial_horizon/orientation.h
            artificial_horizon/compass.h)
    endif()

    if (VTK_FOUND AND VTK_QT_VERSION EQUAL 4)
        list(APPEND QT4_EXTRA_DEPS VTK)
        list(APPEND QT4_EXTRA_LIBS vtkftgl vtkVolumeRendering vtkRendering vtkCommon vtkFiltering vtkIO vtkGraphics QVTK)
        list(APPEND QT4_COLLECTION_MOC_HDRS
            vtk/sonar_display/SonarDisplay.h
            vtk/sonar_display/SonarDisplayPlugin.h
            vtk/vectorfield3D/vectorfield3D.h
            vtk/vectorfield3D/vectorfield3DPlugin.h
        )
        list(APPEND QT4_COLLECTION_HDRS
            vtk/sonar_display/SonarDisplay.h
            vtk/vectorfield3D/vectorfield3D.h
        )
        list(APPEND QT4_COLLECTION_SOURCES
            vtk/sonar_display/SonarDisplay.cc
            vtk/sonar_display/SonarDisplayPlugin.cc
            vtk/vectorfield3D/vectorfield3D.cc
            vtk/vectorfield3D/vectorfield3DPlugin.cc
        )
    endif()

    rock_vizkit_widget(rock_widget_collection SHARED
        SOURCES ${QT4_COLLECTION_SOURCES} ${Qt4App_RCC_SRCS}
        HEADERS ${QT4_COLLECTION_HDRS}
        MOC ${QT4_COLLECTION_MOC_HDRS}
        DEPS_PKGCONFIG base-types base-lib frame_helper
        DEPS_CMAKE ${VTK_DEPS} ${QT4_EXTRA_DEPS}
        LIBS ${QT4_EXTRA_LIBS} ${VTK_LIBS} GLU Qt4::QtCore Qt4::QtGui Qt4::QtOpenGL Qt4::QtDesigner
        )

    if (QWT_QT4_FOUND)
        target_compile_definitions(rock_widget_collection PRIVATE -DUSE_QWT)
    endif()
    if (VTK_FOUND AND VTK_QT_VERSION EQUAL 4)
        target_compile_definitions(rock_widget_collection PRIVATE -DUSE_VTK)
    endif()
endif()


add_subdirectory(stream_aligner_widget)

if (USE_VTK AND VTK_FOUND)
    if (VTK_QT_VERSION EQUAL 4)
        rock_executable(sonarTest sonar_view/sonarTest.cpp
            DEPS rock_widget_collection
            NOINSTALL)
    endif()
    if (VTK_QT_VERSION EQUAL 5)
        rock_executable(sonarTest sonar_view/sonarTest.cpp
            DEPS rock_widget_collection-qt5
            NOINSTALL)
    endif()
endif()

#rock_executable(timelineTest timeline/main.cpp
#    DEPS rock_widget_collection
#    NOINSTALL)

rock_executable(imageviewTest-qt5 image_view/main.cpp
    DEPS rock_widget_collection-qt5
    NOINSTALL)

#rock_executable(imageviewTest image_view/main.cpp
#    DEPS rock_widget_collection
#    NOINSTALL)
#

#rock_executable(virtualJoystickTest 
#    virtual_joystick/main.cpp
#    DEPS rock_widget_collection
#    NOINSTALL)
