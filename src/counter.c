#include "counter.h"

static uint16_t ms_count;

static uint32_t tick_count;
static uint32_t second_count;

static uint32_t timer;

ISR(TIMER2_COMP_vect)
{
	++ms_count;
	++tick_count;

	if(ms_count == 1000)
	{
		++second_count;
		ms_count = 0;
	}
}


uint32_t gettc(void)
{
return tick_count;
}

uint32_t rtime(void)
{
return second_count;
}


uint8_t second(void)
{
  if (second_count > timer)
  {
    timer = second_count;
    return 1;
  }
return 0;
}

void counter_init(void)
{
	// clk = Fcpu/128, mode = CTC, freq = 1 kHz
	TCCR2 = (1<<WGM21)|(1<<CS22)|(1<<CS20);
	OCR2 = F_CPU/128/1000;
	TIMSK |= 1<<OCIE2;
}
