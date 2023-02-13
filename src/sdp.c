#include "sdp.h"

sdp_session_t sdp_descr;

void parse_media(char *value, char *end, sdp_session_t *descr)
{
    extract_token(&value, end); // Audio
    descr->port = atou(value); // port
}

void parse_connection(char *value, char *end, sdp_session_t *descr)
{
    extract_token(&value, end); // IN
    extract_token(&value, end); // IP4
    descr->addr = atoip(value); // addres
}

uint16_t sdp_add(char *bufptr)
{
    char *ptr = bufptr;
    sdp_session_t *sdp= &sdp_descr;
    
    fill_buf_P(&bufptr, PSTR("v=0\r\n"));
    fill_buf_P(&bufptr, PSTR("o=test "));
    ltoa_buff (&bufptr, rnd()); 
    fill_buf_P(&bufptr, PSTR(" 1234 IN IP4 "));
    iptoa_buff(&bufptr, sdp->addr);
    fill_buf_P(&bufptr, PSTR("\r\n"));

    fill_buf_P(&bufptr, PSTR("s=uC"));
    //ltoa_buff (&bufptr, rnd()); 
    fill_buf_P(&bufptr, PSTR("\r\n"));  
        
    fill_buf_P(&bufptr, PSTR("c=IN IP4 "));
    iptoa_buff(&bufptr, sdp->addr);
    fill_buf_P(&bufptr, PSTR("\r\n"));     
    
    fill_buf_P(&bufptr, PSTR("t=0 0\r\n"));
    fill_buf_P(&bufptr, PSTR("m=audio "));
    ltoa_buff (&bufptr, sdp->port);
    fill_buf_P(&bufptr, PSTR(" RTP/AVP 0\r\n"));
    fill_buf_P(&bufptr, PSTR("a=sendrecv\r\n"));  
    fill_buf_P(&bufptr, PSTR("a=rtpmap:0 PCMU/8000\r\n"));   
        
    return (bufptr - ptr);
}

uint8_t sdp_parse(char *data, uint16_t len)
{
   char *ptr, *s_end, *line_end, *next_line, *value;
	
	sdp_session_t *sdp = 	&sdp_descr;
	
  ptr = data;
  s_end = data + len;
  
  if (len == 0) return 0;
  
  while (ptr && ptr < s_end - 1) {
  if (ptr[1] != '=') return 0;
 
   value = &ptr[2];
   line_end = memchr(value, '\n', s_end - value);
		if (!line_end) {
			line_end = s_end;
			next_line = NULL;
		}
		else {
			next_line = line_end + 1;
			if (line_end[-1] == '\r')
				line_end--;
		}

		switch (ptr[0]) {

			case 'm':
        if(!memcmp_P(value, PSTR("audio"), 5)) {
				parse_media(value, line_end, sdp);}
				break;

			case 'c':
        parse_connection(value, line_end, sdp);
				break;

			default:
				break;				
		}
		ptr= next_line;
	}
	return 1;
}

uint32_t sdp_raddres(void)
{
	return sdp_descr.addr;
}
uint16_t sdp_rport(void)
{
	return sdp_descr.port;
}

void sdp_connection(uint32_t addr, uint16_t port)
{
    sdp_session_t *sdp = 	&sdp_descr;
    sdp->addr = addr;
    sdp->port = port;
}
