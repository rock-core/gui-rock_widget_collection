
find_package(Qt4 REQUIRED QtCore QtGui QtOpengl QtDesigner)
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
  vtk/sonar_display/SonarDisplay.h
  vtk/sonar_display/SonarDisplayPlugin.h
  vtk/plot2d/Plot2d.h
  vtk/plot2d/Plot2dPlugin.h
  2dvis/qwaterfalldisplay.h
  2dvis/WaterwallDisplayPlugin.h
  timeline/Timeline.h
  timeline/TestWidget.h
  timeline/Slider.h
  timeline/BoundarySlider.h
  timeline/IndexSlider.h
  timeline/SlideBarItem.h
  timeline/TimelinePlugin.h
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
  vtk/sonar_display/SonarDisplay.h
  vtk/plot2d/Plot2d.h
  2dvis/qwaterfalldisplay.h
  generic_widgets/RockSliderPlugin.h
  timeline/Timeline.h
  timeline/TestWidget.h
  timeline/Slider.h
  timeline/BoundarySlider.h
  timeline/IndexSlider.h
  timeline/SlideBarItem.h
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
    vtk/sonar_display/*.cc
    vtk/plot2d/*.cc
    2dvis/*.cc
    timeline/*.cc
    *.cc)

SET(QtApp_RCCS resources.qrc)
QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})

rock_vizkit_widget(rock_widget_collection
    SOURCES ${COLLECTION_SOURCES} ${QtApp_RCC_SRCS} 
    HEADERS ${COLLECTION_HDRS}
    MOC ${COLLECTION_MOC_HDRS}
    DEPS_PKGCONFIG base-types frame_helper QtCore QtGui
    DEPS_CMAKE LIBXML2 QWT GLU VTK
    LIBS ${QT_QTCORE_LIBRARY} vtkVolumeRendering vtkRendering vtkCommon vtkFiltering vtkIO vtkGraphics QVTK ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${QT_QTDESIGNER_LIBRARY}
    )

add_executable(sonarTest
    sonar_view/sonarTest.cpp
)
add_executable(timelineTest
    timeline/main.cpp
)

target_link_libraries(sonarTest rock_widget_collection)
target_link_libraries(timelineTest rock_widget_collection)

rock_executable(virtualJoystickTest 
		virtual_joystick/main.cpp
		DEPS rock_widget_collection)
