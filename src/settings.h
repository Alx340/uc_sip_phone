#pragma once

#include <avr/io.h>
#include "sip.h"
#include "net.h"
#include <avr/eeprom.h> 


typedef struct net_settings{
	uint32_t	myip;
	uint32_t	gateway;
	uint32_t	mask;
	char my_name[9];
	char my_domain[18];
	char abon_name[9];
	char abon_domain[18];
} net_settings_t;



void read_eerprom_set(void);
void save_eerprom_ip(uint32_t ip);
void save_eerprom_gw(uint32_t ip);
void save_eerprom_mask(uint32_t ip);
void save_eerprom_my(char *name, uint8_t len);
void save_eerprom_my_domain(char *name, uint8_t len);
void save_eerprom_abon(char *name, uint8_t len);
void save_eerprom_domain(char *name, uint8_t len);