/*
 * pongGame.cpp
 *
 *  Created on: 02.08.2016
 *      Author: ringo
 */

#include "pongGame.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

player players[NUMEROFPLAYERS];
ligthpoint red;

ISR(INT0_vect)
{
	unsigned int i = 0;
	//debounce
	while (!(PIND & ( 1 << PD2)) && (i < 1000)) i++;

	if (i == 1000)
	{
		players[UP].buttonPressed = true;
		players[UP].buttonReset = 0;
		EIMSK &= ~(1 << INT0);
	}

}

ISR(INT1_vect)
{
	unsigned int i = 0;
	//debounce
	while (!(PIND & ( 1 << PD3)) && (i < 1000)) i++;

	if (i == 1000)
	{
		players[DOWN].buttonPressed = true;
		players[DOWN].buttonReset = 0;
		EIMSK &= ~(1 << INT1);
	}
}

pongGame::pongGame() {
	reset();
	resetPoints();
	sound.playSound(STARTSOUND);
	mode = HITIT;//HITSPEED;//LINEARSPEED;

	players[UP].color.b = 160;
	players[UP].color.r = 20;
	players[UP].color.g = 20;

	players[UP].baseColor.b = 16;
	players[UP].baseColor.r = 2;
	players[UP].baseColor.g = 2;

	players[UP].hitPoint.r = 120;
	players[UP].hitPoint.g = 120;
	players[UP].hitPoint.b = 0;

	players[UP].pointJingle = POINT1SOUND;


	players[DOWN].color.b = 20;
	players[DOWN].color.r = 20;
	players[DOWN].color.g = 160;

	players[DOWN].baseColor.b = 2;
	players[DOWN].baseColor.r = 2;
	players[DOWN].baseColor.g = 16;

	players[DOWN].hitPoint.r = 120;
	players[DOWN].hitPoint.g = 120;
	players[DOWN].hitPoint.b = 0;

	players[DOWN].pointJingle = POINT2SOUND;

	// init Buttons
	//EICRA |= ( 1 << ISC11) | ( 1 << ISC01); // falling edge interrupt
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


		if (this->mode == HITIT)
		{
			players[UP].hitPointIndex = this->hitTargetIndex;
			players[DOWN].hitPointIndex = NUMBEROFGAMEAREALED - 1 - this->hitTargetIndex;
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

		if (this->mode == HITIT)
		{
			players[UP].hitPointIndex = this->hitTargetIndex;
			players[DOWN].hitPointIndex = NUMBEROFGAMEAREALED - 1 - this->hitTargetIndex;

			this->leds.setLightPoint(players[UP].hitPointIndex, players[UP].hitPoint);
			this->leds.setLightPoint(players[DOWN].hitPointIndex, players[DOWN].hitPoint);
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

			if (this->mode == HITIT)
			{

				this->leds.setLightPoint(players[UP].hitPointIndex, players[UP].hitPoint);
				this->leds.setLightPoint(players[DOWN].hitPointIndex, players[DOWN].hitPoint);
			}

			// show points
			updatePointsOnLedStripe();

			// move point around
			if ( this->dir == UP)
			{

				// UP TURN (led runs up to green (Player DOWN)
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
						pointFor(UP); // LED out of field, UP(blue gets a point)
						actualPoint = NUMBEROFGAMEAREALED - 1;
					}
				} else {
					// button from Opponnent was pressed!
					if (( actualPoint >= (NUMBEROFGAMEAREALED - NUMBEROFBASELEDS)))
					{
						uint8_t hitPosition = NUMBEROFGAMEAREALED - actualPoint;
						// did he hit the point?
						if ((this->mode == HITIT) && (this->actualPoint == players[DOWN].hitPointIndex))
						{
                           pointFor(DOWN); // he hit the point and gets the point
						} else {
							//valid hit
							this->dir = DOWN; //lets move down to blue
							sound.playSound(HITSOUND);
							increaseSpeed(hitPosition, DOWN);
						}


					} else {
						// point pressed out side game area, point for blue
						pointFor(UP);
					}
				}

			} else {
				// DOWN TURN (blue (UP) has to click)

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
						pointFor(DOWN); // LED OUT of the gaming area
						actualPoint = 0;
					}
				} else {
					// button was pressed!
					if (( actualPoint > 0) && (actualPoint < NUMBEROFBASELEDS))
					{
						uint8_t hitPosition = actualPoint;
						//valid hit
						if ((this->mode == HITIT) && (this->actualPoint == players[UP].hitPointIndex))
						{
							pointFor(UP); // point for blue
						} else {
							this->dir = UP;
							increaseSpeed(hitPosition,UP);
							sound.playSound(HITSOUND);
						}
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

	if (this->mode == HITIT)
	{
      this->hitTargetIndex = rand() % NUMBEROFBASELEDS;
	}

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
	enum direction looser;
    if (winner == UP)
    {
    	looser = DOWN;
    } else {
    	looser = UP;
    }
	players[looser].points--;
	lastWinner = winner;

	if (players[looser].points == 0)
	{
		//match over
		this->status = MATCH_OVER;
		sound.playSound(MATCH_OVERSOUND);
	} else {
		this->status = POINT;
	}

	//update
    updatePointsOnLedStripe();

	movingPoint = players[looser].color;

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
