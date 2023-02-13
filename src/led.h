#pragma once


#include <avr/io.h>
#include "sip.h"

#define LED_PORT	PORTD
#define LED_DDR		DDRD
#define LED_1		(1<<PD1)
#define LED_2		(1<<PD0)
#define LED_3		(1<<PD7)


#define led_off(a)	LED_PORT &= ~a
#define led_on(a)	LED_PORT |= a
#define led_init(a)	LED_DDR |= a ; led_off(a)


void led_poll(void);