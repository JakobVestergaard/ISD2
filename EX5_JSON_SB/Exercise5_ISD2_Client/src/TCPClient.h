/*
 * TCPClient.h
 *
 *  Created on: 2 Mar 2020
 *      Author: Steffen
 */

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

/** Includes	*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>

#define MAXDATASIZE 100

namespace std {

class TCP_Client {
public:
	TCP_Client(const char * port, const char * ip);
	bool Send(string data);
	string receive(int size);
	void Exit();
	virtual ~TCP_Client();
private:
	void * get_in_addr(struct sockaddr *sa);
	int sockfd;
	char buf[MAXDATASIZE];
};

} /* namespace std */

#endif /* TCPCLIENT_H_ */
