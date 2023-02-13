#include "utils.h"



void fill_buf(char **buf, char *str)
{
	uint16_t len = 0;
	char *p = *buf;

	while(*str)
		p[len++] = *(str++);
	*buf += len;
}

void fill_buf_P(char **buf, const prog_char * str)
{
	uint16_t len = 0;
	char *p = *buf, byte;

	while((byte = pgm_read_byte(str++)))
		p[len++] = byte;
	*buf += len;
}

uint16_t atou(char *s)
{
  uint16_t value=0;

  if(!s)
  {
    return 0;
  }  

  while(*s)
  {
    if((*s >= '0') && (*s <= '9'))
    {
      value = (value*10) + (*s-'0');
    }
    else
    {
      break;
    }
    s++;
  }

  return value;
}

uint32_t atoul(char *s)
{
  uint32_t value=0;

  if(!s)
  {
    return 0;
  }  

  while(*s)
  {
    if((*s >= '0') && (*s <= '9'))
    {
      value = (value*10) + (*s-'0');
    }
    else
    {
      break;
    }
    s++;
  }

  return value;
}

uint32_t atoip(char *s)
{
  uint32_t ip=0;
  uint8_t ip_4, ip_3, ip_2, ip_1;
  
  if(isdigit(*s))
  {
    ip_4 = atou(s);
    while(isdigit(*s)){ s++; }; while(!isdigit(*s)){ s++; };
    ip_3 = atou(s);
    while(isdigit(*s)){ s++; }; while(!isdigit(*s)){ s++; };
    ip_2 = atou(s);
    while(isdigit(*s)){ s++; }; while(!isdigit(*s)){ s++; };
    ip_1 = atou(s);
    ip = inet_addr(ip_4,ip_3,ip_2,ip_1);
    
  }
  else
  {
   // ip = dns_getip(s);
  }

  return ip;
}


uint8_t extract_token(char **sp, char *end) {
	char *space;
	
	if (**sp == ' ') {
	*sp += 1;
	return 0;
	}
	
	space = memchr(*sp, ' ', end - *sp);
	
	if (space == *sp || end == *sp)
		return -1;
		
	if (!space) {
		*sp = end;
	}
	else {
		*sp = space + 1;
	}
	return 0;
}

void iptoa_buff(char **buff, uint32_t ip)
{
   uint8_t ip_tmp[4];
   char *prt;
   uint8_t shifter, cnt;
   
   ip_tmp[3] =  (ip>>24);
   ip_tmp[2]  = (ip>>16) & 0xFF;
   ip_tmp[1]  = (ip>>8) & 0xFF;
   ip_tmp[0]  =  ip & 0xFF;   

   prt = *buff;
   cnt = 3;
   
   do{
   shifter = ip_tmp[cnt];
   
        do{
            ++prt;
            shifter = shifter/10;
        }while(shifter);
    ++prt;
   }while(cnt--);

   --prt;
   *prt = '\0';
   *buff = prt;
   
   cnt = 3;
   
   do{ 
   shifter = ip_tmp[cnt];
        do{ 
            *--prt = (shifter%10) + '0';
            shifter = shifter/10;
        }while(shifter);
   if (cnt > 0) *--prt = '.';
   }while(cnt--);   
}

void ltoa_buff(char **buff, uint32_t i)
{
   char *prt;
   uint32_t shifter;

   prt = *buff;
   shifter = i;
    
   do{
       ++prt;
       shifter = shifter/10;
   }while(shifter);
    
   *prt = '\0';
   *buff = prt;
   
   do{
       *--prt = (i%10) + '0';
       i = i/10;
   }while(i);
}