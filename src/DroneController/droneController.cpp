/****************************************************************************************
 ****************************************************************************************
 **  File: droneController.cpp                                                          *
 **  Author: Joseph James (jgj2@aber.ac.uk)                                             *
 **  Purpose: This file controls the drone itself.				                        *
 **                                                                                     *
 ****************************************************************************************
 **  To-do:                                                                             *
 **    - PID Controller                                                                 *
 **    - API for control with vision system                                             *
 ****************************************************************************************
*/

/****************************************************************
 ****************************************************************
 ** Function: moveForward()                                     *
 ** Purpose : Moves the drone forwards a certain distance       *
 ** Params  : 							                        *
 ** Returns :							                        *
 ****************************************************************
*/
/*
void moveForward() {
  //Converting from metres to a velocity (used by move3D) is easy.
  for (convert(distance)){
    //ardrone.move3D(vx, vy, vz, vr)
    ardrone.move3D(velocity,0,0,0);
    wait(100);
  }
}
*/

/****************************************************************
 ****************************************************************
 ** Function: convert()                                         *
 ** Purpose : Converts a distance in metres to the time we need *
 **           to fly at our velocity                            *
 ** Params  :                                                   *
 **           int distance  :  The distance to be converted     *
 ** Returns :                                                   *
 **           int time      :  The time to travel at a          *
 **                               particular velocity			*
 ****************************************************************
*//*
private int convert(int distance) {
  return (distance / velocity);
}
*/
