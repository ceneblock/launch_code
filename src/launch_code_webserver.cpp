#include "launch_code_webserver.h"

/**
 * @brief returns the mime type for a given file
 * @input location the path to the file
 * @return the corresponding MIME type
 */
string lc_webserver::getMagic(string location)
{
  string mime = "";
  magic_t magic;
  magic = magic_open(MAGIC_MIME_TYPE); 
  magic_load(magic, NULL);
  magic_compile(magic, NULL);
  mime = magic_file(magic, location.c_str());
  magic_close(magic);
  return mime;
}

/**
 * @brief handles when we recieve a GET
 * @param client_FD who to talk back to
 * @param PARAMETER what to serve up.
 */
void lc_webserver::handleGET(unsigned int client_FD, string PARAMETER)
{
  unsigned long bytes_available = 0;
  string message = "HTTP/1.1 200 OK\n"
    "Server: PACKAGE\n"
    "Connection: close\n"
    "Content-Type: ";

  PARAMETER = "." + PARAMETER;
  int content_fd = open(PARAMETER.c_str(), O_RDONLY);
  if(content_fd == -1)
  {
    //Something bad happened. Just gonna abort!
    cerr << "Unable to open " << PARAMETER << endl;
    cerr << "Aborting this client\n";
    handleUNKNOWN(client_FD);
  }
  else
  {
    if((ioctl(content_fd,FIONREAD,&bytes_available)) == -1)
    {
      cerr << "Error in ioctl\n";
      handleUNKNOWN(client_FD);
    }
    else
    {
      char data[bytes_available];
      ssize_t amount_read = read(content_fd, data, bytes_available);
      if(amount_read != bytes_available)
      {
        cerr << "Had a problem reading the whole file\n";
        handleUNKNOWN(client_FD);
      }
      else
      {
        message = message + getMagic(PARAMETER) + "\n\n";
        message = message + data + "\n";
        send(client_FD, message.c_str(), message.length(), 0);
        close(client_FD);
        close(content_fd);
      }
    }
  }
}

/**
 * @brief handles when we recieve something weird
 * @param client_FD who to talk back to
*/
void lc_webserver::handleUNKNOWN(unsigned int client_FD)
{
  string message = "HTTP/1.0 405 Method Not Allowed\n"
    "Server: PACKAGE\n"
    "Connection: close\n"
    "Content-Type: text/html\n"
    "\n"
    "Error 405 Method Not Allowed\n\n";

  send(client_FD, message.c_str(), message.length(), 0);
  close(client_FD);
}

/**
 * @brief handles client's data
 * @param client_FD the FD to the client
 * @param data what the request way that was sent to us
 */
void lc_webserver::handleData(unsigned int client_FD, string data)
{
  char *token;
  string METHOD;
  string PARAMETER;

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
    PARAMETER = token;
  }
  else
  {
    //handle error
  }

  if(METHOD == "GET")
  {
    handleGET(client_FD, PARAMETER);
  }
  else
  {
    handleUNKNOWN(client_FD);
  }
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
      break;
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

