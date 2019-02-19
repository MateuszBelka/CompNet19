#include "Client.h"
/*
#include <stdio.h> //sizeof(), fgets()
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h> //later for fork()
#include <netinet/in.h>
#include <arpa/inet.h> //inet_addr()
#include <strings.h> //bzero()
#include <string.h> //strlen()
#include <string>
*/
using namespace std;

void Client::tick(){}

int Client::readFromStdin(){}

int Client::readFromSocket(){}

void Client::createSocketAndLogIn()
{
  int sockfd, n; //n stores the return value from getaddrinfo(), send() and recv()
  struct addrinfo serv_info, *results;
  string portno = "5378";
  const int buffersize = 256;
  char buffer[buffersize];

  sockfd = socket(AF_INET, SOCK_STREAM, 0); //protocol is 0 because it automatically changes based on the type
  if (sockfd < 0) {
    cout << "error while opening the socket" << endl;
    exit(1);
  }

  serv_info.ai_family = AF_INET;
  serv_info.ai_socktype = SOCK_STREAM;
  serv_info.ai_protocol = IPPROTO_TCP;
  n = getaddrinfo("52.58.97.202", "5378", &serv_info, &results);
  if (n > 0) {
    cout << "error while getting IP address" << endl;
    exit(1);
  }

  if (connect(sockfd, results->ai_addr, results->ai_addrlen) < 0){
    cout << "error while connecting" << endl;
    exit(1);
  }
  cout << "Connection to the server 52.58.97.202 with port number " << portno << endl;

  //communicate (send and receive) with server (login)
  cout << "Client ";
  cin >> buffer;
  cout << endl;
  n = send(sockfd, buffer, buffersize, 0);
  if (n == -1){
    cout << "error while sending to socket" << endl;
    exit(1);
  }
  cout << "Server: ";
  n = recv(sockfd, buffer, buffersize, 0);
  if (n == -1){
    cout << "error while receiving from socket" << endl;
    exit(1);
  }
  cout << buffer << endl;
  cout << "Connection terminated." << endl;
  close(sockfd);
  freeaddrinfo(results);
}

void Client::closeSocket(){}
