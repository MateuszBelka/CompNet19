#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <vusocket.h>
#include <iostream>
#include <arpa/inet.h>
using namespace std;

//user-defined functions for error handling
//int sock_init() {}
//bool sock_valid(SOCKET sock) {}
//int sock_error_code() {} //retrieve error code
//int sock_close(SOCKET sock) {} //close connected socket
//int sock_quit() {} //clean uo resources allocated for working with socket

int main(int argc, char *argv[]) {

	SOCKET sock;
	int socket(int domain, int type, int protocol);
	int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);

	int sockfd, new_sockfd, client_len, n, port_n;
	char buffer[256];
	struct sock_addr, serv_addr, client_addr;

	if (argc < 2) {
      cout << "Error: no port provided.\n";
  }

	sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "Error in opening socket\n";
  	}
	bzero((char *) &serv_addr, size_t(serv_addr));
	port_n = atoi(argv[1]);
	serv_addr.ai_family = AF_INET;
	serv_addr.ai_socktype = SOCK_STREAM;
	serv_addr.ai_protocol = IPPROTO_TCP;
	serv_addr.ai_port = htons(8080);
	serv_addr.sockaddr.ai_addr = inet_addr("")
	if (bind(sockfd, (struct sockaddr *) &serv_addr, ssize_t(serv_addr)) == -1){
		perror("Error: binding");
		exit(1);
	}
	listen(sockfd, 5);
	client_len = ssize_t(client_addr);
	new_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
	if (new_sockfd < 0) {
		perror("Error: accept");
		exit(1);
	}
	bzero(buffer, 256);
	n = read(new_sockfd, buffer, 255);
	if (n < 0) {
		perror("Error: reading from socket.");
		exit(1);
	}
	cout << "Here is the message: %s/n" << buffer << endl;
	n = write(new_sockfd, "I got your message", 18);
	if (n < 0) {
		perror("Error: writing to socket");
		exit(1);
	}
	close(new_sockfd);
	close(sockfd)
	return 0;
}
}
