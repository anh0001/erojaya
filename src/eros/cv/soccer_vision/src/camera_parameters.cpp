// Soccer Vision Gui
// Author: Julio Pastrana <pastrana@ais.uni-bonn.de>

// camera_parameters.cpp
// Defines the constants required to distort and undistort points in the camera.
#include "camera_parameters.h"

//
// CamParam struct
//

// Notes:
// The constants were obtained using the OpenCV camera calibration (version 2.4.6) using a checkerboard pattern
// fx and fy are the camera focal lengths
// cx and cy specify the camera optical center
// k1 k2 p1 p2 k3 k4 k5 k6 are the distortion parameters
//
// Refer to:
// http://docs.opencv.org/doc/tutorials/calib3d/camera_calibration/camera_calibration.html

// Camera resolution
//const float CamParam::rx = 640;
//const float CamParam::ry = 480;
const float CamParam::rx = 424;
const float CamParam::ry = 240;

// Camera parameters
const float CamParam::fx =  4.3101499868848003e+02;
const float CamParam::fy =  4.3101499868848003e+02;
const float CamParam::cx =  2.6139884044698385e+02;
const float CamParam::cy =  2.0516247546568135e+02;

// Radial distortion parameters
const float CamParam::k1 = -1.4334388480908447e+00;
const float CamParam::k2 =  2.0700796519939146e+01;
const float CamParam::k3 =  1.8824323641297690e+03;
const float CamParam::k4 =  0.0;
const float CamParam::k5 =  0.0;
const float CamParam::k6 =  2.0187374017386162e+03;

// Tangential distortion parameters
const float CamParam::p1 = -1.6149998446134701e-02;
const float CamParam::p2 = -2.6142641942786468e-02;

// Linear distortion extension parameters (calculated based on the other OpenCV-generated constants using calcparams.m)
const float CamParam::api =  8.0;
const float CamParam::apo =  14.0;
const float CamParam::ani = -8.0;
const float CamParam::ano = -12.0;
const float CamParam::bpi =  8.0;
const float CamParam::bpo =  12.0;
const float CamParam::bni = -8.0;
const float CamParam::bno = -14.0;
const float CamParam::mx =  348.2478;
const float CamParam::bx =  -6.0891e+03;
const float CamParam::my =  421.1629;
const float CamParam::by =  -3.7179e+03;
// EOF
