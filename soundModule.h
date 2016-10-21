/*
 * soundModule.h
 *
 *  Created on: 28.07.2016
 *      Author: ringo
 */

#ifndef SOUNDMODULE_H_
#define SOUNDMODULE_H_

#include "softwareTimer.h"

enum SOUNDNAMES
{
	STARTSOUND,
	POINT1SOUND,
	POINT2SOUND,
	HITSOUND,
	MATCH_OVERSOUND,
	SUDDEN_DEATH_SOUND,
	NOSOUND
};

typedef struct s_tone{
	unsigned char value;
	unsigned char length;
} tone;



typedef struct s_jingle{
	tone* tones;
	unsigned int length;
} jingle;


class soundModule {
public:
	soundModule();
	~soundModule();
	void setTone(unsigned char tone);
	void playSound(enum SOUNDNAMES newSound);
	void noSound();
	void run();
private:
	softwareTimer toneLengthTimer;
	enum SOUNDNAMES activeSound;
	unsigned int toneIndex;
	void wait();
};

#endif /* SOUNDMODULE_H_ */
