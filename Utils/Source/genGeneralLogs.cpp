#include<iostream>
#include<fstream>
#include<sstream>
#include<ctime>
/**********************************************************
 * This script generates a general log, dated for the     *
 *  date that the script is ran.                          *
 **********************************************************
*/

//Function Defs
std::string filenameDate(struct tm *local);
bool fileExists(const std::string& fname);
//End of Function Defs

//Constants
const std::string dir = "../../Doc/Logs/General/";
const std::string filesuffix = "GeneralLog.txt";

int genGen() {
  //Date collection for it's use later (should be refactored into main.cpp)
  time_t now;
  struct tm * local;
  time(&now);
  local = localtime(&now);
  char buffer[80];
  strftime(buffer,80,"%d/%m/%Y", local);
  std::string formatDate(buffer);

  //Create my new log file
  std::stringstream ss;
  ss << dir <<filenameDate(local) << filesuffix;
  std::string filename(ss.str());
 
  if (!fileExists(filename)) {
    std::ofstream o(filename.c_str());
    
    //Required formatting
    o << "General log for " << formatDate <<std::endl;
    o << std::endl << std::endl;;
    std::cout << "Created "<< filenameDate(local) << filesuffix << " in " << dir << std::endl;
  } else {
    std::cout << filename << " already exists!" << std::endl;
  }
  return 0;
}
