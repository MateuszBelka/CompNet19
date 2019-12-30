#include "Client.h"
#include <iostream>
using namespace std;

int Client::readFromStdin(){
  string input;
  cin.ignore();
  getline(cin>>ws, input);
  input.append("\n");
  stdinBuffer.writeChars(input.c_str(), strlen(input.c_str()));
  return strlen(input.c_str());
}

int Client::readFromSocket() {
  int numbytes;
  try {
    memset(buff, 0, buffSize);
    numbytes = recv(Client::sock, buff, buffSize, 0);
    socketBuffer.writeChars(buff, numbytes);
    if (numbytes == -1) {
      throw runtime_error("while receiving from socket");
    }
    return numbytes;
  }
  catch(runtime_error &excpt){
    cout << "Error: " << excpt.what() << endl;
    stop = true;
    return -1;
  }
}

int Client::tick() {
  int numbytes;
  try {
    if (stdinBuffer.hasLine()) {
      readline = stdinBuffer.readLine();

      string username;
      string message_str;
      string startswith = readline.substr(0,1);

      if (startswith == "@") {
        size_t pos = readline.find("@");
        username = readline.substr(pos + 1, readline.find(' ') - 1);
        message_str = readline.substr(readline.find(' ') + 1);
        readline = "SEND " + username + " " + message_str;
      } else if (readline == "!who\n") {
        readline = "WHO\n";
      } else if (readline == "!exit\n") {
        return -1;
      }

      numbytes = send(Client::sock, readline.c_str(), strlen(readline.c_str()), 0);
      if (numbytes == -1) {
        throw runtime_error("while sending to socket");
      }
    }
    if (socketBuffer.hasLine()) {
      readline = socketBuffer.readLine();
      if (readline != "SEND-OK\n") {
        if (readline.substr(0,7) == "WHO-OK ") {
          readline = readline.substr(7);
          cout << "Server: ";
        } else if (readline.substr(0,9) == "DELIVERY ") {
          readline = readline.substr(9);
        } else {
          throw runtime_error("unexpected response from server");
        }
        cout << readline << endl;
      }
    }
  }

  catch(runtime_error &excpt){
    cout << "Error: " << excpt.what() << endl;
    stop = true;
    return -1;
  }
  stop = false;
  return 0;
}

void Client::createSocketAndLogIn()
{
  int n; //n stores the return value from getaddrinfo(), send() and recv()
  struct addrinfo hints, *results;
  const int bufferSize = 1024;
  char buffer[bufferSize];
  string buff;
  string username;
  bool goodAnswerServer = false;

  try {
    while (goodAnswerServer == false) {
      Client::sock = socket(AF_INET, SOCK_STREAM, 0); //protocol is 0 because it automatically changes based on the type
      if (sock < 0) {
        throw runtime_error("while opening the socket");
      }

      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = IPPROTO_TCP;

      n = getaddrinfo("52.58.97.202", "5378", &hints, &results);
      if (n > 0) {
        throw runtime_error("while getting IP address");
      }

      if (connect(Client::sock, results->ai_addr, results->ai_addrlen) < 0){
        throw runtime_error("while connecting");
      }
      cout << "Connection to the server 52.58.97.202 with port number 5378" << endl << endl;
      cout << "Type !exit to terminate the program" << endl;
      cout << "Enter the username: ";
      cin >> buff;
      if (buff == "!exit") {
        stop = true;
        return;
      }
      username = buff;
      buff = "HELLO-FROM " + username + "\n";
      Client::message = buff.c_str();
      n = send(Client::sock, Client::message, string(Client::message).length(), 0);
      if (n == -1){
        throw runtime_error("while sending to socket");
      }
      memset(buffer, 0, bufferSize); //clear array of char
      cout << "Server: ";
      n = recv(Client::sock, buffer, bufferSize, 0);
      if (n == -1){
        throw runtime_error("while receiving from socket");
      }
      buff = buffer;
      if (buff != buffer) {
        throw runtime_error("while converting from array of char to string");
      }
      cout << buff << endl;
      if (buff == "HELLO " + username + "\n") {
        goodAnswerServer = true;
      }
      cout << "type !exit to terminate the program" << endl;
      cout << "type: !who to see list of all currently logged-in users" << endl;
      cout << "type @username message to send a message" << endl;
    }
  }
  catch(runtime_error &excpt){
    cout << "Error: " << excpt.what() << endl;
    stop = true;
  }
}

void Client::closeSocket(){
  sock_close(Client::sock);
  sock_quit();
}
