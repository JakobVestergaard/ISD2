/**
 *@file PWM.cpp
 *@brief Test PWM on pocket beagle in cpp.
 *@copyright Free for everyone
 *@date 18 November 2019
 *@author Steffen Breinbjerg
 *Aarhus Universitet, Birk Centerpark Herning.
 *Elektronik Ingeniør 3. Semester.
 */

#include "PWM.h"

PWM::PWM(int channel) {

	pwm_path = PATH;
	/**
	 * PWM freq = 1 Hz
	 */
	period = 1000000000;
	switch (channel) {
	case 1:
		pwm_path.append("0:0/");
		break;
	case 2:
		pwm_path.append("0:1/");
		break;
	case 3:
		pwm_path.append("2:0/");
		break;
	case 4:
		pwm_path.append("4:1/");
		break;
	default:
		syslog(LOG_ERR, "Wrong channel");
		break;
	}

}

PWM::~PWM() {
	// TODO Auto-generated destructor stub
}

bool PWM::SetDutyCycleProcent(float percent) {

	if (percent > 100 or percent < 0) {
		syslog(LOG_ERR, "Wrong procent");
		return false;
	}
	file.open(pwm_path + DUTY_CYCLE);
	if (!file.is_open()) {
		syslog(LOG_ERR, "Failed to open path + Duty cycle");
		return false;
	}
	unsigned int duty_cycle = period * (percent / 100);
	file << duty_cycle;
	file.close();
	if (file.is_open()) {
		syslog(LOG_ERR, "Failed to open path + period");
		return false;
	}
	return true;

}

bool PWM::SetFreq(float hz) {

	float period_f = 1.0f / hz;
	unsigned int period_d = (period_f * 1000000000);

	file.open(pwm_path + PERIOD);
	if (!file.is_open()) {
		syslog(LOG_ERR, "Failed to open path + period");
		return false;
	}
	file << period_d;
	file.close();
	if (file.is_open()) {
		syslog(LOG_ERR, "Failed to open path + period");
		return false;
	}
	period = period_d;
	return true;
}

bool PWM::EnablePwm() {

	file.open(pwm_path + ENABLE);
	if (!file.is_open()) {
		syslog(LOG_ERR, "Failed to open path + enable");
		return false;
	}
	file << "1";
	file.close();
	if (file.is_open()) {
		syslog(LOG_ERR, "Failed to open path + enable");
		return false;
	}
	return true;
}

bool PWM::StopPwm() {

	file.open(pwm_path + ENABLE);
	if (!file.is_open()) {
		syslog(LOG_ERR, "Failed to open path + enable");
		return false;
	}
	file << "0";
	file.close();
	if (file.is_open()) {
		syslog(LOG_ERR, "Failed to open path + enable");
		return false;
	}
	return true;

}

bool PWM::SetDutyCycleMS(float ms) {

	unsigned int nanosec = ms * 1000000;
	if (period < nanosec) {
		syslog(LOG_ERR, "Longer duty than period");
		return false;
	}
	file.open(pwm_path + DUTY_CYCLE);
	if (!file.is_open()) {
		syslog(LOG_ERR, "Failed to open path + Duty cycle");
		return false;
	}
	file << nanosec;
	file.close();
	if (file.is_open()) {
		syslog(LOG_ERR, "Failed to open path + Duty cycle");
		return false;
	}

	return true;
}

