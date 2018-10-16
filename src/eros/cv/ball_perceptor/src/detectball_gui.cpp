/*
*  detectball
*
*  Detect a black and white ball in images
*  Copyright 2017 Domenico Daniele Bloisi
*
*  This file is part of the tutorial
*  "How to Use OpenCV for Ball Detection" by Domenico D. Bloisi
*  http://profs.scienze.univr.it/~bloisi/tutorial/balldetection.html
*  and it is distributed under the terms of the
*  GNU Lesser General Public License (Lesser GPL)
*
*  detectball is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  detectball is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with createpos.  If not, see <http://www.gnu.org/licenses/>.
*
*  This file contains a C++ OpenCV based implementation for
*  detecting a black and white ball in images as explained
*  in "How to Use OpenCV for Ball Detection" by Domenico D. Bloisi
*  http://profs.scienze.univr.it/~bloisi/tutorial/balldetection.html
*
*  Please, cite the above web page if you use createpos.
*
*  detectball has been written by Domenico D. Bloisi
*
*  Please, report suggestions/comments/bugs to
*  domenico.bloisi@gmail.com
*
*/

#include <ball_perceptor/detectball.h>
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#if defined(__GNUC__) || defined(__GNUG__)
#include <dirent.h>
#endif

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );
void imageSubcriberCallback(const sensor_msgs::ImageConstPtr& msg);

/** Global variables */
DetectBall detectball;

string window_name = "EROS TEAM - ball detection with LBP";

int main( int argc, char **argv )
{
	ros::init(argc, argv, "detectball_gui");
	ros::NodeHandle nh;
    image_transport::ImageTransport it_(nh);
    image_transport::Subscriber image_sub_;

    image_sub_ = it_.subscribe("/image", 10, &imageSubcriberCallback);

	ros::spin();

	return EXIT_SUCCESS;
}


void imageSubcriberCallback(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    detectAndDisplay(cv_ptr->image);

    // Update GUI Window
    //cv::imshow(window_name, cv_ptr->image);
    cv::waitKey(3);
}


/**
* @function detectAndDisplay
*/
void detectAndDisplay( Mat frame )
{
	std::vector<Rect> balls;
	detectball.detectBall(frame, balls, 70);

   	Mat gui_frame = frame.clone();
   	for( unsigned int i = 0; i < balls.size(); i++ )
   	{
    	Point center( balls[i].x + cvRound(balls[i].width*0.5), cvRound(balls[i].y + balls[i].height*0.5) );
      	circle(gui_frame, center, cvRound(balls[i].width*0.5), Scalar( 255, 0, 255 ), 2, 8, 0 );
	  	//Mat ballROI = frame_gray( balls[i] );
   	} 

   	//-- Show what you got
   	imshow( window_name, gui_frame );
   	//int key = cvWaitKey(0);
}
