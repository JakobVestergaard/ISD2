/*
 * Connection.h
 *
 *  Created on: 14 Feb 2020
 *      Author: Steffen
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "unistd.h"
#include "stdlib.h"
#include "syslog.h"
#include "sys/socket.h"
#include "string.h"
#include "ReadTemp.h"
#include "JSONRPC.h"

/** Global value defined in ReadTemp.h */
extern float current_temp;

using namespace std;

class Connection {
public:
	void Connection_thread(int socket);
	Connection (){}
	virtual ~Connection() {}
};






#endif /* CONNECTION_H_ */
