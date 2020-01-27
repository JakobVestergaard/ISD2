/*
 ============================================================================
 Name        : sockets_threads.c
 Author      : Steffen Breinbjerg
 Version     :  1.2
 Copyright   : FFE
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#define PORT "1955"
#define BACKLOG 3
pthread_mutex_t mutex;

//Current connections to the server
int connections = 0;
//get sockaddr
void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in *) sa)->sin_addr);
	}
	return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}
void communication(void *fd);

int main(void) {

	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	/*hints is the struc to call in getaddrinfo.
	 * Which are filled with relevant information.
	 * p is just a pointer used for traversing the linked list.*/
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	int yes = 1;
	char s[INET6_ADDRSTRLEN];
	int rv, rc;
	int threadId;
	//intialize the mutex.
	pthread_mutex_init(&mutex, NULL);
	//To make sure the struct is empty
	memset(&hints, 0, sizeof hints);
	//AF_UNSPEC = Don't care if IPv4 og IPv6
	// AF_INET or AF_INET6 for specific
	hints.ai_family = AF_UNSPEC;
	//Sets Kinds of sockets - Stream = TCP
	hints.ai_socktype = SOCK_STREAM;
	//AI_PASSIVE tells getaddrinfo to assign my local adress to the sockets structs.
	hints.ai_flags = AI_PASSIVE; // use my IP
	/* getaddrinfo -- Take 3 arguments -
	 * 1st Node = host name to connect to or IP adress
	 * 2nd Service = Can be a port or name of particular service eg. telnet
	 * 3rd struct addrinfo - The "Hint" struct filled with relevant information.
	 * Returns: Pointer to linked list in this example servinfo
	 * Return value non zero = no error else it prints the specific error using gai_strerror */
	if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
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

	//This struct is only used to load the linked list to.
	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while (1) { // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
		inet_ntop(their_addr.ss_family,
				get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
		printf("server: got connection from %s\n", s);
		rc = pthread_create(&threadId, NULL, communication, (void *)new_fd);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}

	}

	return EXIT_SUCCESS;
}

void communication(void *fd) {

	//Typecast the void argument back to int.
	int socket = (int)fd;
	char buffer[512];
	int keep_go = 1;
	int firstTime = 0;

	while (keep_go) {

		if ((pthread_mutex_lock(&mutex)) == 0) {
			if (connections < 2) {
				//Addind one more client to the server
				connections++;
				//Free the mutex again
				pthread_mutex_unlock(&mutex);
				//Send msg to client that you are connected
				char msg[] = "Server is ready.\n";
				send(socket, msg, sizeof(msg), 0);
				while (keep_go) {
					int r;
					if ((r = recv(socket, buffer, sizeof(buffer), 0)) > 0) {

						if (buffer[0] == 0x04) {	//CTRL+d //EOF
							keep_go = 0;
							break;
						}
						//Send back what recieved - Remember the size is r, which are generated from the recv call.
						if (send(socket, buffer, r, 0) != r) {
							break;
						}

					} else {
						break;
					}

				}
			} else {
				//If there are more connections than allowed
				pthread_mutex_unlock(&mutex);
				if (firstTime) {
					//To save CPU
					usleep(1000);
				} else {
					//Send msg to client to tell you are in queue.
					//Increment the firstTime int, so next time if will go in sleep
					char msg2[] =
							"You are now in queue. Please wait until you are connected.\n";
					send(socket, msg2, sizeof(msg2), 0);
					firstTime = 1;
				}
			}
		} else {
			exit(EXIT_FAILURE);
		}

	}
	close(socket);
	//Decrement the current count on connection to allow a new.
	if (pthread_mutex_lock(&mutex) == 0) {
		connections--;
		pthread_mutex_unlock(&mutex);
	} else {
		exit(EXIT_FAILURE);
	}

	pthread_exit(NULL);
}

