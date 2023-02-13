#pragma once

#include <string.h>
#include <stdint.h> 

#define G711_BPS	    64000
#define PTIME		    10	/* basic frame size is 10 msec	    */
#define FRAME_SIZE	    (8000 * PTIME / 1000)   /* 80 bytes	    */
#define SAMPLES_PER_FRAME   (8000 * PTIME / 1000)   /* 80 samples   */

uint8_t linear2alaw(int	pcm_val);	
int alaw2linear(unsigned char	a_val);
unsigned char encoder_g711(unsigned char *buff_out, unsigned char  *buff_in, unsigned int len);
unsigned char decoder_g711(unsigned char *buff_out, unsigned char  *buff_in, unsigned int len);
