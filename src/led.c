#include "led.h"



void led_poll(void)
{
  switch(sip_call_state())
		{
			case STATE_RINGING:
        led_on(LED_1);
				break;		
					
			case STATE_ESTABLISHED:	
			 	led_on(LED_3);     
				break;

			case STATE_OUTGOING:	
			 	//led_on(LED_2);     
				break;

      default:
      led_off(LED_1);
      led_off(LED_2); 
      led_off(LED_3);   
      break;	
    }
}