/*
 * soundModule.cpp
 *
 *  Created on: 28.07.2016
 *      Author: ringo
 */

#include "soundModule.h"
#include <avr/io.h>
#include <avr/delay.h>



jingle jingles[10];

tone start_jingel[]
				 {
						 {50,200},
						 {100,200},
						 {150,200},
						 {200,200},
						 {250,200},
						 {0,200},
						 {250,200}

				 };

tone point1_jingel[]
				 {
						 {50,50},
						 {100,100},
						 {50,50},
						 {100,100},
						 {50,50},
						 {100,100}

				 };

tone point2_jingel[]
				 {
						 {150,50},
						 {200,100},
						 {150,50},
						 {200,100},
						 {150,50},
						 {200,100}

				 };

tone hit_jingle[]
				 {
						 {200,50},
						 {20,50}
				 };


tone matchover_jingle[]
				 {
						 {140,60},
						 {160,30},
						 {170,200},
						 {0,10},
						 {120,350},
						 {0,10},
						 {140,60},
						 {160,30},
						 {170,200}
				 };

soundModule::soundModule() {

	DDRD |= ( 1 << PD5);
	TCCR0A |= ( 1 << COM0B0) | ( 1 << WGM01);
	TCCR0B |= ( 1 << CS01) | ( 1 << CS00);
	OCR0B = 1;
	OCR0A = 0;

	activeSound = NOSOUND;

	// init jingles
	jingles[STARTSOUND].tones = start_jingel;
	jingles[STARTSOUND].length = 7;

	jingles[POINT1SOUND].tones = point1_jingel;
	jingles[POINT1SOUND].length = 6;

	jingles[POINT2SOUND].tones = point2_jingel;
	jingles[POINT2SOUND].length = 6;

	jingles[HITSOUND].tones = hit_jingle;
	jingles[HITSOUND].length = 2;

	jingles[MATCH_OVERSOUND].tones = matchover_jingle;
	jingles[MATCH_OVERSOUND].length = 9;// matchover_jingle;

	toneIndex = 0;

}

soundModule::~soundModule() {
	// TODO Auto-generated destructor stub
}

void soundModule::setTone(unsigned char in)
{
	OCR0A = in;
}

void soundModule::noSound()
{
	OCR0A = 0;
}

void soundModule::playSound(enum SOUNDNAMES newSound)
{
	if (this->activeSound == NOSOUND)
	{
		this->activeSound = newSound;
		toneIndex = 0;
	}
}

void soundModule::run()
{
	if (activeSound != NOSOUND)
	{
		if (toneIndex >= jingles[this->activeSound].length)
		{
			// finished song
			this->activeSound = NOSOUND;
			noSound();
		} else {
			// we are playing
			setTone(jingles[this->activeSound].tones[toneIndex].value);
			if (toneLengthTimer.wait(jingles[this->activeSound].tones[toneIndex].length) == TIMES_UP)
			{
				toneIndex++;
			}
		}
	}


}

