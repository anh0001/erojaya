cmake_minimum_required(VERSION 2.8.3)
project(nimbro_op_gazebo)

find_package(catkin REQUIRED COMPONENTS urdf)

catkin_package()

install(DIRECTORY launch
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
	FILES_MATCHING PATTERN "*.launch"
)

install(DIRECTORY worlds
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
