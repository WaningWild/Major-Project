#ifndef INC_ARDRONE_H
  #define INC_ARDRONE_H
  #include "../lib/cvdrone/ardrone/ardrone.h"
#endif

#ifndef INC_GLOBALS_H
  #define INC_GLOBALS_H
  #include "../Globals/globals.hpp"
#endif

#include <vector>
#include "../lib/cvdrone/3rdparty/opencv/include/opencv2/flann/flann.hpp"

std::vector<cv::Point2f> featureDetect(cv::Mat img, int max_features);
void splitToTiles(cv::Mat, int, int, cv::Mat*);
const int segThreshold = 2;
const int angleThreshold = 15;
const int kNNRadius = 10;
