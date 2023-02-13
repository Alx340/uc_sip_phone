#pragma once

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "net.h"
#include "rtp.h"
#include "g711.h"

#define SIZE_SBUF 160         

typedef enum audio_control {
	AU_WAIT,
	AU_PROCESS
} audio_control_t;

void audio_init(void);
void audio_poll(void);
void audio_run(void);
void audio_stop(void);