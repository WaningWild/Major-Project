#ifndef INC_ARDRONE_H
  #define INC_ARDRONE_H
  #include "../lib/cvdrone/ardrone/ardrone.h"
#endif

#include <vector>

std::vector<cv::Point2f> featureDetect(cv::Mat img, int max_features);
void splitToTiles(cv::Mat, int, int, cv::Mat*);

