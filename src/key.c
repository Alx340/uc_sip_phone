#include "key.h"

uint8_t get_key(void)
{
	unsigned char  i;
	for (i = 0; i <8 ; i++)
	{
		if ((KEY_PIN & (1<<i)) == 0)
			return i + 1;
	}
   return 0;
}


void key_init(void)
{
	  KEY_PORT	= 0xFF;		
    KEY_DDR	= 0;       		
}
