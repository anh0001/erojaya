cmake_minimum_required(VERSION 2.8.3)
project(nodelet_gui_manager)

find_package(catkin REQUIRED COMPONENTS roscpp nodelet)

#find_package(Qt4 COMPONENTS QtCore QtGui)
#include(${QT_USE_FILE})
find_package(Qt5Widgets REQUIRED)

include_directories(include ${catkin_INCLUDE_DIRS})

catkin_package(
	INCLUDE_DIRS .
)

add_executable(nodelet_gui_manager
	nodelet_gui_manager.cpp
	qtguithread.cpp
)

target_link_libraries(nodelet_gui_manager
	${catkin_LIBRARIES}
	#${QT_LIBRARIES}
)
qt5_use_modules(nodelet_gui_manager Core Gui Widgets)

add_library(qtguithread
	qtguithread.cpp
)
qt5_use_modules(qtguithread Core Gui Widgets)

install(TARGETS qtguithread nodelet_gui_manager
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)
