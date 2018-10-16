cmake_minimum_required(VERSION 2.8.3)
project(soccer_vision)

find_package(catkin REQUIRED COMPONENTS roscpp tf image_transport genmsg field_model config_server ball_perceptor)

add_message_files(
  FILES
  Detections.msg
  LineDetection.msg
  ObjectDetection.msg
  ObstacleDetection.msg
)

generate_messages(
	DEPENDENCIES std_msgs geometry_msgs
)

catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

find_package(GTK2)
if(GTK2_FOUND)
	include_directories(${GTK2_INCLUDE_DIRS})
	add_definitions(-DHAVE_GTK)
endif()


find_package(Eigen3 REQUIRED)
include_directories(${Eigen3_INCLUDE_DIRS})


find_package(OpenCV REQUIRED)


add_executable(soccercv_gui src/soccercv_gui.cpp)
target_link_libraries(soccercv_gui ${catkin_LIBRARIES} ${GTK2_LIBRARIES} ${OpenCV_LIBS})

add_library(soccer_vision src/soccer_vision.cpp src/yuvClasses.cpp src/convexhullfunctions.cpp src/pixelCameraCorrection.cpp  src/camera_parameters.cpp  src/lsd.c
			src/findField.cpp  src/findBall.cpp src/findGoal.cpp src/findObstacles.cpp src/findLandmarksAndLines.cpp)
			
target_link_libraries(soccer_vision ${catkin_LIBRARIES} ${OpenCV_LIBS})

add_dependencies(soccer_vision soccer_vision_gencpp)

install(TARGETS soccer_vision soccercv_gui
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(FILES Black.lut Objects.lut White.lut maskForCamera plugin.xml
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

## Copy launch files
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  FILES_MATCHING PATTERN "*.launch"
)
