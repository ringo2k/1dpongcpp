/*
 * softwareTimer.cpp
 *
 *  Created on: 31.07.2016
 *      Author: ringo
 */

#include "softwareTimer.h"
#include "hardwareTimer.h"


softwareTimer::softwareTimer() {
	waitTime = 0;
	timestamp = 0;

}

softwareTimer::~softwareTimer() {
	// TODO Auto-generated destructor stub
}

enum WAITSTATES softwareTimer::wait(uint16_t newWaitTime) {


	if (this->waitTime == 0)
	{
		waitTime = newWaitTime;
		timestamp = hardwareTimer::getTimerValue();
		return WAITING;
	} else {

		if (hardwareTimer::getTimerValue() >= (this->timestamp + this->waitTime))
		{
			this->waitTime = 0;
			return TIMES_UP;
		}
	}

	return WAITING;

}
