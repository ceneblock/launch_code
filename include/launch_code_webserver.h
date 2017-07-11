#ifndef LAUNCH_CODE_WEBSERVER_H
#define LAUNCH_CODE_WEBSERVER_H
#include <sys/types.h> //network
#include <sys/socket.h> //network
#include <netinet/in.h> //network
#include <string.h> //for memset
#include <arpa/inet.h> //for inet_ntoa (network to ascii)

#include <string>
#include <thread> //C++11 baby!
#include <sstream> //for stringstream
#include <vector>

class lc_webserver
{
  struct thread_pool
  {
    thread *thread_var;
    bool alive;
    sockaddr_in *client_socket;
  }

  private: 
    string root_directory;
    unsigned long port;
    vector<thread_pool> pool;

    void handleGET(char *input, char *input2, sockaddr_in *client_socket);

  public:
    lc_webserver(string root_directory, unsigned long port)
    {
      this -> root_directory = root_directory;
      this -> port = port;
    }

    void listen();
}
#endif
