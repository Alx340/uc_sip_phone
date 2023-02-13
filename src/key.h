#pragma once

#include <avr/io.h>

#define KEY_BYE				3
#define KEY_CALL			2
#define KEY_REGISTER	4
#define KEY_ACK			  1
#define DEF_SET			  5

#define KEY_PIN			PINC
#define KEY_PORT		PORTC
#define KEY_DDR			DDRC

void key_init(void);
uint8_t get_key(void);

