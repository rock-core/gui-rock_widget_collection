# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(plotting_widget_MOC_HDRS
  PlottingWidget.h PlotWidget.h OptionsDialog.h ColorPickerButton.h GeneralOptionsWidget.h BorderLineOptionDialog.h CurveOptionWidget.h CurveSelectionDialog.h
)
QT4_WRAP_CPP(plotting_widget_MOC_SRCS ${plotting_widget_MOC_HDRS})


# For a large number of source files:
# using file() function:
file(GLOB plotting_widget_SOURCES *.cc)
#file(GLOB image_view_widget_SOURCES main.cc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")


# Create an executable file called helloworld from sources:
add_library(${PROJECT_NAME} SHARED ${plotting_widget_SOURCES} ${plotting_widget_MOC_SRCS})
add_executable(${PROJECT_NAME}_EXEC main.cc)


target_link_libraries(${PROJECT_NAME} ${QWT_LIBRARIES} ${QT_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_EXEC ${PROJECT_NAME} ${QWT_LIBRARIES} ${QT_LIBRARIES})


install(FILES libplotting_widget.so.rb DESTINATION lib)
install(TARGETS ${PROJECT_NAME} 
		LIBRARY DESTINATION lib
    )
