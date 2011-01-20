# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(QtApp_RCCS resources.qrc)
SET(image_view_widget_MOC_HDRS
  image_view/ImageView.h
  image_view/ImageViewGL.h
  image_view/ImageViewPlugin.h
  image_view/DrawItem.h
  image_view/TextItem.h
  sonar_view/SonarView.h
  sonar_view/SonarViewGL.h
  sonar_view/SonarViewPlugin.h
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
)
QT4_WRAP_CPP(image_view_widget_MOC_SRCS ${image_view_widget_MOC_HDRS})
QT4_ADD_RESOURCES(QtApp_RCC_SRCS ${QtApp_RCCS})


# For a large number of source files:
# using file() function:
#file(GLOB image_view_widget_SOURCES *.cc)
#file(GLOB image_view_widget_SOURCES main.cc)

# Find all the input files
FILE(GLOB image_view_widget_SOURCES image_view/*.cc sonar_view/*.cc plotting/*.cc artificial_horizon/*.cc *.cc)

# Create an executable file called helloworld from sources:
add_library(${PROJECT_NAME} SHARED ${image_view_widget_SOURCES} ${image_view_widget_MOC_SRCS} ${QtApp_RCC_SRCS})
#add_executable(${PROJECT_NAME}_EXEC main.cc)

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${FRAMEHELPER_LIBRARIES} ${QWT_LIBRARIES} ${LIBXML2_LIBRARIES})
#target_link_libraries(${PROJECT_NAME}_EXEC ${PROJECT_NAME} ${QT_LIBRARIES})

INSTALL_TARGETS(/lib/qt/designer ${PROJECT_NAME})
#FILE(GLOB files artificial_horizon/resources/*.svg)
#INSTALL(FILES ${files} DESTINATION lib/qt/designer/resources/artificial_horizon)

