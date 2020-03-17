/*
 * ReadTemp.h
 *
 *  Created on: 5 Mar 2020
 *      Author: Steffen
 */

#ifndef READTEMP_H_
#define READTEMP_H_


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <syslog.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

/** Defines 		*/
#define LDR_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"
#define LM35 1
#define LOW_TEMP 22
#define HIGH_TEMP 38.5

void Read_Temp_Thread(int channel, int time_sec);





#endif /* READTEMP_H_ */
