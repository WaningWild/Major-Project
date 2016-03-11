/***************************************************************************************
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
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/video.hpp"

using namespace std;
using namespace cv;

void readTiles();

int main(int argc, char* argv[]) 
{
  VideoCapture cap(0); //Open Webcam

  //Check the camera is opened
  if (!cap.isOpened()) {cout << "Failed to open webcam!" <<endl; return -1;}

  //Get the width and height of the camera footage.
  double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH), dHeight = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  cout << "Frame width: " << dWidth << endl << "Frame height: " << dHeight << endl;

  
  //Create a window called "Video"
  namedWindow("Video", CV_WINDOW_AUTOSIZE);
  Mat img;
  cap.read(img);
  int colCalc = img.cols / 4;
  int rowCalc = img.rows / 3;
  
  //Array of video sections  
  Mat tiles[20];
  int count = 0;
  
  imshow("Video", img);
  cout << "what" << endl;
  //Show video feed.
  while(1)
  { 
    readTiles();
    //processTile(tiles[count]);
    imshow("Vid1", tiles[0]);
    imshow("Vid2", tiles[1]);
    imshow("Vid3", tiles[2]);
    imshow("Vid4", tiles[3]);
    imshow("Vid5", tiles[4]);
    imshow("Vid6", tiles[5]);
    imshow("Vid7", tiles[6]);
    imshow("Vid8", tiles[7]);
    imshow("Vid9", tiles[8]);
    imshow("Vid10", tiles[9]);
    imshow("Vid11", tiles[10]);
    imshow("Vid12", tiles[11]);
       
    count = 0;
    if (waitKey(30) == 27) {cout<<"User exit" << endl; break;}
    cap.read(img);
    cout << img.rows << "     " << img.cols << endl;
    imshow("Video",img);
  }
  return 0;
}

void readTiles() 
{
  for (int r = 0; r <img.rows;r += rowCalc) 
  {
    for (int c = 0; c <img.cols; c += colCalc) 
    {
      cout << "count = " << count << endl; 
      tiles[count] = img(Range(r, min(r + rowCalc, img.rows)),
                      Range(c, min(c + colCalc, img.cols)));
       
      cout << tiles[count].rows << " Rows" << endl << tiles[count].cols << " Cols" << endl;
      count++;
     }
  }
}
