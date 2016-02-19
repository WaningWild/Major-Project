#include <iostream>
#include <limits>
#include <sys/stat.h>
using namespace std;
//Function Definitions
int genProc();
int genProg();

std::string filenameDate(struct tm *local) {
  char buffer[80];
  strftime(buffer, 80,"%d%m%Y", local);
  std::string returnDate(buffer);
  return returnDate;
}

bool fileExists(const std::string& filename) {
  struct stat buf;
  if(stat(filename.c_str(), &buf) != -1) {
    return true;
  } 
  return false;
}

void drawMenu() {
  cout << "_______________________________________________________" <<endl;
  cout << "||~~~~~~~~~~~~~~~~~~~~ Utilities ~~~~~~~~~~~~~~~~~~~~||" <<endl;
  cout << "||___________________________________________________||" <<endl;
  cout << "||                                                   ||" <<endl;
  cout << "|| 1) Generate Process Log                           ||" <<endl;
  cout << "|| 2) Generate Progress Log                          ||" <<endl;
  cout << "|| 3) Exit                                           ||" <<endl;
  cout << "||___________________________________________________||" <<endl;
}

int main() {
  drawMenu();
  
  bool carryOn = true;
  while(carryOn == true) {
    int usrChoice = 0;
    for(;;) {
      if (cin >> usrChoice) {
        break;
      } else {
        cout << "Please enter a number between 0-9:" <<endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
      }
    }

    switch(usrChoice) {
      case 1:
        cout << "Generating process log..." <<endl;
        genProc();
        break;
      case 2:
        cout << "Generating progress log..." <<endl;
        genProg();
        break;
      case 3:
        return 0;
        break;
      default:
        cout << "Please try again"<<endl; 
    }
  }
}
