cmake_minimum_required(VERSION 2.8.3)
project(display_image)

find_package(catkin REQUIRED COMPONENTS roscpp nodelet nodelet_gui_manager)

#find_package(Qt4 COMPONENTS QtCore QtGui)
#include(${QT_USE_FILE})
find_package(Qt5Widgets REQUIRED)

include_directories(
	include/${PROJECT_NAME}/
	${catkin_INCLUDE_DIRS}
)

catkin_package(
	INCLUDE_DIRS .
	LIBRARIES imagewidget display_image
)

qt5_wrap_cpp(MOCFILES
	include/${PROJECT_NAME}/imagewidget.h
)

add_library(imagewidget
	imagewidget.cpp
	${MOCFILES}
)
qt5_use_modules(imagewidget Core Gui Widgets)

add_library(display_image
	display_image.cpp
)

target_link_libraries(display_image
	${catkin_LIBRARIES}
	imagewidget
	#${QT_LIBRARIES}
)
qt5_use_modules(display_image Core Gui Widgets)

install(TARGETS display_image imagewidget
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)

## Copy launch files
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  FILES_MATCHING PATTERN "*.launch"
)

# Mark other files for installation (e.g. launch and bag files, etc.)
install(FILES nodelet_plugins.xml
  	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
