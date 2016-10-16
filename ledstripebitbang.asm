/*
 * ledstripebitbang.asm
 *
 *  Created on: 26.07.2016
 *      Author: ringo
 */


//.include "m8def.inc"
#include <avr/io.h>


.global ledstripesendbyte


ledstripesendbyte:

	push R18
	push R19
	push R20
	push R28

	//sbi _SFR_IO_ADDR(PORTD), 1
	mov XH, r25
	mov XL, r24
	ldi R18, 255
nextByte:
	ld R20, X+
	ldi R28, 8
iterBit:
	sbi _SFR_IO_ADDR(PORTD), 1
	ldi R19, 0
	lsl R20
	brcc clear // 0 - kurz, 1 lang
	ldi R19,1
	nop
	nop
	nop
	//nop
clear:
	cbi _SFR_IO_ADDR(PORTD), 1
	tst R19
	brne finish //wenn 1 - kurz 0 -lang
	nop
	nop
	nop
	//nop

finish:
	dec R28
	brne iterBit
Ende:
	dec R18
	brne nextByte
	cbi _SFR_IO_ADDR(PORTD), 1
	pop R28
	pop R20
	pop R19
	pop R18
	ret



