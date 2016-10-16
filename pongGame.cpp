/*
 * pongGame.cpp
 *
 *  Created on: 02.08.2016
 *      Author: ringo
 */

#include "pongGame.h"
#include <avr/io.h>
#include <avr/interrupt.h>


player players[NUMEROFPLAYERS];
ligthpoint red;

ISR(INT0_vect)
{
	unsigned int count = 0;
    for (count = 0; count < 1e3; count++)
    {
    	if (PIND &= ( 1 << PD2))
    	{
    		return; //debounce
    	}
    }
	players[UP].buttonPressed = true;
	players[UP].buttonReset = 0;
	EIMSK &= ~(1 << INT0);
}

ISR(INT1_vect)
{
	unsigned int count = 0;
    for (count = 0; count < 1e3; count++)
    {
    	if (PIND &= ( 1 << PD3))
    	{
    		return; //debounce
    	}
    }
	players[DOWN].buttonPressed = true;
	players[DOWN].buttonReset = 0;
	EIMSK &= ~(1 << INT1);
}

pongGame::pongGame() {
	reset();
	resetPoints();
	sound.playSound(STARTSOUND);
	mode = HITSPEED;//LINEARSPEED;

	players[UP].color.b = 160;
	players[UP].color.r = 20;
	players[UP].color.g = 20;

	players[UP].baseColor.b = 16;
	players[UP].baseColor.r = 2;
	players[UP].baseColor.g = 2;

	players[UP].pointJingle = POINT1SOUND;


	players[DOWN].color.b = 20;
	players[DOWN].color.r = 20;
	players[DOWN].color.g = 160;

	players[DOWN].baseColor.b = 2;
	players[DOWN].baseColor.r = 2;
	players[DOWN].baseColor.g = 16;

	players[DOWN].pointJingle = POINT2SOUND;

	// init Buttons
	EICRA |= ( 1 << ISC11) | ( 1 << ISC01); // falling edge interrupt
	EIMSK |= ( 1 << INT0) | ( 1 << INT1) ;
	PORTD |= ( 1 << PD2) | ( 1 << PD3); //activate pullups

	players[DOWN].buttonPressed = false;
	players[DOWN].buttonReset = 0;

	players[UP].buttonPressed = false;
	players[UP].buttonReset = 0;

	red.b = 0;
	red.g = 0;
	red.r = 250;


}

pongGame::~pongGame() {
	// TODO Auto-generated destructor stub
}

void pongGame::updatePointsOnLedStripe()
{
	// show the points of the players
	for (int i = 0; i < NUMEROFPLAYERS; i++)
	{
		uint8_t ledSpace = 2;
		if (players[i].points == 3)
		{
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace)) -(0 *ledSpace),players[i].color);
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace)) -(1 *ledSpace),players[i].color);
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace)) -(2 *ledSpace),players[i].color);
		} else if ((players[i].points == 2))
		{
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace))-(0 *ledSpace),red);
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace))-(1 *ledSpace),players[i].color);
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace))-(2 *ledSpace),players[i].color);
		} else if ((players[i].points == 1))
		{
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace))- (0 *ledSpace),red);
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace))- (1 *ledSpace),red);
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace))-(2 *ledSpace),players[i].color);
		} else if ((players[i].points == 0))
		{
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace))-(0 *ledSpace),red);
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace))-(1 *ledSpace),red);
			leds.setLightPoint(NUMBEROFLIGHTPOINTS-1 - (i*(3*ledSpace))-(2 *ledSpace),red);
		}

	}
}

void pongGame::run()
{

	switch (status)
	{

	case WAIT_FOR_GAMESTART:
		//fancy blinking till start
		if (blinkTimer.wait(100) == TIMES_UP)
		{
			movingPoint.r =  ~movingPoint.r;
			//movingPoint.g = ~movingPoint.g;
			//movingPoint.b = ~movingPoint.b;
		}

		// show points
		updatePointsOnLedStripe();

		// show area
		for (int i = 0; i < NUMBEROFBASELEDS; i++)
		{
			this->leds.setLightPoint(i, players[UP].color);
		}

		for (int i = 0; i < NUMBEROFBASELEDS; i++)
		{
			this->leds.setLightPoint(NUMBEROFGAMEAREALED - 1 - i, players[DOWN].color);
		}

		// wait till start
		if (checkButton(&players[this->dir]))
		{
			this->movingPoint = players[this->dir].color;
			start();
		}

		break;

	case RUNNING:

		if (speedTimer.wait(speed) == TIMES_UP)
		{
			leds.clear();
			leds.setLightPoint(NUMBEROFBASELEDS - 1, players[UP].baseColor);
			leds.setLightPoint(NUMBEROFGAMEAREALED - NUMBEROFBASELEDS, players[DOWN].baseColor);

			// show points
			updatePointsOnLedStripe();

			// move point around
			if ( this->dir == UP)
			{
				// UP TURN
				if (checkButton(&players[UP]))
				{
					// player pressed button but it was not his turn, idiot!
					pointFor(DOWN);
				}

				if (!checkButton(&players[DOWN]))
				{
					this->actualPoint++;
					if (actualPoint >= NUMBEROFGAMEAREALED)
					{
						pointFor(UP);
						actualPoint = NUMBEROFGAMEAREALED - 1;
					}
				} else {
					// button from Opponnent was pressed!
					if (( actualPoint >= (NUMBEROFGAMEAREALED - NUMBEROFBASELEDS)))
					{
						uint8_t hitPosition = NUMBEROFGAMEAREALED - actualPoint;
						//valid hit
						this->dir = DOWN;
						sound.playSound(HITSOUND);
						increaseSpeed(hitPosition, DOWN);

					} else {
						pointFor(UP);
					}
				}

			} else {
				// DOWN TURN (green)

				if (checkButton(&players[DOWN]))
				{
					// player pressed button but it was not his turn, idiot!
					pointFor(UP);
				}
				if (checkButton(&players[UP]) == false)
				{

					this->actualPoint--;
					if (actualPoint < 0)
					{
						pointFor(DOWN);
						actualPoint = 0;
					}
				} else {
					// button was pressed!
					if (( actualPoint > 0) && (actualPoint < NUMBEROFBASELEDS))
					{
						uint8_t hitPosition = actualPoint;
						//valid hit
						this->dir = UP;
                        increaseSpeed(hitPosition,UP);
						sound.playSound(HITSOUND);

					} else {
						pointFor(DOWN);
					}
				}
			}
		}
		break;

	case POINT:

		if (blinkTimer.wait(100) == TIMES_UP)
		{
			movingPoint.b = ~movingPoint.b;
			movingPoint.r = ~movingPoint.r;
			movingPoint.g = ~movingPoint.g;
		}

		if ((checkButton(&players[DOWN]) == true))
		{
			prepare();
		}

		if (checkButton(&players[UP]) == true)
		{
			prepare();
		}


		break;

	case MATCH_OVER:

		if (blinkTimer.wait(100) == TIMES_UP)
		{
			movingPoint.b = ~movingPoint.b;
			movingPoint.r = ~movingPoint.r;
			movingPoint.g = ~movingPoint.g;
		}

		if ((checkButton(&players[DOWN]) == true))
		{
            resetPoints();
			prepare();
		}

		if (checkButton(&players[UP]) == true)
		{
			resetPoints();
			prepare();
		}


		break;
	default:
		break;
	}


	// update
	leds.setLightPoint(actualPoint, movingPoint);
	this->leds.run();
	this->sound.run();

	if ((PIND & ( 1 << PD2)))
	{
		// cool down, debounce
		if (players[UP].buttonReset > 200 )
		{
			//reactivate int0
			EIFR |= ( 1 << INT0);
			EIMSK |= (1 << INT0);

		} else {
			players[UP].buttonReset++;
		}
	}

	if ((PIND & ( 1 << PD3)))
	{
		// cool down, debounce
		if (players[DOWN].buttonReset > 200 )
		{
			//reactivate int0
			EIFR |= ( 1 << INT1);
			EIMSK |= (1 << INT1);

		} else {
			players[DOWN].buttonReset++;
		}
	}


}

void pongGame::reset()
{
	leds.clear();

	speed = DEFAULT_SPEED;
	players[UP].color.b = 160;
	players[UP].color.r = 20;
	players[UP].color.g = 20;

	players[UP].baseColor.b = 16;
	players[UP].baseColor.r = 2;
	players[UP].baseColor.g = 2;

	players[UP].pointJingle = POINT1SOUND;


	players[DOWN].color.b = 20;
	players[DOWN].color.r = 20;
	players[DOWN].color.g = 160;

	players[DOWN].baseColor.b = 2;
	players[DOWN].baseColor.r = 2;
	players[DOWN].baseColor.g = 16;

}

void pongGame::resetPoints()
{
	players[0].points = 3;
	players[1].points = 3;


}

void pongGame::start()
{
	this->status = RUNNING;
	checkButton(&players[UP]);
	checkButton(&players[DOWN]);

}

void pongGame::prepare()
{
	this->reset();
	this->status = WAIT_FOR_GAMESTART;
	enum direction side;

	if (lastWinner == UP)
	{
		side = DOWN;
	} else {
		side = UP;
	}

	this->dir = side;

	// set start Point
	if (side == UP)
	{
		actualPoint = 0;
	} else {
		actualPoint = NUMBEROFGAMEAREALED - 1;
	}

	movingPoint = players[side].color;

}

bool pongGame::checkButton(player* pl)
{
	if (pl->buttonPressed)
	{
		pl->buttonPressed = false;
		return true;
	}
	return false;

}

void pongGame::pointFor(enum direction winner)
{
	players[winner].points--;
	lastWinner = winner;

	if (players[winner].points == 0)
	{
		//match over
		this->status = MATCH_OVER;
		sound.playSound(MATCH_OVERSOUND);
	} else {
		this->status = POINT;
	}

	//update
    updatePointsOnLedStripe();

	movingPoint = players[winner].color;

	sound.playSound(players[winner].pointJingle);
}

void pongGame::increaseSpeed(uint8_t hitPosition, enum direction dir)
{
	switch (mode)
	{
	case LINEARSPEED:
		this->speed -= 2;
		break;
	case HITSPEED:
		this->speed =(MIN_SPEED + NUMBEROFBASELEDS) - ( NUMBEROFBASELEDS - hitPosition);
       	break;
	default:
		break;
	}

	if (this->speed < MIN_SPEED)
	{
		this->speed = MIN_SPEED;
	}
}
