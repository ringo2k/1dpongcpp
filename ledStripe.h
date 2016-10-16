/*
 * ledStripe.h
 *
 *  Created on: 01.08.2016
 *      Author: ringo
 */

#ifndef LEDSTRIPE_H_
#define LEDSTRIPE_H_
#include "softwareTimer.h"

#define NUMBEROFLIGHTPOINTS 85


enum ledColor
{
	C_WHITE,
	C_RED,
	C_GREEN,
	C_BLUE,
	C_OFF
};

typedef struct s_light
{
	unsigned char g;
	unsigned char r;
	unsigned char b;

} ligthpoint __attribute__((packed));

typedef struct s_playground
{
	ligthpoint lightPoints[NUMBEROFLIGHTPOINTS];
} playground;

class ledStripe {
private:
public:
	softwareTimer updateTimer;
	ledStripe();
	~ledStripe();
	void run();
	void clear();
	void setLightPoint(unsigned char index, ligthpoint in);
	void setAllLigthPoints(ligthpoint lp);
};

#endif /* LEDSTRIPE_H_ */
