//============================================================================
// Name        : Client_test.cpp
// Author      : Steffen Breinbjerg
// Version     : 1.0
// Copyright   : Free for everyone
// Description : Testing client program to connect to server.
//============================================================================

#include <iostream>
#include <string>
#include "TCPClient.h"

#define PORT "1955"
#define IP "192.168.7.2"

using namespace std;

int main() {
	string data;
	TCP_Client Client(PORT, IP);
	cout << "Enter Commands: " << endl << "get-temp, set-temp or stop" << endl;
	cin >> data;
	while (data != "stop") {
		Client.Send(data);
		/** Blocking call */
		data = Client.recieve(200);
		cout << "From TCP server: " << data << endl;
		cin >> data;
	}
	/** Sends the stop signal */
	Client.Send(data);
	Client.Exit();

	return 0;
}
