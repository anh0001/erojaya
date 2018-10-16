#ifndef DETECTBALL_H
#define DETECTBALL_H

#include <ros/package.h>
#include <image_transport/image_transport.h>

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

using namespace cv;	

	/**
	* @class FindBall
	* @brief Class that has the methods to find the ball in the images.	
	**/
	class DetectBall
	{


	public:

		cv::String jauh_cascade_name;
		cv::String dekat_cascade_name;
		cv::String sedang_cascade_name;
		cv::CascadeClassifier jauh_cascade;
		cv::CascadeClassifier sedang_cascade;
		cv::CascadeClassifier dekat_cascade;

		unsigned char MIN_COLOR_INTENSITY;
		unsigned char MIN_BALL_REGION_SIZE;
		unsigned char BALL_REGION_SIZE_INCR;
		double ROBOT_TRUNK_TO_FLOOR_DISTANCE;
		double BALL_CENTER_TO_FLOOR_DISTANCE;

		/*! \fn FindBall()
		*   \brief Constructor that initialize all the necesary structures and variables.
		*/
		DetectBall();
		
		/*! \fn virtual void ~FindBall()
		*   \brief Destructor
		*/		
		~DetectBall(){}
		
		/*! \fn void detectBall(Mat image);
		*   \brief Finds the ball in the current frame.
		*   \param image.
		*/				
		void detectBall(Mat image, std::vector<Rect>& detected_balls, int sudutKepala);
		
	};
	
#endif
