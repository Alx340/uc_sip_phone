#pragma once


#include "net.h"
#include "sip.h"
#include <avr/pgmspace.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "settings.h"

#define HTTPD_PORT			htons(80)

typedef enum http_pg {
  INDEX,
	IP_SETTINGS,
	MAC_SETTINGS,
	MY_SETTINGS,
	ABON_SETTINGS
} http_pg;


uint8_t tcp_listen(uint8_t id, eth_frame_t *frame);         
void tcp_read(uint8_t id, eth_frame_t *frame, uint8_t re);    
void tcp_write(uint8_t id, eth_frame_t *frame, uint16_t len);               
void tcp_connected(uint8_t id, eth_frame_t *frame, uint8_t hard);           
void tcp_closed(uint8_t id, uint8_t hard);                    
void tcp_opened(uint8_t id, uint8_t hard);  