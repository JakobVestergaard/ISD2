/**
 * @File Connection.h
 * @author Steffen Breinbjerg
 * Elektronik Ingeniør, Aarhus universitet Herning. 4th. Semester
 * @brief Connection class. To handle clients. 
 * @copyright Free for everyone
 * @details	This class i made to handle TCP clients
 * connecting to the server. 
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "unistd.h"
#include "stdlib.h"
#include "syslog.h"
#include "sys/socket.h"
#include "string.h"
#include "ReadTemp.h"
#include "PWM.h"

extern float current_temp;
extern int user_set_temp;

namespace std {

class Connection {
public:
	void Connection_thread(int socket);
	Connection (){}
	virtual ~Connection() {}
};





} /* namespace std */

#endif /* CONNECTION_H_ */
