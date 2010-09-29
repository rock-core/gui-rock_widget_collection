# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(plotting_widget_MOC_HDRS
  PlottingWidget.h PlotWidget.h
)
QT4_WRAP_CPP(plotting_widget_MOC_SRCS ${plotting_widget_MOC_HDRS})


# For a large number of source files:
# using file() function:
file(GLOB plotting_widget_SOURCES *.cc)
#file(GLOB image_view_widget_SOURCES main.cc)


# Create an executable file called helloworld from sources:
add_library(${PROJECT_NAME} SHARED ${plotting_widget_SOURCES} ${plotting_widget_MOC_SRCS})
add_executable(${PROJECT_NAME}_EXEC main.cc)


target_link_libraries(${PROJECT_NAME} QtCore QtGui qwt-qt4)
target_link_libraries(${PROJECT_NAME}_EXEC ${PROJECT_NAME} QtCore QtGui)


install(FILES libplotting_widget.so.rb DESTINATION lib)
install(TARGETS ${PROJECT_NAME} 
		LIBRARY DESTINATION lib
    )
