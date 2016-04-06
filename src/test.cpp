#include "lib/cvdrone/ardrone/ardrone.h"

//- --------------------------------------------------------------------------
// test  main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// --------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // AR.Drone class
  ARDrone ardrone;

  // Initialize
  if (!ardrone.open()) {
      std::cout << "Could not connect to the AR Drone, please try again!" << std::endl;
      return -1;
  }
  std::cout << "Use WSAD to move the drone, R moves the drone up, F moves the drone down." << std::endl;
  std::cout << "Space will land/takeoff the drone." << std::endl;
  // Get an image
  cv::Mat prev_image = ardrone.getImage();

  while (1) {
    // Key input
    char key = cv::waitKey(33);
    if (key == 0x1b) break;
     // Get an image
    cv::Mat image = ardrone.getImage();
    //take off / land
    if (key == ' ') {
      if (ardrone.onGround()) ardrone.takeoff();
      else                    ardrone.landing();
    }

    //Movement
    double vx = 0.0, vy = 0.0, vz = 0.0, vr = 0.0;
    if (key == 'w' || key == CV_VK_UP) 	 vx = 1.0; 
    if (key == 's' || key == CV_VK_DOWN) vx = -1.0; 
    if (key == 'a' || key == CV_VK_LEFT) vy = 1.0; 
    if (key == 'd' || key == CV_VK_RIGHT) vy = -1.0; 
    if (key == 'q') vr = 1.0; 
    if (key == 'e') vr = -1.0; 
    if (key == 'r') vz = 1.0; 
    if (key == 'f') vz = -1.0; 
    ardrone.move3D(vx, vy, vz, vr);

    //Begin vision stuff
    // Convert the camera image to grayscale
    cv::Mat prev_gray, new_gray;
    cv::cvtColor(image, new_gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(prev_image, prev_gray, cv::COLOR_BGR2GRAY);

      // Detect corners
    int max_corners = 1000;
    std::vector<cv::Point2f> prev_corners;
    std::vector<cv::Point2f> new_corners;
    cv::goodFeaturesToTrack(prev_gray, prev_corners, max_corners, 0.01, 0.5);
    cv::goodFeaturesToTrack(new_gray, new_corners, max_corners, 0.01, 0.5);
 
   // Calclate optical flow
    std::vector<unsigned char> status;
    std::vector<float> errors;
    cv::calcOpticalFlowPyrLK(prev_gray, new_gray, prev_corners, new_corners, status, errors);

    // Save the last frame
    image.copyTo(prev_image);

    // Draw optical flow
    for (size_t i = 0; i < status.size(); i++) {
      cv::Point p0(ceil(prev_corners[i].x), ceil(prev_corners[i].y));
      cv::Point p1(ceil(new_corners[i].x), ceil(new_corners[i].y));
      cv::line(image, p0, p1, cv::Scalar(0, 255, 0), 2);
    }

    // Change camera
    static int mode = 0;
    if (key == 'c') ardrone.setCamera(++mode % 4);

    // Display the image
    cv::imshow("camera", image);
  
  }

  // See you
  ardrone.close();

  return 0;
}
