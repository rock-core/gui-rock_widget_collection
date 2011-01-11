# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.

cmake_minimum_required(VERSION 2.6)

project(plotting_widget)
set(PROJECT_VERSION 1.0)
set(PROJECT_DESCRIPTION "Widget for plotting graphs")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(CommonSetup)

set (CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${CMAKE_HOME_DIRECTORY}
)

# Include headers within source
# If you create subdirectories within source include headers with subdirectory, i.e. 
# #include "subdirectory/myheader.h"
#include_directories(${PROJECT_SOURCE_DIR}/external/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

include(FindPkgConfig)


#Need qwt as include
find_package(QWT REQUIRED)
include_directories(${QWT_INCLUDE_DIRS})
link_directories(${QWT_LIBRARY_DIRS})

find_package(LIBXML2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIRS})
link_directories(${LIBXML2_LIBRARY_DIRS})


# command line output on, you can also use make VERBOSE=1
# set(CMAKE_VERBOSE_MAKEFILE on)

### Uncomment if you want to use Qt

#set(QT_SEARCH_PATH /opt)
find_package(Qt4 REQUIRED QtCore QtGui, QtSvg, QtDesigner)

# Definitions to use when compiling code that uses Qt
add_definitions(${QT_DEFINITIONS})

# Set your desired Qt-Version here
include_directories(${QT_INCLUDE_DIR}
	${QT_QT_INCLUDE_DIR}
	${QT_QTCORE_INCLUDE_DIR}
	${QT_QTGUI_INCLUDE_DIR}
        ${QT_QTSVG_INCLUDE_DIR}
        ${QT_DESIGNER_INCLUDE_DIR})

# Path to a Cmake file that can be included to compile Qt4 applications and libraries
include(${QT_USE_FILE})


### Uncomment if you want to use Boost. Use the first find_package stanza to
### search for headers only, and the second one if you need binary libraries
#set(Boost_ADDITIONAL_VERSION 1.40 1.41)
#find_package(Boost REQUIRED)
#find_package(Boost REQUIRED COMPONENTS graph filesystem)
#include_directories(${Boost_INCLUDE_DIRS})

### Update external libraries that are available
#set(EXTERNAL_LIBS ${QT_LIBRARIES})

# Process CMakeLists.txt in the following subdirectory
add_subdirectory(src)

##### COPY Configuration files into build directory
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/configuration ${PROJECT_BINARY_DIR}/configuration)

# Workaround: Cleanup the in file from build directory
execute_process(COMMAND ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/configuration/${PROJECT_NAME}.pc.in)

##### INSTALL Configuration ####################################################

# include/
# TODO: recursive copy with directories
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/ DESTINATION include/${PROJECT_NAME}
	FILES_MATCHING PATTERN "*.h")

# configuration/<projectname>.pc
configure_file(${PROJECT_SOURCE_DIR}/configuration/${PROJECT_NAME}.pc.in
		${PROJECT_BINARY_DIR}/configuration/${PROJECT_NAME}.pc @ONLY)

# Install pkg-config file
install(FILES ${CMAKE_BINARY_DIR}/configuration/${PROJECT_NAME}.pc DESTINATION lib/pkgconfig)

# Dont' forget to add an install for your target, when you create your executable or library
   

##### End INSTALL Configuration ################################################

