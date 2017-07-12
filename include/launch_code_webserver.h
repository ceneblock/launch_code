#ifndef LAUNCH_CODE_WEBSERVER_H
#define LAUNCH_CODE_WEBSERVER_H
#include <sys/stat.h> //file IO
#include <fcntl.h>    //file IO

#include <sys/types.h> //network
#include <sys/socket.h> //network
#include <netinet/in.h> //network
#include <string.h> //for memset
#include <arpa/inet.h> //for inet_ntoa (network to ascii)
#include <unistd.h> //chdir and other POSIX things
#include <poll.h> //so I don't busy wait
#include <sys/ioctl.h> //so I don't waste my time on multiple calls on recv

#include <string>
#include <thread> //C++11 baby!
#include <sstream> //for stringstream
#include <vector>

#include <iostream>
using namespace std;

class lc_webserver
{
  struct thread_pool
  {
    thread *thread_var;
    bool alive;
    sockaddr_in *client_socket;
    unsigned int client_FD;
  };

  private: 
    string root_directory;
    unsigned long port;
    vector<thread_pool> pool;

    void handleData(unsigned int client_FD, string data);
    void handleGET(unsigned int client_FD, string PARAMETER);
    void handleUNKNOWN(unsigned int client_FD);

    void handleClient(thread_pool temp_thread);

  public:
    lc_webserver(unsigned long port, string root_directory)
    {
      this -> root_directory = root_directory;
      this -> port = port;
      if(chdir(this -> root_directory.c_str()) != 0)
      {
        cerr << "Unable to change directories!\n";
        cerr << "Bailing!\n";
        exit(EXIT_FAILURE);
      }
    }

    int lc_listen();
};
#endif
