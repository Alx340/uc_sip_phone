#include "server.h"

http_pg http_page = INDEX;

uint8_t tcp_listen(uint8_t id, eth_frame_t *frame)
{
	ip_packet_t *ip = (void*)(frame->data);
	tcp_packet_t *tcp = (void*)(ip->data);

	return (tcp->to_port == HTTPD_PORT);
}

void tcp_read(uint8_t id, eth_frame_t *frame, uint8_t re){
        
    ip_packet_t *ip = (void*)(frame->data);
    tcp_packet_t *tcp = (void*)(ip->data);
	  char *data = (void*)(tcp->data); 
    uint16_t len_str;
    char *buff;
    buff = data; 
    uint8_t *mac;
     
    fill_buf_P(&buff, PSTR("HTTP/1.0 200 OK\r\n"));
		fill_buf_P(&buff, PSTR("Content-Type: text/html; charset=windows-1251\r\n"));		
		fill_buf_P(&buff, PSTR("Server: ATmega32\r\n"));
		fill_buf_P(&buff, PSTR("\r\n"));
		fill_buf_P(&buff, PSTR("<pre><a href='/'>Main</a> | <a href='/ip'>IP config</a> | <a href='/mac'>Mac config</a> | <a href='/my'>My config</a> | <a href='/abon'>Abon config</a></pre>")); 
    fill_buf_P(&buff, PSTR("\r\n"));
    fill_buf_P(&buff, PSTR("\r\n"));
    fill_buf_P(&buff, PSTR("<pre>"));
    
    if( http_page == INDEX )
        {
            fill_buf_P(&buff, PSTR("<h2>Server Up!</h2>"));
        }
    
    if( http_page == IP_SETTINGS )
        {
            fill_buf_P(&buff, PSTR("<h2>IP settings</h2>"));
            fill_buf_P(&buff, PSTR("<form name='chname' action='/' method='GET'>"));	
            fill_buf_P(&buff, PSTR("IP: <input name='ip' maxlength='15' value='"));
            iptoa_buff(&buff, get_local_ip());
            fill_buf_P(&buff, PSTR("'/><br/>"));
        		fill_buf_P(&buff, PSTR("Gateway IP: <input name='gw_ip' maxlength='15' value='"));
            iptoa_buff(&buff, get_local_gw());		
            fill_buf_P(&buff, PSTR("'/><br/>"));
            fill_buf_P(&buff, PSTR("Net Mask: <input name='mask' maxlength='15' value='"));
            iptoa_buff(&buff, get_local_mask());			
            fill_buf_P(&buff, PSTR("'/><br/>"));
            fill_buf_P(&buff, PSTR("<input type='submit' value='Save'/></form>"));
        }
        
    if( http_page == MAC_SETTINGS )
        {
            mac = get_local_mac();
            fill_buf_P(&buff, PSTR("<h2>MAC settings</h2>"));
        		fill_buf_P(&buff, PSTR("<form name='chname' action='/' method='GET'>"));
            fill_buf_P(&buff, PSTR("MAC: <input name='mac' maxlength='20' value='"));
            ltoa_buff (&buff, mac[0]);fill_buf_P(&buff, PSTR(":")); 
            ltoa_buff (&buff, mac[1]);fill_buf_P(&buff, PSTR(":")); 
            ltoa_buff (&buff, mac[2]);fill_buf_P(&buff, PSTR(":")); 
            ltoa_buff (&buff, mac[3]);fill_buf_P(&buff, PSTR(":")); 
            ltoa_buff (&buff, mac[4]);fill_buf_P(&buff, PSTR(":")); 	
            ltoa_buff (&buff, mac[5]);		
            fill_buf_P(&buff, PSTR("'/><br/>"));
            fill_buf_P(&buff, PSTR("<input type='submit' value='Save'/></form>"));
        }
         
    if( http_page == MY_SETTINGS )
        {
            fill_buf_P(&buff, PSTR("<h2>My settings</h2>"));
        		fill_buf_P(&buff, PSTR("<form name='chname' action='/' method='GET'>"));
            fill_buf_P(&buff, PSTR("My USER NAME: <input name='m_name' maxlength='9' value='"));
            fill_buf(&buff, sip_get_m_name());
            fill_buf_P(&buff, PSTR("'/><br/>"));
            fill_buf_P(&buff, PSTR("My Domain: <input name='a_ip' maxlength='18' value='"));
            fill_buf(&buff, sip_get_m_domain());		
            fill_buf_P(&buff, PSTR("'/><br/>"));  
            fill_buf_P(&buff, PSTR("<input type='submit' value='Save'/></form>"));   
        }
           
    if( http_page == ABON_SETTINGS ) 
        {
            fill_buf_P(&buff, PSTR("<h2>Abon settings</h2>"));
        		fill_buf_P(&buff, PSTR("<form name='chname' action='/' method='GET'>"));
            fill_buf_P(&buff, PSTR("Abon USER NAME: <input name='a_name' maxlength='9' value='"));
            fill_buf(&buff, sip_get_a_name());		
            fill_buf_P(&buff, PSTR("'/><br/>"));
            fill_buf_P(&buff, PSTR("Abon Domain: <input name='a_ip' maxlength='18' value='"));
            fill_buf(&buff, sip_get_a_domain());		
            fill_buf_P(&buff, PSTR("'/><br/>"));
            fill_buf_P(&buff, PSTR("<input type='submit' value='Save'/></form>")); 
        }

		fill_buf_P(&buff, PSTR("</pre>"));
				
    len_str = buff - data;
    tcp_send(id, frame, len_str, 2);
}

void tcp_write(uint8_t id, eth_frame_t *frame, uint16_t len)
{

	ip_packet_t *ip = (void*)(frame->data);
	tcp_packet_t *tcp = (void*)(ip->data);
	char *request = (void*)tcp_get_data(tcp);
  char *params;
  char *tmp;
  
	if( !memcmp_P(request, PSTR("GET "), 4))
		{
      params = request + 4;
      
      if( !memcmp_P(params, PSTR("/ "), 2))  {  http_page = INDEX;}
      if( !memcmp_P(params, PSTR("/ip"), 3))  { http_page = IP_SETTINGS;}
      if( !memcmp_P(params, PSTR("/mac"), 4)) { http_page = MAC_SETTINGS;}
      if( !memcmp_P(params, PSTR("/my"), 3))  { http_page = MY_SETTINGS;}
      if( !memcmp_P(params, PSTR("/abon"), 5)) {http_page = ABON_SETTINGS;}
      
      if( !memcmp_P(params, PSTR("/?"), 2))
            {    
               if( http_page == IP_SETTINGS )
                  {
                    params = (strchr(request, '=') +1);
                    //test_ip = atoip(params);
                   // set_ip(atoip(params));
                   save_eerprom_ip(atoip(params));
                    params = (strchr(params, '=') +1);
                   // test_gw = atoip(params);
                   // set_gw(atoip(params));
                   save_eerprom_gw(atoip(params));
                    params = (strchr(params, '=') +1);
                   // test_mask = atoip(params);
                   // set_mask(atoip(params));
                   save_eerprom_mask(atoip(params));
                  }
                  
               if( http_page == MAC_SETTINGS )
                  {  
                  /*
                    params = (strchr(request, '=') +1);
                    test_mac[0] = atou(params); params = (strchr(params, '%') + 4);
                    test_mac[1] = atou(params); params = (strchr(params, '%') + 4);
                    test_mac[2] = atou(params); params = (strchr(params, '%') + 4);
                    test_mac[3] = atou(params); params = (strchr(params, '%') + 4);
                    test_mac[4] = atou(params); params = (strchr(params, '%') + 4);
                    test_mac[5] = atou(params); 
                    */
                    read_eerprom_set();
                  }
                  
               if( http_page == MY_SETTINGS )
                  {
                    params = (strchr(request, '=') +1);
                    tmp = (strchr(params, '&'));
                    //memcpy(test_mac, params, tmp-params);
                    //sip_put_my(params, tmp-params);
                    save_eerprom_my(params, tmp-params);
                    
                    params = (strchr(tmp, '=') +1);
                    tmp = (strchr(params, ' '));
                    //memcpy(test_namep, params, tmp-params);
                    //sip_put_abon_domain(params, tmp-params);;
                    save_eerprom_my_domain(params, tmp-params);
                    
                  }
               
               if( http_page == ABON_SETTINGS ) 
                  {
                    params = (strchr(request, '=') +1);
                    tmp = (strchr(params, '&'));
                    //memcpy(test_mac, params, tmp-params);
                   // sip_put_abon(params, tmp-params);
                   save_eerprom_abon(params, tmp-params);
                    
                    params = (strchr(tmp, '=') +1);
                    tmp = (strchr(params, ' '));
                    //memcpy(test_namep, params, tmp-params);
                    //sip_put_abon_domain(params, tmp-params);;
                    save_eerprom_domain(params, tmp-params);
                  }
            }

		}
}

void tcp_closed(uint8_t id, uint8_t hard){}
void tcp_opened(uint8_t id, uint8_t hard){}
void tcp_connected(uint8_t id, eth_frame_t *frame, uint8_t hard){}
