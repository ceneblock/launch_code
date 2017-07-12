#include "config.h"

#include "launch_code_webserver.h"


#include <signal.h> //catching when the program dies

#include <unistd.h> //POSIX

#include <iostream>
#include <string>

#define DATE  "2017/07/11"

using namespace std;

bool DEBUG = false;

/**
 * @brief prints the help message
*/
void print_help()
{
  cout << "launch_code webserver\n";
  cout << "a small webserver built so I have a recent finished project to show :)\n";
  cout << endl;
  cout << PACKAGE << " options\n";
  cout << "-h          display this message\n";
  cout << "-D          enable Debug mode\n";
  cout << "-p ###      what port to run on\n";
  cout << "-r /path    where our root document is\n";     
}

int main(int argc, char* argv[])
{
  string root_dir = "./";
  unsigned long port = 80;
  int opt = -1;
  lc_webserver *my_webserver;

  while((opt = getopt(argc, argv, "Dhp:r:")) != -1)
  {
    switch(opt)
    {
      case 'D':
        DEBUG = true;
        break;
      case 'h':
        print_help();
        exit(EXIT_SUCCESS);
        break; //not needed, but for habbit
      case 'p':
        port = atoi(optarg);
        break;
      case 'r':
        root_dir = optarg;
        break;
      default:
        cerr << "Unknown argument: " << opt << endl; 
        //unknown argument
        break;
    }
  }

  my_webserver = new lc_webserver(port, root_dir);

  int return_status = my_webserver -> lc_listen();

  delete my_webserver;

  return return_status;

}


