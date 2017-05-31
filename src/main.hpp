#ifndef INC_ARDRONE_H
  #define INC_ARDRONE_H
  #include "lib/cvdrone/ardrone/ardrone.h"
#endif

#ifndef INC_GLOBALS_H
  #define INC_GLOBALS_H
  #include "Globals/globals.hpp"
#endif
#include <iostream>

/*Forward declarations*/
void setCamera(ARDrone *ardrone);
cv::Mat getImage(ARDrone *ardrone);
void splitToTiles(cv::Mat, int, int, cv::Mat*);
std::vector<cv::Point2f> featureDetect(cv::Mat img, int max_features);
interestPoint convertToInterestPoint(cv::Point2f *curFeature, cv::Point2f *prevFeature);

bool isForegroundBackground(cv::Mat*, interestPoint*);
extern cv::VideoCapture cap;


void performAlgorithm(ARDrone *ardrone);
std::vector<cluster> clusterPoints(std::vector<interestPoint> *interestPoints, int splitLevel);

double calcAngle(cv::Point2f *current, cv::Point2f *previous);
void refinePoints(std::vector<interestPoint> *myPoints, std::vector<cluster> *myClusters);

cv::Scalar colorTab[] =
{
  //  Scalar(B, G, R)
  cv::Scalar(0, 0, 255),
  cv::Scalar(0, 255, 0),
  cv::Scalar(255, 0, 0),
  cv::Scalar(0, 128,128),
  cv::Scalar(128, 128, 0),
  cv::Scalar(128, 128, 0),
  cv::Scalar(0, 0, 0),
  cv::Scalar(255,255,255),

};
