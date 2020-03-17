/*
 * TCPClient.cpp
 *
 *  Created on: 2 Mar 2020
 *      Author: Steffen
 */

#include "TCPClient.h"

namespace std {

TCP_Client::TCP_Client(const char * port, const  char *ip) {

	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	/** Init Hints with Zero's */
	memset(&hints, 0, sizeof hints);
	/**AF_UNSPEC = Don't care if IPv4 og IPv6 */
	/** AF_INET or AF_INET6 for specific */
	hints.ai_family = AF_UNSPEC;
	/** Sets Kinds of sockets - Stream = TCP */
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(ip, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		exit(EXIT_FAILURE);
	}
	/** loop through all the results and bind to the first we can */
	for (p = servinfo; p != NULL; p = p->ai_next) {
		/** Sockets takes 3 arguments and return socket descriptor or -1 if error.
		 * 1st  int domain - IP version 4 or 6
		 * 2nd int type - Stream og datagram
		 * 3rd int protocal - TCP or UDP
		 * You can use the values from the result struct "servinfo"
		 * */
		/**Checks for error in creating socket file descriptor */
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			printf("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			printf("client: connect");
			continue;
		}

		break;
	}
	/** Failed to connect to specified ip and port */
	if (p == NULL) {
		fprintf(stderr, "client: failed to connect (p==Null)\n");
		exit(EXIT_FAILURE);
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr), s,
			sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure

}

TCP_Client::~TCP_Client() {
	// TODO Auto-generated destructor stub
}

void *TCP_Client::get_in_addr(struct sockaddr *sa) {

	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

bool TCP_Client::Send(string data) {

	if (sockfd != -1) {
		if ((send(sockfd, data.c_str(), strlen(data.c_str()), 0)) < 0) {
			cout << "Send failed : " << data << endl;
			return false;
		}
	} else {
		cout << "Error: Fail in socket" << endl;
		return false;
	}

	return true;
}

string TCP_Client::receive(int size) {

	char buffer[size];
	memset(&buffer[0], 0, sizeof(buffer));

	string reply;
	/** Waits for message */
	if (recv(sockfd, buffer, size, 0) < 0) {
		cout << "receive failed!" << endl;
		return nullptr;
	}
	buffer[size - 1] = '\0';
	reply = buffer;
	return reply;

}


void TCP_Client::Exit(){

	close(sockfd);
}

} /* namespace std */

