cmake_minimum_required(VERSION 2.8.3)
project(bag_from_images)

add_definitions(-std=c++11)

find_package(catkin REQUIRED COMPONENTS roscpp std_msgs rosbag sensor_msgs cv_bridge image_transport)

find_package(OpenCV REQUIRED)

include_directories( 
   ${catkin_INCLUDE_DIRS}
   ${OpenCV_INCLUDE_DIRS}
)

catkin_package(
#   INCLUDE_DIRS include
#   LIBRARIES vision_module
#   CATKIN_DEPENDS pcl_conversions pcl_ros
)

add_executable(${PROJECT_NAME}
main.cc
Thirdparty/DLib/FileFunctions.cpp)

target_link_libraries (${PROJECT_NAME}
	${catkin_LIBRARIES}
   	${OpenCV_LIBS}
   	${OpenCV_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

#
install(DIRECTORY images/
   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/images/
)
