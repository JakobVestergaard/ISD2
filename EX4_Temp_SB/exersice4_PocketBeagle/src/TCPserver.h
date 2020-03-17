/**
 * @file TCPserver.h
 * @author Steffen Breinbjerg
 * @date 03-02-2020
 *  @brief Header for TCP Server Class
 *  See Source file for implementation.
 *@copyright Free for everyone
 *
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include "errno.h"
#include "syslog.h"
#include <thread>
#include "Connection.h"


#define BACKLOG 3

using namespace std;

#define MAXPACKETSIZE 4096

class TCP_server {
public:

	void operator() (const char * port);
private:
	void * get_in_addr(struct sockaddr *sa);

};

#endif /* TCPSERVER_H_ */
