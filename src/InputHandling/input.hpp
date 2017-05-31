#include <iostream>
#ifndef INC_ARDRONE_H
  #define INC_ARDRONE_H
  #include "../lib/cvdrone/ardrone/ardrone.h"
#endif

//Begin variable declarations
extern cv::VideoCapture cap;
extern bool demo;
//End variable declarations

//Begin function declarations
void setCamera(ARDrone *drone);
cv::Mat getImage(ARDrone *ardrone);
//End function declarations

