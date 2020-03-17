/**
 * @file TCPserver.cpp
 * @author Steffen Breinbjerg
 * Elektronik ingeniør, Aarhus universitet Herning
 * @details Implementation of TCP class.
 */

#include "TCPserver.h"

using namespace std;

void TCP_server::operator() (const char * port)  {

	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	/*hints is the struc to call in getaddrinfo.
	 * Which are filled with relevant information.
	 * p is just a pointer used for traversing the linked list.*/
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];
	int rv;
	int yes = 1;

	openlog("Exersice_2-TCP_SERVER", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	/** Typecast input int to char * to getadd */
	//char * port = (char *)&port_in;
	/** Init Hints with Zero's */
	memset(&hints, 0, sizeof hints);
	//AF_UNSPEC = Don't care if IPv4 og IPv6
	// AF_INET or AF_INET6 for specific
	hints.ai_family = AF_UNSPEC;
	//Sets Kinds of sockets - Stream = TCP
	hints.ai_socktype = SOCK_STREAM;
	//AI_PASSIVE tells getaddrinfo to assign my local adress to the sockets structs.
	hints.ai_flags = AI_PASSIVE;	// use my IP
	yes = 1;
	/* getaddrinfo -- Take 3 arguments -
	 * 1st Node = host name to connect to or IP adress
	 * 2nd Service = Can be a port or name of particular service eg. telnet
	 * 3rd struct addrinfo - The "Hint" struct filled with relevant information.
	 * Returns: Pointer to linked list in this example servinfo
	 * Return value non zero = no error else it prints the specific error using gai_strerror */
	if ((rv = getaddrinfo("192.168.7.2", port, &hints, &servinfo)) != 0) {
		syslog(LOG_NOTICE, gai_strerror(rv));
		exit(EXIT_FAILURE);
	}

	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next) {
		/* Sockets takes 3 arguments and return socket descriptor or -1 if error.
		 * 1st  int domain - IP version 4 or 6
		 * 2nd int type - Stream og datagram
		 * 3rd int protocal - TCP or UDP
		 * You can use the values from the result struct "servinfo"
		 * */
		//Checks for error in creating socket file descriptor
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("setsockopt");
			exit(1);
		}
		/* Bind takes 3 arguments
		 * 1st. Sockets descriptor
		 * 2nd.
		 * */
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL) {
		syslog(LOG_ERR, "Server: Failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		syslog(LOG_ERR, "Server: Listen\n");
		exit(1);
	}

	while (1) { // main accept() loop
		sin_size = sizeof their_addr;
		syslog(LOG_NOTICE, "Wating for connection");
		new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
		if (new_fd == -1) {
			syslog(LOG_ERR,"Error\n ");
			continue;
		}
		inet_ntop(their_addr.ss_family,
				get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
		syslog(LOG_NOTICE, "server: got connection from %s", s);
		Connection ConnectionObject;
		thread t(&Connection::Connection_thread, &ConnectionObject, new_fd);
		/** Detaching thread */
		if (t.joinable()) {
			t.detach();
			syslog(LOG_NOTICE,"Created new thread, and it's detached");
		}


	}
}

void * TCP_server::get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *) sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

