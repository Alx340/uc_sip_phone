#pragma once

#include "net.h"
#include "utils.h"
#include <avr/pgmspace.h>
#include "rnd.h"


typedef struct sdp_session {              
    uint32_t addr; 
    uint16_t port;    
} sdp_session_t;



uint16_t sdp_add(char *bufptr);
uint8_t sdp_parse(char *data, uint16_t len);


uint32_t sdp_raddres(void);
uint16_t sdp_rport(void);
void sdp_connection(uint32_t addr, uint16_t port);
