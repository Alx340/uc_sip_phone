#include "audio.h"

uint8_t sound_buf_out[SIZE_SBUF];
uint8_t sound_buf_in[SIZE_SBUF];
uint8_t cnt;
audio_control_t au_control;

ISR(TIMER0_COMP_vect)
{
   //sound_buf_in[cnt] = sound_buf_out[cnt] - 128;
   OCR1A = sound_buf_out[cnt];
   sound_buf_in[cnt] = ADCH - 128;
    ADCSRA |= (1<<ADSC);
    cnt++; 

    if (cnt == 160) {
            cnt = 0;
            rtp_write();
    }             
}


void audio_init(void)
{
  DDRD|=(1<<4);
  DDRD|=(1<<5);
  ASSR=0x00;
  TCCR0=0;
  TCNT0=0x00;
  OCR0=0x26;
	OCR1A = 128;
	OCR1B = 128;
	TCNT1H = 0;
	TCNT1L = 0;
	TCCR1A = 0;
	TCCR1B = 0;
	TIMSK |= 1<<OCIE0;
	ADMUX = (1<<REFS1)| (1<<REFS0)  | (1<<ADLAR)| (0<<MUX4)| (0<<MUX3)| (0<<MUX2)| (0<<MUX1)| (0<<MUX0);
	ADCSRA = (1<<ADEN)  | (4<<ADPS0);
  ADCSRA |= (1<<ADSC);
  
  au_control = AU_WAIT;
}
	
void audio_poll(void)	
{
  eth_frame_t *frame;
  uint8_t *data;
  
  if (au_control == AU_PROCESS){
  
      frame = net_frame();
      data = rtp_payload(frame);
      
      if (rtp_sync_write()) {
      
          if (rtp_sync_read()){
          decoder_g711(sound_buf_out,data,160);
        //  proc_packet++;
          }
          
          encoder_g711(data,sound_buf_in, 160);
          rtp_send(frame, 160);
      }
  }
}

void audio_run(void)	  {   au_control = AU_PROCESS; TCCR0=0b00001011; TCCR1B = 0b00001001; TCCR1A = 0b10110001;}
void audio_stop(void)	{   au_control = AU_WAIT; TCCR0=0; TCCR1B = 0; TCCR1A = 0; PORTD &= ~((1<<5)|(1<<4)); }

/*
void audio_poll(void)	
{
  eth_frame_t *frame;
  uint8_t *data;
  
  if (au_control == AU_PROCESS){
  
      frame = net_frame();
      data = rtp_payload(frame);
  
      if (rtp_sync_write()) {
      
          if (rtp_sync_read()){
          decoder_g711(sound_buf_out,data,160);
          proc_packet++;
          }
          
          encoder_g711(data,sound_buf_in, 160);
          rtp_send(frame, 160);
      }
  }
}
*/