//============================================================================
// Name        : camera-setting.cpp
// Author      : khilmi@eros
// Version     : 8.0
// Copyright   : EROS Re-BORN Team
// Description : We Must Win This Competition, No Matter What !!
//============================================================================

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include "ros/ros.h"

using namespace std;

int main(int argc, char **argv){
	int c_index,focus_auto,focus_absolute,white_balance_temperature_auto;
	int white_balance_temperature,gain,sharpness,backlight_compensation;
	int exposure_auto,exposure_absolute,exposure_auto_priority;
	char buf[1000];
	cout << "Bismillah" <<endl; // prints Bismillah
	
	//ROS Publish/Subscribe Intialize ====================================
	ros::init(argc, argv, "camera_c920");
	ros::NodeHandle n;
	
	n.getParam("/c920/c_index",c_index);
	n.getParam("/c920/focus_auto",focus_auto);
	n.getParam("/c920/focus_absolute",focus_absolute);
	n.getParam("/c920/white_balance_temperature_auto",white_balance_temperature_auto);
	n.getParam("/c920/white_balance_temperature",white_balance_temperature);
	n.getParam("/c920/gain",gain);
	n.getParam("/c920/sharpness",sharpness);
	n.getParam("/c920/backlight_compensation",backlight_compensation);
	n.getParam("/c920/exposure_auto",exposure_auto);
	n.getParam("/c920/exposure_absolute",exposure_absolute);
	n.getParam("/c920/exposure_auto_priority",exposure_auto_priority);
	
	
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl focus_auto=%d",c_index,focus_auto);
	system(buf);system(buf);system(buf);
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl focus_absolute=%d",c_index,focus_absolute);
	system(buf);
	sleep(1);
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl focus_auto=%d",c_index,focus_auto);
	system(buf);
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl focus_absolute=%d",c_index,focus_absolute);
	system(buf);system(buf);system(buf);
	
	
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl white_balance_temperature_auto=%d",c_index,white_balance_temperature_auto);
	system(buf);
	sleep(1);
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl white_balance_temperature=%d",c_index,white_balance_temperature);
	system(buf);
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl gain=%d",c_index,gain);
	system(buf);
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl sharpness=%d",c_index,sharpness);
	system(buf);
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl backlight_compensation=%d",c_index,backlight_compensation);
	system(buf);
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl exposure_auto=%d",c_index,exposure_auto);
	system(buf);
	sleep(1);
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl exposure_absolute=%d",c_index,exposure_absolute);
	system(buf);
	sprintf(buf,"v4l2-ctl -d %d --set-ctrl exposure_auto_priority=%d",c_index,exposure_auto_priority);
	system(buf);
	//system("v4l2-ctl -d 1 --set-ctrl led1_frequency=10");
	return 0;
}
