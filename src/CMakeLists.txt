
find_package(Qt4 REQUIRED QtCore QtGui QtOpengl QtDesigner)
include_directories(multi_view/)
include_directories(${QT_INCLUDE_DIR})
include_directories(${QT_QTCORE_INCLUDE_DIR})
include_directories(${QT_QTGUI_INCLUDE_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(COLLECTION_MOC_HDRS
  image_view/ImageView.h
  image_view/ImageViewGL.h
  image_view/ImageViewPlugin.h
  image_view/DrawItem.h
  image_view/RectangleItem.h
  image_view/FillItem.h
  image_view/LineItem.h
  image_view/TextItem.h
  image_view/PointItem.h
  image_view/EllipseItem.h
  image_view/PolygonItem.h
  image_view/PolylineItem.h
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
)
SET(COLLECTION_HDRS
  image_view/ImageView.h
  image_view/ImageViewGL.h
  image_view/ImageViewPlugin.h
  image_view/DrawItem.h
  image_view/LineItem.h
  image_view/FillItem.h
  image_view/TextItem.h
  image_view/PointItem.h
  image_view/EllipseItem.h
  image_view/RectangleItem.h
  image_view/PolygonItem.h
  image_view/PolylineItem.h
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
  multi_view/MultiWidgetPlugin.h
  multi_view/MultiWidget.h
)

FILE(GLOB COLLECTION_SOURCES range_view/*.cc image_view/*.cc sonar_view/*.cc plotting/*.cc artificial_horizon/*.cc multi_view/*.cc *.cc)
SET(QtApp_RCCS resources.qrc)
QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})

rock_vizkit_widget(rock_widget_collection
    SOURCES ${COLLECTION_SOURCES} ${QtApp_RCC_SRCS} 
    HEADERS ${COLLECTION_HDRS}
    MOC ${COLLECTION_MOC_HDRS}
    DEPS_PKGCONFIG base-types frame_helper
    DEPS_CMAKE LIBXML2 QWT GLU
    LIBS ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${QT_QTDESIGNER_LIBRARY}
    )
