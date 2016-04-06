/***************************************************************************************
 ****************************************************************************************
 **  File: input.cpp									*
 **  Author: Joseph James (jgj2@aber.ac.uk)						*
 **  Purpose: This file gets the camera feed, determining whether the drone is		*
 **            available and if not, defaulting to the first available camera.		*
 **											*
 ****************************************************************************************
 **  To-do:										*
 **    - There's probably a better method to switch the camera methods			*
 **    - Infinite loop in drone.open()??? 										*
 **    - 										*
 **    - 										*
 ****************************************************************************************
*/
#include "input.hpp"

/****************************************************************
 ****************************************************************
 ** Function: setCamera()					*
 ** Purpose : This function will set the camera to be used.	*
 ** Params  : 							*
 **	ARDrone drone : This is the drone object		*
 ** Returns : None						*
 ****************************************************************
*/
void setCamera(ARDrone drone)
{
  if (!drone.open()) {
    demo = true;
    std::cout << "Drone is not connected, running in demo mode." << std::endl;
    std::cout << "This mode will show the vision system running on the default capture device, usually an integrated webcam on laptops. If you don't have a connected camera, please connect one and relaunch the program";
    std::cout << std::endl << std::endl;
    std::cout << "If you do have an AR Drone 2.0 and were expecting to use it, check it is connected properly and charged." << std::endl;
  } else
  {
    demo = false;
    std::cout << "Drone is connected, using drone camera." << std::endl;
  } 
}

/****************************************************************
 ****************************************************************
 ** Function: getImage()					*
 ** Purpose : This function will return a Mat image, from	*
 **		whatever camera is currently in use.		*
 ** Params  : None						*
 ** Returns : Mat						*
 ****************************************************************
*/
cv::Mat getImage(ARDrone ardrone)
{ 
  cv::Mat image;
  if (!demo) {
    return ardrone.getImage();
  }
  cap.read(image);
  return image;
  
}
