/*
 * hardwareTimer.h
 *
 *  Created on: 31.07.2016
 *      Author: ringo
 */

#ifndef HARDWARETIMER_H_
#define HARDWARETIMER_H_
#include <avr/io.h>
#define TICKSPERMILLISECOND 1 // Prescale 1024@16MHz

class hardwareTimer {
public:
	hardwareTimer();
	~hardwareTimer();

	static uint32_t getTimerValue();
};

#endif /* HARDWARETIMER_H_ */
