/*
 * main.cpp
 *
 *  Created on: 28.07.2016
 *      Author: ringo
 */


#include "hardwareTimer.h"
#include "softwareTimer.h"
#include "pongGame.h"

#include <avr/io.h>
#include <avr/delay.h>


int main()
{

	hardwareTimer hwTimer;
	softwareTimer ledBlinkTimer;
	pongGame game;
	unsigned char i = 0;

	DDRD |= ( 1 << PD0);
	PORTD |= ( 1 << PD2) | ( 1 << PD3);


    game.prepare();
	while(1)
	{

			if (ledBlinkTimer.wait(2000)== TIMES_UP)
			{
				i++;
				//game.sound.playSound(MATCH_OVERSOUND);
				if (!( i %2))
				{
					//game.prepare(UP);
				} else {
					//game.prepare(DOWN);
				}
			}


		game.run();

	}
	return 0;
}
