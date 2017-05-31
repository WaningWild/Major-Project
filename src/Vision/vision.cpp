/****************************************************************************************
 ****************************************************************************************
 **  File: vision.cpp                                                                   *
 **  Author: Joseph James (jgj2@aber.ac.uk)                                             *
 **  Purpose: This file	performs the vision element on the camera feed provided.	*
 **                                                                                     *
 ****************************************************************************************
 **  To-do:                                                                             *
 **    - Is it better to have feature detection, tracking, background segmentation      *
 **    	   and interpretation in seperate files?                                        *
 **    - splitToTiles() Needs to return POINTER to Array! (We should pass in a pointer  *
 **		to an array to be filled, so we're not creating an array on the stack)  *
 **    -
 ****************************************************************************************
*/
#include "vision.hpp"

/****************************************************************
 ****************************************************************
 ** Function: featureDetect()                                   *
 ** Purpose : Detects the easy to track features from an image  *
 ** Params  : 							                        *
 **	Mat img          : The image                                *
 **	int max_features : Maximum number of features to find       *
 **				in the image			                        *
 ** Returns :							                        *
 **	vector<Point2f>  : A set of points that are indicative	    *
 **			   of features within the image		                *
 ****************************************************************
*/

std::vector<cv::Point2f> featureDetect(cv::Mat img, int max_features)
{
  std::vector<cv::KeyPoint> features;
  std::vector<cv::Point2f> returnFeatures;

  cv::Ptr<cv::FastFeatureDetector> detector = cv::FastFeatureDetector::create(10, true);
  detector->detect(img, features);
  cv::KeyPoint::convert(features,returnFeatures);

  return returnFeatures;
}

/****************************************************************
 ****************************************************************
 ** DEPRECATED                                                 **
 ****************************************************************
 ** Function: splitToTiles()                                    *
 ** Purpose : Splits the image passed in, into tiles.           *
 ** Params  : 							                        *
 **	Mat img	      : The image to be split                       *
 **	int noColumns : The number of columns to split into	        *
 **	int noRows    : The number of rows to split into	        *
 ** Returns : 							                        *
 ** 	Mat tiles[]   : An array of images, split from the	    *
 **			 original image                        	            *
 ****************************************************************
*/
void splitToTiles(cv::Mat img, int noColumns, int noRows, cv::Mat* tiles)
{
  int colCalc = img.cols / noColumns;
  int rowCalc = img.rows / noRows;
  int count = 0;
  for (int r = 0; r < img.rows; r += rowCalc) {
    for (int c = 0; c < img.cols; c += colCalc) {
      tiles[count] = img(cv::Range(r, cv::min(r + rowCalc, img.rows)),
			cv::Range(c,cv::min(c + colCalc, img.cols)));
      count++;
    }
  }
}

/****************************************************************
 ****************************************************************
 ** Function: segmentForegroundBackground()                     *
 ** Purpose : This function will seperate background/Camera     *
 **             motion from foreground motion                   *
 **                                                             *
 ** Params  : cv::Mat *homography : A pointer to a homography   *
 **                          to multiply the points by          *
 **                                                             *
 **           interestPoint *point : the point that we're       *
 **                          classifying                        *
 **                                                             *
 ** Returns : bool : True if foreground, false if background    *
 ****************************************************************
*/

bool isForegroundBackground(cv::Mat *homography, interestPoint *point) {
  //Here we convert the point into a matrix to be multiplied.
  cv::Mat pointMat = (cv::Mat_<float>(3, 1) << (float)point->previousFeature.x , (float)point->previousFeature.y, 1);
  //convert the expected point into a mat for easy comparison
  cv::Mat actualPointMat = (cv::Mat_<float>(3,1) << (float)point->currentFeature.x, (float)point->currentFeature.y, 1);

  //homography is CV_64F rather than the CV_32F that the point Mats are
  pointMat.convertTo(pointMat, CV_64F);
  pointMat.convertTo(actualPointMat, CV_64F);

  if (homography->cols == pointMat.rows) {

    cv::Mat estPointMat = *homography * pointMat;
    float estimation_error = norm(estPointMat - actualPointMat);

    //If the absolute value of my estimation error is bigger than my segmentation threshold.
    if (std::abs(estimation_error) > segThreshold) {
      point->isForeground = true;
      return true;
    } else {
      point->isForeground = false;
      return false;
    }
  }
  //should never be reached
  return false;
}

/****************************************************************
 ****************************************************************
 ** Function: calcAngle()                                       *
 ** Purpose : This function will calculate the angle of 2 point *
                vectors passed to it.                           *
 ** Params  : cv::Point2f *current, cv::Point2f *previous       *
 **                 : The two points to calculate the angle     *
 **                     between.                                *
 **                                                             *
 ** Returns : double : The calculated angle                    	*
 ****************************************************************
*/
double calcAngle(cv::Point2f *current, cv::Point2f *previous) {
  double angle = (double)atan2(previous->y - current->y, previous->x - current->x);
  if ((current->x * previous->y) - (current->y*previous->x) < 0) {
    angle = -angle;
  }
  angle = angle * 180/3.14159;

  return angle < 0.0 ? ceil(angle - 0.5) : floor(angle + 0.5);
}

/****************************************************************
 ****************************************************************
 ** Function: convertToInterestPoint()                          *
 ** Purpose : This function will convert detected features into *
 **             interest points.                                *
 ** Params  : cv::Point2f *curFeature, *prevFeature             *
 ** Returns : interestPoint : The converted interest point      *
 ****************************************************************
*/
interestPoint convertToInterestPoint(cv::Point2f *curFeature, cv::Point2f *prevFeature) {
  interestPoint iP;

  iP.currentFeature = *curFeature;
  iP.previousFeature = *prevFeature;
  iP.MotionVectorAngle = calcAngle(curFeature, prevFeature);

  return iP;
}

/****************************************************************
 ****************************************************************
 ** Function: angularDeviation(double A, double B)              *
 ** Purpose : Calculates the angular deviation of angle A from  *
 **             angle B.                                        *
 ** Params  : double a,b							                        *
 ** Returns : Bool : true if greater than threshhold,           *
 **                 false otherwise 							*
 ****************************************************************
*/
double angularDeviation(double a, double b) {
 return std::abs(a - b);
}

/****************************************************************
 ****************************************************************
 ** Function: min()                                             *
 ** Purpose : returns the minimum of two doubles                *
 ****************************************************************
*/

double min(double a, double b) {
  return (a < b) ? a : b;
}

/****************************************************************
 ****************************************************************
 ** Function: mergeVectors()                              *
 ** Purpose : Merge two sorted vectors                          *
 ** Params  : 							                        *
 ** Returns : Bool : true if greater than threshhold,           *
 **                 false otherwise 							*
 ****************************************************************
*/
void mergeVectors(std::vector<interestPoint> &vec, const std::vector<interestPoint>& left, const std::vector<interestPoint>& right) {
  unsigned left_it = 0, right_it = 0;

  while(left_it < left.size() && right_it < right.size()) {
    if (left[left_it].MotionVectorAngle > right[right_it].MotionVectorAngle) {
      vec[left_it + right_it] = left[left_it];
      left_it++;
    } else {
      vec[left_it + right_it] = right[right_it];
      right_it++;
    }
  }

  while( left_it < left.size()) {
    vec[left_it + right_it] = left[left_it];
    left_it++;
  }

  while(right_it < right.size()) {
    vec[left_it + right_it] = right[right_it];
    right_it++;
  }
}
/****************************************************************
 ****************************************************************
 ** Function: mergeSort                                         *
 ** Purpose : Sorts the interest points by motion vector angle  *
 ** Params  : 							                        *
 ** Returns :                                                   *
 ****************************************************************
*/
void mergeSort(std::vector<interestPoint>& vec) {
  if(vec.size() == 1) {
    return;
  }

  std::vector<interestPoint>::iterator middle = vec.begin() + (vec.size() / 2);

  std::vector<interestPoint> left(vec.begin(), middle);
  std::vector<interestPoint> right(middle, vec.end());
  if (left.size() != 0) {
    mergeSort(left);
  }
  if (right.size() != 0) {mergeSort(right);}

  mergeVectors(vec, left, right);
}

/****************************************************************
 ****************************************************************
 ** Function: geMotionMagnitude()                               *
 ** Purpose : This function will return the motion magnitude of *
 **             a point through frames.                         *
 ** Params  : interestPoint point : The point to find the       *
 **                 magnitude of motion from.                   *
 ** Returns : double : The distance between the previous        *
 **                     position of the point and the current   *
 ****************************************************************
*/
double getMotionMagnitude(interestPoint point) {
  double returnVal;

  returnVal = std::sqrt(
                    (point.currentFeature.x - point.previousFeature.x)*(point.currentFeature.x - point.previousFeature.x)
                   +(point.currentFeature.y - point.previousFeature.y)*(point.currentFeature.y - point.previousFeature.y)
                   );
}

/****************************************************************
 ****************************************************************
 ** Function: clusterPoints()                                   *
 ** Purpose : This function will perform the basics to start    *
 **             clustering the points.
 ** Params  : 							                        *
 ** Returns : 							                     	*
 ****************************************************************
*/
std::vector<cluster> clusterPoints(std::vector<interestPoint> *interestPoints, int splitLevel) {

  ///Sort the features in descending order based on number of occurences of corresponding motion vector angle
  mergeSort(*interestPoints);

  std::vector<cluster> allClusters;
  cluster current_cluster;
  current_cluster.centroid = interestPoints->at(0);
  current_cluster.label = 0;

  for (size_t i = 0; i < interestPoints->size(); i++) {
    if (std::abs(interestPoints->at(i).MotionVectorAngle - current_cluster.centroid.MotionVectorAngle) < angleThreshold) {
      current_cluster.interestPoints.push_back(interestPoints->at(i));
    }
    else {
      allClusters.push_back(current_cluster);
      current_cluster.centroid = interestPoints->at(i);
      current_cluster.interestPoints.clear();
      current_cluster.interestPoints.push_back(interestPoints->at(i));
      current_cluster.label = current_cluster.label + 1;
    }
  }

  return allClusters;

}
/****************************************************************
 ****************************************************************
 ** Function: euclideanDistance()                                    *
 ** Purpose : This function will return the Euclidean distance  *
 **             between 2 interest points                       *
 ** Params  : 							                        *
 ** Returns : 							                     	*
 ****************************************************************
*/
double euclideanDistance(interestPoint x, interestPoint y) {
  double x1 = x.currentFeature.x, y1 = x.currentFeature.y, x2= y.currentFeature.x, y2 = y.currentFeature.y;
  return std::sqrt(((x2-x1)*(x2-x1)) - ((y2 -y1)*(y2-y1)));
}

/****************************************************************
 ****************************************************************
 ** Function: refinePoints()                                    *
 ** Purpose : This function will refine our clusters by appling *
 **             k-NN to points within the cluster.              *
 ** Params  : 							                        *
 ** Returns : 							                     	*
 ****************************************************************
*/
void refinePoints(std::vector<interestPoint> *myPoints, std::vector<cluster> *myClusters) {

  std::vector<cv::Point2f> allPoints;
  ///Create a vector of all points to calculate neighbours within a certain euclidean distance
  for (int i = 0; i < myPoints->size(); i++) {
    allPoints.push_back(myPoints->at(i).currentFeature);
  }

  for (int i = 0; i < myClusters->size(); i++) {
    cluster x = myClusters->at(i);
      for (int j = 0; j < x.interestPoints.size(); j++) {
      int labelCount = 0;
        for (int k = 0; k < x.interestPoints.size(); k++) {
          if(j==k){continue;}
          if (euclideanDistance(x.interestPoints.at(j), x.interestPoints.at(k)) > kNNRadius) {
            labelCount++;
          }
        }
        if (labelCount > x.interestPoints.size()/2 || !x.interestPoints.at(j).isForeground)          {
          x.interestPoints.erase(x.interestPoints.begin() + j); j--;
        }
      }
    myClusters->at(i) = x;
  }
}
