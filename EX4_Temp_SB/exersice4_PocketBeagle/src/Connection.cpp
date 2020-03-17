/**
 * @File Connection.cpp
 * @author Steffen Breinbjerg
 * Elektronik Ingeniør, Aarhus universitet Herning. 4th. Semester
 * @brief Implementation of Connection.h
 * @copyright Free for everyone
 */
#include "Connection.h"

namespace std {

void Connection::Connection_thread(int socket) {
	char buffer[200];
	int nbytes;
	char msg2[] = "Insert temperature:";
	char msg3[] = "Did not understand that input";
	char msg4[] = "Terminating";
	/** Creating object to control PWM - Channel 1 - P1_36*/
	openlog("TCP_Client", LOG_CONS | LOG_PID | LOG_NDELAY,
	LOG_LOCAL1);
	/** Waits for incoming message */
	while (1) {
		if ((nbytes = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
			buffer[nbytes] = '\0';
			syslog(LOG_NOTICE, "JSON message received");
			if (strncmp(buffer, "get-temp", 200) == 0) {
				nbytes = sprintf(buffer,"%f",current_temp);
				send(socket, buffer, sizeof(nbytes), 0);
			} else if (strncmp(buffer, "set-temp", 200) == 0) {
				send(socket, msg2, sizeof(msg2), 0);
				recv(socket, buffer, sizeof(buffer),0);
				user_set_temp = atoi(buffer);
				raise(SIGINT);
				sprintf(buffer,"Temperature received: %d",user_set_temp);
				send(socket,buffer,sizeof(buffer),0);
			} else if (strncmp(buffer, "stop", 200) == 0) {
				send(socket, msg4, sizeof(msg4), 0);
				syslog(LOG_NOTICE, "Received terminating signal. Ending this client");
				break;
			} else {
				send(socket, msg3, sizeof(msg3), 0);
			}
		} else {
			syslog(LOG_ERR, "Error in recv call. Terminating");
			exit(1);
		}
	}
}

} /* namespace std */
