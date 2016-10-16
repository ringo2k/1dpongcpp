/*
 * hardwareTimer.cpp
 *
 *  Created on: 31.07.2016
 *      Author: ringo
 */

#include "hardwareTimer.h"
#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint32_t hw_counter = 0;

ISR (TIMER1_OVF_vect)
{
	//PORTD ^= ( 1 << PD0);
	hw_counter++;
}


hardwareTimer::hardwareTimer()
{
	// init a timer
	TCCR1B |= ( 1 << CS10) | ( 1 << WGM13); // just prescale 1024 and let it run
	TIMSK1 |= ( 1 << TOIE1);
	ICR1 = 8000;
	sei();

}

hardwareTimer::~hardwareTimer() {
}

uint32_t hardwareTimer::getTimerValue()
{
	return hw_counter;
}
