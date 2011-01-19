# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT4_WRAP_CPP(artificial_horizon_MOC_SRCS artificialhorizon.h)

add_library(${PROJECT_NAME}_widget SHARED artificialhorizon.cpp ${artificial_horizon_MOC_SRCS})
target_link_libraries(${PROJECT_NAME}_widget ${QT_LIBRARIES} ${FRAMEHELPER_LIBRARIES})
install(TARGETS ${PROJECT_NAME}_widget
		LIBRARY DESTINATION lib)
install(FILES libartificial_horizon.so.rb DESTINATION lib)

