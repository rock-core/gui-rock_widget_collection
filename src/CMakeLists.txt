# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(image_view_widget_MOC_HDRS
  ImageViewWidget.h
  DrawItem.h
  TextItem.h
)
QT4_WRAP_CPP(image_view_widget_MOC_SRCS ${image_view_widget_MOC_HDRS})


# For a large number of source files:
# using file() function:
file(GLOB image_view_widget_SOURCES *.cc)
#file(GLOB image_view_widget_SOURCES main.cc)


# Create an executable file called helloworld from sources:
add_library(${PROJECT_NAME} SHARED ${image_view_widget_SOURCES} ${image_view_widget_MOC_SRCS})
add_executable(${PROJECT_NAME}_EXEC main.cc)


target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${FRAMEHELPER_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_EXEC ${PROJECT_NAME} ${QT_LIBRARIES})



install(TARGETS ${PROJECT_NAME} 
		LIBRARY DESTINATION lib
    )
