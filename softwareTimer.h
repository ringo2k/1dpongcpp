/*
 * softwareTimer.h
 *
 *  Created on: 31.07.2016
 *      Author: ringo
 */

#ifndef SOFTWARETIMER_H_
#define SOFTWARETIMER_H_
#include <avr/io.h>

enum WAITSTATES
{
	WAITING,
	TIMES_UP
};

class softwareTimer {
private:
	uint32_t timestamp;
	uint16_t waitTime;
public:
	softwareTimer();
	~softwareTimer();
	enum WAITSTATES wait(uint16_t waitTime);
};

#endif /* SOFTWARETIMER_H_ */
