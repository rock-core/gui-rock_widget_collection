# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 3.10)
project(rock_widget_collection
	VERSION 0.1
	DESCRIPTION "rock widget collection"
)

find_package(Rock)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

rock_init()
rock_feature(NOCURDIR)
rock_standard_layout()# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.

