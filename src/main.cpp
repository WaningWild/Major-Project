/****************************************************************************************
 ****************************************************************************************
 **  File: main.cpp									                                    *
 **  Author: Joseph James (jgj2@aber.ac.uk)						                        *
 **  Purpose: This file gets the program up and running, creating necessary objects.	*
 **											                                            *
 ****************************************************************************************
 **  To-do:										                                        *
 ****************************************************************************************
*/
#include "main.hpp"
#include <cmath>
int main(int argc, char* argv[])
{
  //Set up drone object
  //Create a window called "Video"
  cv::namedWindow("Video", CV_WINDOW_AUTOSIZE);

  ARDrone ardrone;
  performAlgorithm(&ardrone);

}

void performAlgorithm(ARDrone *ardrone) {
  std::cout << "Setting camera!" << std::endl;
  setCamera(ardrone); //Open the default camera or the drone camera if it's connected.
  //Check battery
  int droneBatt = ardrone->getBatteryPercentage();
  std::cout<< "The current battery percentage is " << droneBatt << "%" <<std::endl;
  if (droneBatt < 20) {
    std::cout<< "The drone is below 20%, you will be unable to control the drone until it's charged properly." << std::endl;
  }

  //Declaration of our feature vectors.
  std::vector<cv::Point2f> features_prev, features_cur;
  std::vector<bool> foregroundBackground; //Stores a boolean; TRUE if foreground, FALSE if background.
  //Declaration of image matrices, then the first image is converted to grayscale.
  cv::Mat img_cur, img_curgray , img_prev = getImage(ardrone), img_prevgray;
  cv::Mat homography;
  int frameNum = 0, tile = 0;

  cv::cvtColor(img_prev, img_prevgray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(img_prevgray, img_prevgray, cv::Size(3,3), 10, 10, cv::BORDER_DEFAULT);

  while(cv::waitKey(1) != 27)
  {
    //Get new image and convert to greyscale
    img_cur = getImage(ardrone);
    cv::cvtColor(img_cur, img_curgray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(img_curgray, img_curgray, cv::Size(5,5), 10, 10, cv::BORDER_DEFAULT);

    //Detect features in previous and current frames and then calculate optical flow.
    features_prev = featureDetect(img_prevgray,1008);
    features_cur = featureDetect(img_curgray, 1008);
    std::vector<unsigned char> status;
    std::vector<float> errors;
    if (features_prev.size() > 0 && features_cur.size() >0) {
      cv::calcOpticalFlowPyrLK(img_prevgray, img_curgray, features_prev, features_cur, status, errors);

      //Convert this feature list into proper interest points we can use later.
      //For every feature with an optical flow
      //  convert to an interest point
      //  add to interestPoints vector
      std::vector<interestPoint> interestPointVector;
      for(size_t feature = 0; feature < features_cur.size(); feature++) {
        if(!status.at(feature)) {
          continue;
        }
        interestPointVector.push_back(convertToInterestPoint(&features_cur.at(feature), &features_prev.at(feature)));
      }


      homography = cv::findHomography(features_prev,features_cur, cv::RANSAC, 10, cv::noArray(), 2000, 0.5);

      frameNum++;

      for (size_t interestPointCount = 0; interestPointCount < interestPointVector.size(); interestPointCount++) {
        isForegroundBackground(&homography, &interestPointVector[interestPointCount]);
      }
      std::vector<cluster> myClusters = clusterPoints(&interestPointVector, 3);

      if (myClusters.size() > 1) {
        refinePoints(&interestPointVector, &myClusters);
      }

      for (size_t clusterCount = 0; clusterCount < myClusters.size(); clusterCount++) {

        for (size_t interestPointCount = 0; interestPointCount < myClusters.at(clusterCount).interestPoints.size(); interestPointCount++) {
          cv::Point p0(std::ceil(myClusters.at(clusterCount).interestPoints.at(interestPointCount).currentFeature.x), std::ceil(myClusters.at(clusterCount).interestPoints.at(interestPointCount).currentFeature.y));
          cv::Point p1(std::ceil(myClusters.at(clusterCount).interestPoints.at(interestPointCount).previousFeature.x), std::ceil(myClusters.at(clusterCount).interestPoints.at(interestPointCount).previousFeature.y));

          cv::line(img_cur, p0, p1, cv::Scalar(0,255,0), 2 );
          cv::circle(img_cur, p0, 2, colorTab[myClusters.at(clusterCount).label], cv::FILLED, cv::LINE_AA);
        }
      }

    }
    img_curgray.copyTo(img_prevgray);
    foregroundBackground.clear();


    cv::imshow("Video",img_cur);

    ///This section of code below is provided as is from CVDrone and should be credited to the creator of CVDrone, puku0x.
    char key = cv::waitKey(50);
    // Take off / Landing
    if (key == (char)27) {break;}
    if(key == ' ') {
      if (ardrone->onGround())  ardrone->takeoff();
      else                      ardrone->landing();
    }
    

    // Move
    double vx = 0.0, vy = 0.0, vz = 0.0, vr = 0.0;
    if (key == 'w' || key == CV_VK_UP)    vx =  2.0;
    if (key == 's' || key == CV_VK_DOWN)  vx = -2.0;
    if (key == 'q' || key == CV_VK_LEFT)  vr =  2.0;
    if (key == 'e' || key == CV_VK_RIGHT) vr = -2.0;
    if (key == 'a') vy =  2.0;
    if (key == 'd') vy = -2.0;
    if (key == 'r') vz =  2.0;
    if (key == 'f') vz = -2.0;
    ardrone->move3D(vx, vy, vz, vr);
  }
}

