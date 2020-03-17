/**
 * @File Daemon.h
 * @author Steffen Breinbjerg
 * Elektronik Ingeniør, Aarhus universitet Herning. 4th. Semester
 * @date 07-02-2020
 * @brief For deamonizing the program.
 * @copyright Free for everyone
 * @details
 *
 */



#ifndef DAEMON_H_
#define DAEMON_H_


#include <unistd.h>
#include <sys/stat.h>
#include "syslog.h"
#include "stdlib.h"
#include <sys/types.h>



int daemonize();

#endif /* DAEMON_H_ */
