cmvision
========

cmvision for image segmentation

* Error on wxwidget find_file not readable
Solved by directly set this file /usr/share/cmake-3.5/Modules/FindwxWidgets.cmake as follow:
file(GLOB _filename /usr/include/wx-3.0/wx/version.h)

replacing this line
find_file(_filename wx/version.h PATHS ${wxWidgets_INCLUDE_DIRS} NO_DEFAULT_PATH)




