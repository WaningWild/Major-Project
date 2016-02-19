#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
/************************************************************
 * This script will generate a process log, dated for the   *
 * date when the script is ran.				    *
 ************************************************************
*/
//Function Defs
std::string filenameDate(struct tm *local);
bool fileExists(const std::string& fname);
//End of Function Defs

const std::string dir = "../../Doc/Logs/Process/";
const std::string filesuffix = "ProcessLog.txt";

int genProc() {
  //Date collection for it's use later.
  time_t now;
  struct tm * local;
  time (&now);
  local = localtime(&now);
  char buffer[80];
  strftime(buffer, 80, "%d/%m/%Y", local);
  std::string formatDate(buffer);  
  
  //Create my new log file
  std::stringstream ss;
  ss << dir << filenameDate(local) << filesuffix;
  std::string filename(ss.str());
  
  if(!fileExists(filename)) {
    std::ofstream o(filename.c_str());
    //Enter the required titles/categories in the file
    o << "Process Log for " << formatDate << std::endl;
    o << std::endl;
    o << "What went well:" << std::endl;
    o << "  - \n  - \n  - \n  - \n  - \n";
    o << "What could be improved:" << std::endl;
    o << "  - \n  - \n  - \n  - \n  - \n";
    o << "How improvements can be made:" <<std::endl;
    o << "  - \n  - \n  - \n  - \n  - \n";
    std::cout << "Created " << filename << " in " << dir << std::endl;
  } else {
    std::cout << filename << " already exists!" << std::endl;
  }
  return 0;
}


