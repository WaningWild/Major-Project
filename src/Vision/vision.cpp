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
 ** Params  : 							*
 **	Mat img          : The image                            *
 **	int max_features : Maximum number of features to find   *
 **				in the image			*
 ** Returns :							*
 **	vector<Point2f>  : A set of points that are indicative	*
 **			   of features within the image		*
 ****************************************************************
*/

std::vector<cv::Point2f> featureDetect(cv::Mat img, int max_features)
{
  std::vector<cv::Point2f> features;
  //cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
  goodFeaturesToTrack(img, features, max_features, 0.01, 0.5);
  return features;
}

/****************************************************************
 ****************************************************************
 ** Function: splitToTiles()                                    *
 ** Purpose : Splits the image passed in, into tiles.           *
 ** Params  : 							*
 **	Mat img	      : The image to be split                   *
 **	int noColumns : The number of columns to split into	*
 **	int noRows    : The number of rows to split into	*
 ** Returns : 							*
 ** 	Mat tiles[]   : An array of images, split from the	*
 **			 original image                        	*
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
