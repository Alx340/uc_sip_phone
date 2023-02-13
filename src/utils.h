#pragma once


#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "net.h"

void fill_buf(char **buf, char *str);
void fill_buf_P(char **buf, const prog_char * str);
uint16_t atou(char *s);
uint32_t atoul(char *s);
uint32_t atoip(char *s);
void iptoa_buff(char **buff, uint32_t ip);
uint8_t extract_token(char **sp, char *end) ;
void ltoa_buff(char **buff, uint32_t i);