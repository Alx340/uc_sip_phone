#pragma once

#include <avr/interrupt.h>

uint32_t gettc(void);
uint32_t rtime(void);
void counter_init(void);
uint8_t second(void);