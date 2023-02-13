#include "settings.h"

net_settings_t  EEMEM sett_eep;

void read_eerprom_set(void)
{
   net_settings_t tmp_conf;
   eeprom_read_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
   set_ip(tmp_conf.myip); 
   set_gw(tmp_conf.gateway);
   set_mask(tmp_conf.mask);
   sip_put_my(tmp_conf.my_name, 9);
   sip_put_my_domain(tmp_conf.my_domain, 18);
   sip_put_abon(tmp_conf.abon_name, 9);
   sip_put_abon_domain(tmp_conf.abon_domain, 18);
}

void save_eerprom_ip(uint32_t ip)
{
   net_settings_t tmp_conf;
	 eeprom_read_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
	 tmp_conf.myip = ip;
	 eeprom_write_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
}

void save_eerprom_gw(uint32_t ip)
{
   net_settings_t tmp_conf;
	 eeprom_read_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
	 tmp_conf.gateway = ip;
	 eeprom_write_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
}

void save_eerprom_mask(uint32_t ip)
{
   net_settings_t tmp_conf;
	 eeprom_read_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
	 tmp_conf.mask = ip;
	 eeprom_write_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
}

void save_eerprom_my(char *name, uint8_t len)
{  
   char *tmp;
   net_settings_t tmp_conf;
	 eeprom_read_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
	 memcpy(tmp_conf.my_name, name, len);
	 tmp = tmp_conf.my_name + len;
   *tmp = '\0'; 	 
	 eeprom_write_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
}

void save_eerprom_my_domain(char *name, uint8_t len)
{  
   char *tmp;
   net_settings_t tmp_conf;
	 eeprom_read_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
	 memcpy(tmp_conf.my_domain, name, len);
	 tmp = tmp_conf.my_domain + len;
   *tmp = '\0'; 
	 eeprom_write_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
}


void save_eerprom_abon(char *name, uint8_t len)
{  
   char *tmp;
   net_settings_t tmp_conf;
	 eeprom_read_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
	 memcpy(tmp_conf.abon_name, name, len);
	 tmp = tmp_conf.abon_name + len;
   *tmp = '\0'; 
	 eeprom_write_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
}

void save_eerprom_domain(char *name, uint8_t len)
{  
   char *tmp;
   net_settings_t tmp_conf;
	 eeprom_read_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
	 memcpy(tmp_conf.abon_domain, name, len);
	 tmp = tmp_conf.abon_domain + len;
   *tmp = '\0'; 
	 eeprom_write_block(&tmp_conf, &sett_eep, sizeof(net_settings_t));
}
