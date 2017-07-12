#include "launch_code_webserver.h"

/**
 * @brief handles client's data
 * @param client_FD the FD to the client
 * @param data what the request way that was sent to us
 */
void lc_webserver::handleData(unsigned int client_FD, string data)
{
  char *token;
  string METHOD;
  string PARAMATER;

  if((token = strtok(const_cast<char *>(data.c_str()), " ")) != NULL)
  {
    METHOD = token;
  }
  else
  {
    //handle error
  }
  if((token = strtok(NULL, " ")) != NULL)
  {
    PARAMATER = token;
  }
  else
  {
    //handle error
  }
  
  cout << "Method: " << METHOD << endl;
  cout << "Parameter: " << PARAMATER << endl;

}

/**
 * @brief handles when a client connects
 * @input temp_thread all of the stuff we'll need
*/
void lc_webserver::handleClient(thread_pool temp_thread)
{
  unsigned int bytes_available;
  pollfd client_poll;
  client_poll.fd = temp_thread.client_FD;
  client_poll.events = POLLIN;
  int time = 1000; //one second
  int poll_result = poll(&client_poll, 1, time);

  if(poll_result == 0)
  {
    while(poll_result == 0)
    {
      poll_result = poll(&client_poll, 1, time);
    }
  }

  if(poll_result > 0)
  {
    ioctl(temp_thread.client_FD,FIONREAD,&bytes_available);

    while(bytes_available > 0) //This loop shouldn't repeat...
    {

      char client_data[bytes_available];
      recv(temp_thread.client_FD, &client_data, bytes_available, 0);

      string data = client_data;

      data = data.substr(0,(data.find("\n")));
      handleData(temp_thread.client_FD, data);
    }
  }
  else
  {
    cerr << "Bummer, we got an error from poll\n";
    cerr << "Just gonna sweep it under the rug\n";
    close(temp_thread.client_FD);
    delete temp_thread.client_socket;
    
  }


}


/**
 * @brief listens and accepts new connections. Once accepted, a new thread is
 * spun off
 * @return if there was an error.
*/
int lc_webserver::lc_listen()
{
  int return_status = EXIT_SUCCESS;
  socklen_t client_addr_len;

  int socket_desc = socket(AF_INET, SOCK_STREAM,0);

  sockaddr_in srv_addr, *cli_addr;

  if(socket_desc == -1)
  {
    cerr << "Unable to create new sockets!\n";
    cerr << "Aborting!\n";
    return_status = EXIT_FAILURE;
  }

  int yes = 1;
  if(setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
  {
    cerr << "Unable to set reuseable options :-\\\n";
  }
  memset(&srv_addr, 0, sizeof(srv_addr));

  srv_addr.sin_family = AF_INET;
  srv_addr.sin_addr.s_addr = INADDR_ANY;
  srv_addr.sin_port = htons(port);

  if(bind(socket_desc, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0)
  {
    cerr << "Error on binding " << port << endl;
    return_status = EXIT_FAILURE;
  }

  listen(socket_desc,5);

  client_addr_len = sizeof(&cli_addr);

  while(true)
  {
    thread_pool temp_thread;
    cli_addr = new sockaddr_in();
    
    temp_thread.client_FD = accept(socket_desc, (struct sockaddr *) cli_addr, &client_addr_len);
    temp_thread.client_socket = cli_addr;
    temp_thread.thread_var = new thread(&lc_webserver::handleClient, this, temp_thread);

    pool.push_back(temp_thread);

  }


  return EXIT_FAILURE;
}

