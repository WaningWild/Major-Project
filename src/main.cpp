/****************************************************************************************
 ****************************************************************************************
 **  File: main.cpp									*
 **  Author: Joseph James (jgj2@aber.ac.uk)						*
 **  Purpose: This file gets the program up and running, creating necessary objects.	*
 **											*
 ****************************************************************************************
 **  To-do:										*
 **    - Decouple Vision from input							*
 **    - Place vision in it's own module						*
 **    - Place input in it's own module							*
 **    - Create header files								*
 ****************************************************************************************
*/
#include "main.hpp"

int main(int argc, char* argv[]) 
{
  
  ARDrone ardrone;
  std::cout << "Setting camera!" << std::endl;
  setCamera(ardrone); //Open Webcam or Drone if connected.
  if (!ardrone.open() && !cap.isOpened()) {
    std::cout << "NOT CONNECTED" << std::endl;
    return -1;
  } 
   
  //Check battery (BUGGED, check github for fix. is it??? seemed fixed??)
  std::cout << "Battery percentage: " << ardrone.getBatteryPercentage() << "[%]" << std::endl;

  //Create a window called "Video"
  cv::namedWindow("Video", CV_WINDOW_AUTOSIZE);
  
  //Array of video sections  
  cv::Mat tiles_prev[12], tiles_cur[12];
   
  
  cv::Mat img_cur, img_curgray;
  //Show video feed.
  while(1)
  {
     
    cv::Mat img_prev = getImage(ardrone), img_prevgray;
    cv::cvtColor(img_prev, img_prevgray, cv::COLOR_BGR2GRAY);
    img_cur = getImage(ardrone);
    cv::cvtColor(img_cur, img_curgray, cv::COLOR_BGR2GRAY);

    splitToTiles(img_prevgray, 4, 3, tiles_prev); splitToTiles(img_curgray, 4, 3, tiles_cur);
    
    std::vector<cv::Point2f> features_prev[12], features_cur[12];   
   
    for (int c = 0; c < 12; c++) {
      
      features_prev[c] = featureDetect(tiles_prev[c], 84);
      features_cur[c] = featureDetect(tiles_cur[c], 84);
     
      std::vector<unsigned char> status;
      std::vector<float> errors;

      cv::calcOpticalFlowPyrLK(tiles_prev[c], tiles_cur[c], features_prev[c], features_cur[c], status, errors); 
      
      for (size_t i = 0; i < status.size(); i++) {
        cv::Point p0(std::ceil(features_prev[c][i].x), std::ceil(features_prev[c][i].y));
        cv::Point p1(std::ceil(features_cur[c][i].x), std::ceil(features_cur[c][i].y));
        cv::line(tiles_cur[c], p1, p0, cv::Scalar(0,255,0), 2);
      }
      
    }

    
    imshow("Vid1", tiles_cur[0]);
    imshow("Vid2", tiles_cur[1]);
    imshow("Vid3", tiles_cur[2]);
    imshow("Vid4", tiles_cur[3]);
    imshow("Vid5", tiles_cur[4]);
    imshow("Vid6", tiles_cur[5]);
    imshow("Vid7", tiles_cur[6]);
    imshow("Vid8", tiles_cur[7]);
    imshow("Vid9", tiles_cur[8]);
    imshow("Vid10", tiles_cur[9]);
    imshow("Vid11", tiles_cur[10]);
    imshow("Vid12", tiles_cur[11]);
    

    if (cv::waitKey(30) == 27) {std::cout<<"User exit" << std::endl; return 0;}
    img_prev = img_cur;
    //img = ardrone.getImage();
    //cout << img.rows << "     " << img.cols << endl;
    cv::imshow("Video",img_cur);
  }
  return 0;
}

