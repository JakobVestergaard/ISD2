//============================================================================
// Name        : exersice4_PocketBeagle.cpp
// Author      : Steffen Breinbjerg
// Version     : 1.0
// Copyright   : Free for everyone
// Description : TCP Server.
//============================================================================

#include <iostream>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <syslog.h>
#include <stdlib.h>
#include "Daemon.h"
#include <unistd.h>
#include "TCPserver.h"
#include "pthread.h"
#include "ReadTemp.h"
#include <thread>


using namespace std;

#define PORT "1955"
/**  For Saving TCP_Server ID
 *	For pthread_cancel */
long unsigned int Tcp_ID;
/** Saving Temp thread id */
long unsigned int Temp_ID;


void Terminate(int signum) {
	if (signum == SIGHUP) {
		syslog(LOG_NOTICE, "Program received a termination signal");
		pthread_cancel(Tcp_ID);
		pthread_cancel(Temp_ID);
	}
}

int main() {

	daemonize();

	openlog("Exercise_4", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

	syslog(LOG_NOTICE, "Program started by User %d", getuid());
	syslog(LOG_NOTICE, "Standard file descriptors closed.");

	/** Enable signal handlers for Terminating the program and timer
	 * to measure temp sensor */

	if (signal(SIGHUP, Terminate)) {
		syslog(LOG_ERR, "Failed to create handler for SIGHUP\nTerminating");
		exit(EXIT_FAILURE);
	}
	/**
	 * Creating thread to read temperature.
	 * Param: ADC channel and time between each measurement
	 * in seconds.
	 */
	thread Temp(Read_Temp_Thread,0,5);
	Temp_ID = Temp.native_handle();
	/** Starting TCP server
	 * PORT must be const char * therefore use #define */
	thread TCP(TCP_server(), PORT ) ;
	/** Thread ID
	 * Return unsigned long int instead of thread::id type.
	 * To use pthread_cancel(unsigned long int id)
	 */
	Tcp_ID = TCP.native_handle();
	if(TCP.joinable() && Temp.joinable()){
		syslog(LOG_NOTICE,"TCP server started");
		/** This thread is now blocked. Waiting for the TCP and Temp
		 * to stop
		 */
		TCP.join();
		Temp.join();

	}
	syslog(LOG_NOTICE,"Threads was terminating successful. Bye");

	return 0;
}
