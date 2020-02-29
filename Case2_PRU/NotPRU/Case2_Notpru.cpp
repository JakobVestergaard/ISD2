//============================================================================
// Name        : Case2_PRU_Notpru.cpp
// Author      : Jakob
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<iostream>
#include<unistd.h> //for usleep
#include"GPIO.h"
using namespace exploringBB;
using namespace std;

int main() {
	GPIO outGPIO(112), inGPIO(14);

	// Setup GPIO to output and input
	outGPIO.setDirection(OUTPUT);
	inGPIO.setDirection(INPUT);

	while (1) {
		if (inGPIO.getValue()) {
			outGPIO.setValue(HIGH);
		} else {
			outGPIO.setValue(LOW);
		}
	}

	return 0;
}