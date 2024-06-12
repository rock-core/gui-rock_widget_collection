
#find_package(Qt4 REQUIRED QtCore QtGui QtOpengl QtDesigner )
rock_find_qt5(Core Gui Widgets UiPlugin PrintSupport OpenGL Designer REQUIRED)

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
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Gui_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})

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
    list(APPEND COLLECTION_MOC_HDRS
        vtk/sonar_display/SonarDisplay.h
        vtk/sonar_display/SonarDisplayPlugin.h
        vtk/vectorfield3D/vectorfield3D.h
        vtk/vectorfield3D/vectorfield3DPlugin.h
    )
    list(APPEND COLLECTION_HDRS
        vtk/sonar_display/SonarDisplay.h
        vtk/vectorfield3D/vectorfield3D.h
    )
    list(APPEND COLLECTION_SOURCES
      vtk/sonar_display/SonarDisplay.cc 
      vtk/sonar_display/SonarDisplayPlugin.cc
      vtk/vectorfield3D/vectorfield3D.cc
      vtk/vectorfield3D/vectorfield3DPlugin.cc
    )
    set(VTK_DEPS VTK)
    set(VTK_LIBS vtkftgl vtkVolumeRendering vtkRendering vtkCommon vtkFiltering vtkIO vtkGraphics QVTK )
    add_definitions(-DUSE_VTK)
endif()

SET(QtApp_RCCS resources.qrc)
QT5_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})

find_package(QWT)

if (QWT_FOUND)
    add_definitions(-DUSE_QWT)
    list(APPEND EXTRA_DEPS QWT)
    list(APPEND COLLECTION_SOURCES
        artificial_horizon/compass.cc
        artificial_horizon/compassplugin.cc
        artificial_horizon/orientation.cc
        artificial_horizon/orientationplugin.cc)
    list(APPEND COLLECTION_MOC_HDRS
        artificial_horizon/compass.h
        artificial_horizon/compassplugin.h
        artificial_horizon/orientation.h
        artificial_horizon/orientationplugin.h)
    list(APPEND COLLECTION_HDRS
        artificial_horizon/orientation.h
        artificial_horizon/compass.h)
endif()

rock_vizkit_widget(rock_widget_collection SHARED
    SOURCES ${COLLECTION_SOURCES} ${QtApp_RCC_SRCS}
    HEADERS ${COLLECTION_HDRS}
    MOC ${COLLECTION_MOC_HDRS}
    DEPS_PKGCONFIG base-types base-lib frame_helper
    DEPS_CMAKE ${VTK_DEPS} ${EXTRA_DEPS}
    LIBS ${VTK_LIBS} GLU Qt5::Widgets Qt5::PrintSupport Qt5::OpenGL Qt5::Designer Qt5::Gui
    )

add_subdirectory(stream_aligner_widget)

if (USE_VTK)
    rock_executable(sonarTest sonar_view/sonarTest.cpp
        DEPS rock_widget_collection
        NOINSTALL)
endif()

#rock_executable(timelineTest timeline/main.cpp
#    DEPS rock_widget_collection
#    NOINSTALL)
#
rock_executable(imageviewTest image_view/main.cpp
    DEPS rock_widget_collection
    NOINSTALL)

#rock_executable(virtualJoystickTest 
#    virtual_joystick/main.cpp
#    DEPS rock_widget_collection
#    NOINSTALL)
