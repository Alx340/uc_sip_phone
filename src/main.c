#include "net.h"
#include "audio.h"
#include "sip.h"
#include "led.h"
#include "key.h"
#include "counter.h"
#include "settings.h"
#include <util/delay.h>

int main(void)
{
    key_init();
    led_init(LED_1);
    led_init(LED_2);
    led_init(LED_3);

    srnd();
	  net_init();
	  sip_controll_init();
    sip_init();
	  audio_init();
	  counter_init();
	  
	  if (get_key() != DEF_SET) {   read_eerprom_set();  }
	  else {
            led_on(LED_1);led_on(LED_2);led_on(LED_3); _delay_ms(500);
            led_off(LED_1);led_off(LED_2);led_off(LED_3); _delay_ms(500);
            led_on(LED_1);led_on(LED_2);led_on(LED_3); _delay_ms(500);
            led_off(LED_1);led_off(LED_2);led_off(LED_3); _delay_ms(500); }
            
	  sei();

	while(1){
    net_poll();
    audio_poll();

    led_poll();
    
    if (second()) {
        if (get_key()== KEY_BYE)       {   sip_test_bye();  }
        if (get_key()== KEY_CALL)      {   sip_test_call(); }
        //if (get_key()== KEY_REGISTER)  {   sip_test_register();  }
        if (get_key()== KEY_ACK)       {   sip_test_ack();  }    
    }
  }
  
	return 0;
}
