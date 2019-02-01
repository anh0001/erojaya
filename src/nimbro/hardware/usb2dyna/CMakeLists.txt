cmake_minimum_required(VERSION 2.8.3)
project(usb2dyna)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy config_server plot_msgs)

###################################################
## Declare things to be passed to other projects ##
###################################################

## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
   INCLUDE_DIRS include
   LIBRARIES usb2dyna
   CATKIN_DEPENDS roscpp config_server
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
include_directories(include
  ${catkin_INCLUDE_DIRS}
)

## Build the usb2dyna library
add_library(usb2dyna
	src/CM730.cpp
	src/CM740mp.cpp
	src/read_thread.cpp
)
target_link_libraries(usb2dyna
	${catkin_LIBRARIES}
)

## Build the usb2dyna test node
#add_executable(usb2dyna_test
#	src/cm730test.cpp
#)
#target_link_libraries(usb2dyna_test
#	usb2dyna
#	${catkin_LIBRARIES}
#)

## Build the CM740 megapacket test node
#add_executable(cm740mp_test
#	src/cm740mptest.cpp
#)
#target_link_libraries(cm740mp_test
#	usb2dyna
#	${catkin_LIBRARIES}
#)

#############
## Install ##
#############

## Mark executables and/or libraries for installation
install(TARGETS usb2dyna
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
	DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
   	FILES_MATCHING PATTERN "*.h"
   	PATTERN ".svn" EXCLUDE
)
