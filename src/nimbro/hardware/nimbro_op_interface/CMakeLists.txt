cmake_minimum_required(VERSION 2.8.3)
project(nimbro_op_interface)

add_definitions(-std=c++11)

find_package(catkin REQUIRED COMPONENTS roscpp robotcontrol cm730 servomodel plot_msgs rot_conv rc_utils message_generation rrlogger std_msgs)

include_directories(include
	${catkin_INCLUDE_DIRS}
)

add_message_files(FILES
	Button.msg
	Buzzer.msg
	LEDCommand.msg
)
add_service_files(FILES
	AttEstCalib.srv
	CalibGyroAccNow.srv
	CalibGyroAccStop.srv
	CalibGyroReturn.srv
	CalibGyroStart.srv
	CalibGyroStop.srv
	ReadOffset.srv
)
generate_messages(DEPENDENCIES
	std_msgs
)

catkin_package()

add_library(nimbro_op_interface
	src/robotinterface.cpp
	src/dummyinterface.cpp
	src/erosinterface.cpp
	src/robotPKinterface.cpp
	src/dummyPKinterface.cpp
)
target_link_libraries(nimbro_op_interface
	${catkin_LIBRARIES}
)

add_dependencies(nimbro_op_interface
	nimbro_op_interface_gencpp
	rrlogger_gencpp
)

install(TARGETS nimbro_op_interface
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(FILES plugin_robotcontrol.xml
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
	DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
   	FILES_MATCHING PATTERN "*.h"
   	PATTERN ".svn" EXCLUDE
)
