cmake_minimum_required(VERSION 2.8.3)
project(loc_display)

find_package(catkin REQUIRED COMPONENTS roscpp rviz field_model cmake_modules particle_filter)

#find_package(Qt4 REQUIRED)
#include(${QT_USE_FILE})
#add_definitions(-DQT_NO_KEYWORDS)

find_package(Qt5Widgets REQUIRED)


#/usr/share/OGRE/cmake/modules/FindOGRE.cmake
if(EXISTS "/usr/share/OGRE/cmake/modules/")
	set(CMAKE_MODULE_PATH "/usr/share/OGRE/cmake/modules/;${CMAKE_MODULE_PATH}")
#	set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local

elseif(EXISTS "/usr/lib/OGRE/cmake")

	set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
#	set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

else ()
	message(SEND_ERROR "Failed to find OGRE module path.")
endif(EXISTS "/usr/share/OGRE/cmake/modules/")
find_package(OGRE REQUIRED)



catkin_package()

include_directories(include
	${catkin_INCLUDE_DIRS}
	${OGRE_INCLUDE_DIRS}
)

add_library(loc_display
	src/loc_display.cpp
)

target_link_libraries(loc_display
	${catkin_LIBRARIES}
	#${QT_LIBRARIES}
	${OGRE_LIBRARIES}
)
qt5_use_modules(loc_display Widgets)

install(TARGETS loc_display
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(FILES plugin.xml
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY field/
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)


