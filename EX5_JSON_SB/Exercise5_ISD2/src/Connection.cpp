/*
 * Connection.cpp
 *
 *  Created on: 14 Feb 2020
 *      Author: Steffen
 */

#include "Connection.h"


void Connection::Connection_thread(int socket) {
	int nbytes;
	char buffer[200];
	string data;
	JSONRPC JSON;

	openlog("TCP_Client", LOG_CONS | LOG_PID | LOG_NDELAY,
		LOG_LOCAL1);

		while(1){
			/** Blockling statement - Waits for incoming message */
			if ((nbytes = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
				buffer[nbytes] = '\0';
				syslog(LOG_NOTICE, "JSON message received");
				data = buffer;
				data = JSON.RPC_Receive(data);
				syslog(LOG_NOTICE, "JSON: Method received %s",data.c_str());
				if(data == "gettemp"){
					/** Convert float to string type */
					data = to_string(current_temp);
					/** Convert data string to JSON-string*/
					data = JSON.RPC_Answer(data);
					/** Send result back in JSON format*/
					send(socket,&data,data.size(),0);

				}
				else if(data =="stop"){
					syslog(LOG_NOTICE, "Stoping Client");
					break;
				}
				else{
					syslog(LOG_NOTICE,"Invalid input: %s",data.c_str());
					data = "invalid";
					data = JSON.RPC_Answer(data);
					send(socket,&data,data.size(),0);
				}
			}


		}


}




