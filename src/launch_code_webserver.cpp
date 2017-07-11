#include "launch_code_webserver.h"

/**
 * @brief listens and accepts new connections. Once accepted, a new thread is
 * spun off
 * @return if there was an error.
*/
int lc_webserver::listen()
{
  int return_status = EXIT_SUCCESS;
  int new_socket_desc;
  socklen_t client_addr_len;

  int socket_desc = socket(AF_INET, SOCK_STREAM,0);

  sockaddr_in srv_addr, *cli_addr;

  if(socket_desc == -1)
  {
    cerr << "Unable to create new sockets!\n";
    cerr << "Aborting\!";
    return_status = EXIT_FAILURE;
  }

  int yes = 1;
  if(setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
  {
    cerr << "Unable to set reuseable options :-\\\n";
  }
  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);

  if(bind(socket_desc, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
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
    temp_thread.new_thread = new thread(network::handleClient,  &temp_thread);

    pool.push_back(temp_thread);

  }


  return EXIT_FAILURE;
}

