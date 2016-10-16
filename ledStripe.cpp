/*
 * ledStripe.cpp
 *
 *  Created on: 01.08.2016
 *      Author: ringo
 */

#include <avr/io.h>
#include "ledStripe.h"
#include <avr/interrupt.h>
#include <avr/delay.h>

extern "C" void ledstripesendbyte(char* data);
volatile playground pg;

ledStripe::ledStripe() {

	DDRD |= ( 1 << PD1);
	PORTD &= ~( 1 << PD1);
	clear();

}

ledStripe::~ledStripe() {
	// TODO Auto-generated destructor stub
}

void ledStripe::run() {

	if (updateTimer.wait(1) == TIMES_UP)
	{
		cli();
		ledstripesendbyte((char*) pg.lightPoints);
		sei();
		_delay_us(100);

	}
}

void ledStripe::clear()
{
	ligthpoint off;
	off.b = 0;
	off.r = 0;
	off.g = 0;
	setAllLigthPoints(off);
}

void ledStripe::setLightPoint(unsigned char ligthIndex, ligthpoint newPoint) {

	if (ligthIndex < NUMBEROFLIGHTPOINTS)
	{
		pg.lightPoints[ligthIndex].b = newPoint.b;
		pg.lightPoints[ligthIndex].r = newPoint.r;
		pg.lightPoints[ligthIndex].g = newPoint.g;
	}


}

void ledStripe::setAllLigthPoints(ligthpoint lp)
{
	for (int i = 0; i < NUMBEROFLIGHTPOINTS; i++)
	{
		setLightPoint(i,lp);
	}
}
