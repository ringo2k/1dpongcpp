/*
 * pongGame.h
 *
 *  Created on: 02.08.2016
 *      Author: ringo
 */

#ifndef PONGGAME_H_
#define PONGGAME_H_
#include "ledStripe.h"
#include "softwareTimer.h"
#include "soundModule.h"


#define DEFAULT_SPEED (MIN_SPEED + NUMBEROFBASELEDS)
#define MIN_SPEED 3

#define NUMBEROFBASELEDS 10
#define NUMBEROFGAMEAREALED 74

#define NUMEROFPLAYERS 2

typedef struct s_player
{
	unsigned char points;
	ligthpoint color;
	ligthpoint baseColor;
	bool buttonPressed;
	unsigned char buttonReset;
	enum SOUNDNAMES pointJingle;

} player;

enum GAMEMODES
{
	LINEARSPEED,
	HITSPEED
};

enum direction
{
	UP,
	DOWN
};

enum gameStatus
{
	WAIT_FOR_GAMESTART,
	RUNNING,
	POINT,
	FINISH,
	MATCH_OVER
};

class pongGame {
private:
	softwareTimer blinkTimer, speedTimer;
	ledStripe leds;
	int actualPoint;
	ligthpoint movingPoint;
	gameStatus status;
	enum direction dir, lastWinner;
	enum GAMEMODES mode;
	int speed;
	void reset();
	void resetPoints();
public:
	soundModule sound;
	pongGame();
	~pongGame();
	void run();
	void start();
	void prepare();
	bool checkButton(player* pl);
	void pointFor(enum direction winner);
	void increaseSpeed(uint8_t hitPosition, enum direction);
	void updatePointsOnLedStripe();


};

#endif /* PONGGAME_H_ */
