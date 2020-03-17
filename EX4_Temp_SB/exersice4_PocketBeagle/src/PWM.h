/**
 *@file PWM.h
 *@brief Test PWM on pocket beagle in cpp.
 *@copyright Free for everyone
 *@date 18 November 2019
 *@author Steffen Breinbjerg
 *Aarhus Universitet, Birk Centerpark Herning.
 *Elektronik Ingeniør 3. Semester.
 *@details PocketBeagle got 6 PWM channels.
 *@details To init one channel enter channel name, when you create the class.
 *@details Channels: 1 = PWM0A, 2 = PWM0B, 3 = PWM1A, 6 = PWM2B
 *@details See pinmap for Pocketbeagle.
 */

#ifndef PWM_H_
#define PWM_H_

/**
 * 		Includes
 */
//#include <iostream>
#include <string>
#include <fstream>
#include "stdbool.h"

using namespace std;

/**
 * 		Definitions
 */
#define PATH "/sys/class/pwm/pwm-"
//#define SCRIPT_PATH "~/Program/scripts/PWM.sh"
#define CH1 "p1_36"
#define CH2 "p1_33"
#define CH3 "p2_01"
#define CH4 "p2_03"
#define ENABLE	"enable"
#define DUTY_CYCLE "duty_cycle"
#define	PERIOD	"period"


class PWM {
public:
	/**
	 * @param channel - Integer value
	 * @brief See description for channels.
	 */
	PWM(int channel);
	virtual ~PWM();
	virtual bool SetDutyCycleProcent(float percent);
	virtual bool SetFreq(float hz);
	virtual bool EnablePwm();
	virtual bool StopPwm();
	virtual bool SetDutyCycleMS(float ms);

private:
	string pwm_path;
	fstream file;
	unsigned int period;

};


#endif /* PWM_H_ */
