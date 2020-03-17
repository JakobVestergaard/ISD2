//============================================================================
// Name        : TCP client.
// Author      : Steffen Breinbjerg
// Version     : 1.0
// Copyright   : Free for everyone
// Description : TCP-client with JSON-RPC
//============================================================================

#include <iostream>
#include "JSONRPC.h"
#include "TCPClient.h"

/** Define connection info */
#define PORT "1955"
#define IP "192.168.7.2"

using namespace std;

int main() {
	/** Create TCP-Client object */
	TCP_Client Beagle_server(PORT, IP);
	/** Creating JSON object to handle JSON Format */
	JSONRPC JSON;
	string data;
	cout << "Enter Commands: " << endl << "gettemp or stop" << endl;
	cin >> data;
	while (data != "stop") {
		/** Input method. Returns JSON string */
		data = JSON.RPC_Call(data);
		/** Send JSON string to server */
		Beagle_server.Send(data);
		/** Blocking function. Waits for received message from server
		 * Return received message as string object*/
		data = Beagle_server.receive(500);
		/** Convert JSON string to result */
		data = JSON.RPC_Result(data);
		cout << "Result from Server: " << data << endl;
		/** New command */
		cin >> data;
	}
	/** Input method. Returns JSON string
	 * Send Stop message to Server*/
	data = JSON.RPC_Call(data);
	/** Send JSON string to server */
	Beagle_server.Send(data);

	return 0;
}
