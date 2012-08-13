
find_package(Qt4 REQUIRED QtCore QtGui QtOpengl QtDesigner )

#include(/usr/local/lib/QtGStreamer/QtGStreamerConfig.cmake)
find_package(QtGStreamer PATHS /usr/local/lib/QtGStreamer NO_DEFAULT_PATH)
include_directories(/usr/local/include/QtGStreamer)
#include_directories(${QTGSTREAMER_INCLUDES})

include_directories(multi_view/ generic_widgets/)
include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR})
include_directories(${QT_QTGUI_INCLUDE_DIR})


set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(COLLECTION_MOC_HDRS
  image_view/ImageView.h
  image_view/ImageViewGL.h
  image_view/ImageViewPlugin.h
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
  plotting/PlottingWidget.h 
  plotting/PlotWidget.h 
  plotting/OptionsDialog.h 
  plotting/ColorPickerButton.h 
  plotting/GeneralOptionsWidget.h
  plotting/BorderLineOptionDialog.h 
  plotting/CurveOptionWidget.h
  plotting/CurveSelectionDialog.h 
  plotting/PlotWidgetPlugin.h 
  plotting/AxisOptionsWidget.h
  artificial_horizon/artificialhorizon.h
  artificial_horizon/artificialhorizonplugin.h
  multi_view/MultiViewWidget.h
  multi_view/WidgetButton.h
  multi_view/MultiViewPlugin.h
  multi_view/MultiWidget.h
  multi_view/MultiWidgetPlugin.h
  multi_view/WidgetButtonPlugin.h
  artificial_horizon/compass.h
  artificial_horizon/compassplugin.h
  artificial_horizon/orientation.h
  artificial_horizon/orientationplugin.h
  virtual_joystick/VirtualJoystick.h  
  virtual_joystick/VirtualJoystickPlugin.h
  generic_widgets/PaintWidget.h
  generic_widgets/RockSlider.h
  generic_widgets/RockSliderPlugin.h
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
  gst_image_view/GstImageView.h
  gst_image_view/GstImageViewPlugin.h
  gst_image_view/ContextMenuGraphicsWidget.h
  progress_indicator/ProgressIndicator.h
  progress_indicator/ProgressIndicatorPlugin.h
)

SET(COLLECTION_HDRS
  image_view/ImageView.h
  image_view/ImageViewGL.h
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
  plotting/PlottingWidget.h 
  plotting/PlotWidget.h 
  plotting/OptionsDialog.h 
  plotting/ColorPickerButton.h 
  plotting/GeneralOptionsWidget.h
  plotting/BorderLineOptionDialog.h 
  plotting/CurveOptionWidget.h
  plotting/CurveSelectionDialog.h 
  plotting/AxisOptionsWidget.h
  artificial_horizon/artificialhorizon.h
  multi_view/MultiViewWidget.h
  multi_view/WidgetButton.h
  multi_view/MultiWidget.h
  artificial_horizon/compass.h
  artificial_horizon/orientation.h
  virtual_joystick/VirtualJoystick.h  
  generic_widgets/PaintWidget.h
  generic_widgets/RockSlider.h
  plot2d/Plot2d.h
  plot2d/qcustomplot.h
  2dvis/qwaterfalldisplay.h
  generic_widgets/RockSliderPlugin.h
  timeline/Timeline.h
  timeline/TestWidget.h
  timeline/Slider.h
  timeline/BoundarySlider.h
  timeline/IndexSlider.h
  timeline/SlideBarItem.h
  gst_image_view/GstImageView.h
  gst_image_view/ContextMenuGraphicsWidget.h
#   gst_image_view/FrameAppSrc.h
  progress_indicator/ProgressIndicator.h
)

FILE(GLOB COLLECTION_SOURCES 
    range_view/*.cc 
    image_view/*.cc 
    sonar_view/*.cc 
    plotting/*.cc 
    artificial_horizon/*.cc 
    multi_view/*.cc 
    generic_widgets/*.cc
    virtual_joystick/*.cc
    plot2d/*.cc
    2dvis/*.cc
    timeline/*.cc
    gst_image_view/*.cc
    progress_indicator/*.cc
    *.cc)

option(USE_VTK "Use VTK for additional Widgets" FALSE)
option(USE_GST "Use GStreamer in GstImageView" OFF)

## If you want to use GStreamer, uncomment the following
#set(USE_GST ON)

if (USE_GST)
    add_definitions(-DUSE_GST)
endif()

if (USE_VTK)
    find_package(VTK)
    list(APPEND COLLECTION_MOC_HDRS
        vtk/sonar_display/SonarDisplay.h
        vtk/sonar_display/SonarDisplayPlugin.h
    )
    list(APPEND COLLECTION_HDRS
        vtk/sonar_display/SonarDisplay.h
    )
    list(APPEND COLLECTION_SOURCES
      vtk/sonar_display/SonarDisplay.cc vtk/sonar_display/SonarDisplayPlugin.cc
    )
    set(VTK_DEPS VTK)
    set(VTK_LIBS vtkVolumeRendering vtkRendering vtkCommon vtkFiltering vtkIO vtkGraphics QVTK )
    add_definitions(-DUSE_VTK)
endif()

SET(QtApp_RCCS resources.qrc)
QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})

rock_vizkit_widget(rock_widget_collection
    SOURCES ${COLLECTION_SOURCES} ${QtApp_RCC_SRCS} 
    HEADERS ${COLLECTION_HDRS}
    MOC ${COLLECTION_MOC_HDRS}
    DEPS_PKGCONFIG base-types base-lib frame_helper QtCore QtGui
    DEPS_CMAKE LIBXML2 QWT GLU ${VTK_DEPS}
    LIBS ${QT_QTCORE_LIBRARY} ${VTK_LIBS} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${QT_QTDESIGNER_LIBRARY} ${QTGSTREAMER_LIBRARIES} ${QTGSTREAMER_UI_LIBRARIES} 
    )


add_subdirectory(stream_aligner_widget)

if (USE_VTK)
    rock_executable(sonarTest sonar_view/sonarTest.cpp
        DEPS rock_widget_collection
        NOINSTALL)
endif()

rock_executable(timelineTest timeline/main.cpp
    DEPS rock_widget_collection
    NOINSTALL)

rock_executable(gstimageviewTest gst_image_view/main.cpp
    DEPS rock_widget_collection
    NOINSTALL)

rock_executable(virtualJoystickTest 
    virtual_joystick/main.cpp
    DEPS rock_widget_collection
    NOINSTALL)

