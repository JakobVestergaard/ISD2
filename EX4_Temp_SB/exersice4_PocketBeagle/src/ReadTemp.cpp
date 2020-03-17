/**
 * @File ReadTemp.cpp
 * @author Steffen Breinbjerg
 * Elektronik Ingeniør, Aarhus universitet Herning. 4th. Semester
 * @brief Implementation of ReadTemp.h
 * @copyright Free for everyone
 */
#include "ReadTemp.h"

using namespace std;
/** Global definition */
float current_temp;
int user_set_temp;

static void ADC_Read(int channel);
static void Set_Temp(int temp);

static void timer_handler(int signum) {

	if (signum == SIGALRM) {
		ADC_Read(0);
		syslog(LOG_INFO, "Current temp is: %f", current_temp);
	} else {
		syslog(LOG_ERR, "Error in Time event");
	}
}

static void SetTemp_handler(int signum) {

	if (signum == SIGINT) {
		Set_Temp(user_set_temp);
	} else {
		syslog(LOG_ERR, "Error in SetTemp Handler");
	}

}

void Read_Temp_Thread(int channel, int time_sec) {

	openlog("ADC_Measurement", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	struct itimerval timer;
	int ret;

	current_temp = -1;

	/** Enable timer signal. */
	if (signal(SIGALRM, timer_handler)) {
		syslog(LOG_ERR, "Failed to create handler for timer\nTerminating");
		exit(EXIT_FAILURE);
	}

	if (signal(SIGINT, SetTemp_handler)) {
		syslog(LOG_ERR, "Failed to create handler for Temp\nTerminating");
		exit(EXIT_FAILURE);
	}
	/** Configure the timer to expire after time_sec input */
	timer.it_value.tv_sec = time_sec;
	timer.it_value.tv_usec = 0;
	/** ... and every time_sec inpat after that. */
	timer.it_interval.tv_sec = time_sec;
	timer.it_interval.tv_usec = 0;

	/** Start a virtual timer. It counts down whenever this process is
	 executing. */
	ret = setitimer(ITIMER_REAL, &timer, NULL);
	if (ret) {
		syslog(LOG_NOTICE, "Could not install timer\n");
		exit(EXIT_FAILURE);
	}
	syslog(LOG_INFO, "Timer is now running");
	while (1) {
		usleep(1000);
	}

}

static void ADC_Read(int channel) {
	int adc_val;

	stringstream ss;
	ss << LDR_PATH << channel << "_raw";
	fstream fs;
	fs.open(ss.str().c_str(), fstream::in);
	fs >> adc_val;
	fs.close();
	// Calculate the temperature
	// There are 4096 steps in the ADC. Each step equals to 1.80/4096 volts
	float cur_voltage = adc_val * (1.80f / 4096.0f);

#ifdef LM35
	// The LM35DZ outputs 0V + 10mV/C in the range 0 C to 100 C
	// 0V equals 0C and 1.000V equals 100C
	// i.e. the cur_voltage*100 reflects the current temperature
	current_temp = cur_voltage * 100;
#endif
}

static void Set_Temp(int temp){
	/** Creating object - Pwm channel 1 - P1_36 */
	int procent = 0;
	PWM pwm(1);
	pwm.SetFreq(10000);
	procent = ((temp-LOW_TEMP)/(HIGH_TEMP-LOW_TEMP))*100;
	pwm.SetDutyCycleProcent(procent);
	pwm.EnablePwm();

}
