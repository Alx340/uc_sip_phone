#include "sip.h"

const prog_char linebreak[] =       "\r\n";

sip_message_t sip_descriptor;
sip_state_t call_state;


void sip_req_msg(char **buff, const prog_char * str, sip_message_t *sip)
{
    fill_buf_P(buff, str);
    fill_buf_P(buff, PSTR(" sip:"));
    fill_buf(buff, sip->abon.user_name);
    fill_buf_P(buff, PSTR("@"));
    fill_buf(buff, sip->abon.domain);
    fill_buf_P(buff, PSTR(" SIP/2.0"));
    fill_buf_P(buff, linebreak);
}

void sip_add_via(char **buff, sip_via_t *p_via)
{
		fill_buf_P(buff, PSTR("Via: SIP/2.0/UDP "));
		fill_buf  (buff, p_via->via_name);
		fill_buf_P(buff, PSTR(":"));
		ltoa_buff (buff, p_via->port);
		fill_buf_P(buff, PSTR(";rport"));
		
		if (p_via->rport)
		{
      fill_buf_P(buff, PSTR("="));
      ltoa_buff (buff, p_via->rport);
		}
		
		fill_buf_P(buff, PSTR(";branch="));
		fill_buf  (buff, p_via->branch);		
		fill_buf_P(buff, linebreak);
}

void sip_add_max_forwards(char **buff)
{
		fill_buf_P(buff, PSTR("Max-Forwards: 70"));
		fill_buf_P(buff, linebreak);
}

void sip_add_to(char **buff, sip_abon_t *abon)
{
    fill_buf_P(buff, PSTR("To: "));
		fill_buf_P(buff, PSTR("<sip:"));
    fill_buf(buff, abon->user_name);
		fill_buf_P(buff, PSTR("@"));		
		fill_buf(buff, abon->domain);
		fill_buf_P(buff, PSTR(">"));
		
		if(call_state == STATE_OUTGOING) {
		fill_buf_P(buff, linebreak);
		return;
    }
    
		if(call_state != STATE_INCOMING) {    
    fill_buf_P(buff, PSTR(";tag="));
    fill_buf(buff, abon->tag); 
    }
		fill_buf_P(buff, linebreak);
}

void sip_add_from(char **buff, sip_abon_t *abon)
{
    fill_buf_P(buff, PSTR("From: ")); 
		fill_buf_P(buff, PSTR("<sip:"));
    fill_buf(buff, abon->user_name);
		fill_buf_P(buff, PSTR("@"));		
		fill_buf(buff, abon->domain);
		fill_buf_P(buff, PSTR(">"));
    fill_buf_P(buff, PSTR(";tag="));
    fill_buf(buff, abon->tag); 
		fill_buf_P(buff, linebreak);
}

void sip_add_call_id(char **buff, char *call_id)
{
		fill_buf_P(buff, PSTR("Call-ID: "));
		fill_buf(buff, call_id);		
		fill_buf_P(buff, linebreak);
}

void sip_add_cseq(char **buff, sip_cseq_t *cseq)
{
		fill_buf_P(buff, PSTR("CSeq: "));
	  ltoa_buff (buff, cseq->ncseq);
		fill_buf_P(buff, PSTR(" "));	
		fill_buf(buff, cseq->req);
		fill_buf_P(buff, linebreak);
}

void sip_add_contact(char **buff, sip_abon_t *abon)
{
		fill_buf_P(buff, PSTR("Contact: "));
		fill_buf_P(buff, PSTR("<sip:"));		
    fill_buf(buff, abon->user_name);
		fill_buf_P(buff, PSTR("@"));		
		fill_buf(buff, abon->domain);

		if (abon->port)
		{
      fill_buf_P(buff, PSTR(":"));
      ltoa_buff(buff, abon->port);
		}		

		fill_buf_P(buff, PSTR(">"));		
		fill_buf_P(buff, linebreak);
}

void sip_add_content_type(char **buff)
{
		fill_buf_P(buff, PSTR("Content-Type: application/sdp"));	
		fill_buf_P(buff, linebreak);
}

void sip_add_content_length(char **buff, uint16_t len)
{
		fill_buf_P(buff, PSTR("Content-Length: "));	
	  ltoa_buff (buff, len);
		fill_buf_P(buff, linebreak);
}

void sip_add_allow(char **buff)
{
		fill_buf_P(buff, PSTR("Allow: INVITE, ACK, BYE, CANCEL"));	
		fill_buf_P(buff, linebreak);
}

void sip_add_expires(char **buff)
{
		fill_buf_P(buff, PSTR("Expires: 900"));	
		fill_buf_P(buff, linebreak);
}

void sip_add_user_agent(char **buff)
{
		fill_buf_P(buff, PSTR("User-Agent: Microcontroller"));	
		fill_buf_P(buff, linebreak);
}
// ---------------------------------------


void sip_invite(void)
{
  eth_frame_t *frame = net_frame();
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
	char *buf = (void*)(udp->data);
  sip_message_t *sip = &sip_descriptor;
  char *bufptr, *tmp_ptr;
  uint16_t len;   
     
  tmp_ptr = sip->local.tag;
  ltoa_buff(&tmp_ptr, rnd());  
  
  tmp_ptr = sip->call_id;  
  ltoa_buff(&tmp_ptr, rnd());  
     
  tmp_ptr = sip->via.via_name;
	fill_buf(&tmp_ptr, sip->local.domain);
	
  tmp_ptr = sip->via.branch;
  ltoa_buff(&tmp_ptr, rnd()); 	
  
  sip->via.port = sip->local.port;
  sip->via.rport = 0;
    
  call_state = STATE_OUTGOING;
  
  sip->cseq.ncseq++;
  memcpy_P(sip->cseq.req, PSTR("INVITE\0"), 7);

  bufptr = buf;
  
  sdp_connection(get_local_ip(), sip_local_mport());
  
  sip->content_len = sdp_add(bufptr);
  
  sip_req_msg(&bufptr, PSTR("INVITE"), sip);
  sip_add_via(&bufptr, &sip->via);
  sip_add_max_forwards(&bufptr);
  sip_add_from(&bufptr, &sip->local); 
  sip_add_to(&bufptr, &sip->abon);
  sip_add_call_id(&bufptr, sip->call_id); 
  sip_add_cseq(&bufptr, &sip->cseq);
  sip_add_allow(&bufptr);
  sip_add_contact(&bufptr, &sip->local);  
  sip_add_content_type(&bufptr);	
  sip_add_content_length(&bufptr, sip->content_len );	
  fill_buf_P(&bufptr, linebreak);

  sdp_add(bufptr);
  
  len = bufptr - buf;
  len += sip->content_len;
  
  ip->to_addr = sip->abon.ip_addr;
  udp->from_port = htons(sip->local.port);
  udp->to_port = htons(sip->abon.port);
  
  udp_send(frame,len);
}

void sip_register(void)
{
  eth_frame_t *frame = net_frame();
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
	char *buf = (void*)(udp->data);
  sip_message_t *sip = &sip_descriptor;
  char *bufptr, *tmp_ptr;
  uint16_t len;
   
  tmp_ptr = sip->local.tag;
  ltoa_buff(&tmp_ptr, rnd());  
  
  tmp_ptr = sip->call_id;  
  ltoa_buff(&tmp_ptr, rnd());  
  
  tmp_ptr = sip->via.branch;
  ltoa_buff(&tmp_ptr, rnd()); 
      
  tmp_ptr = sip->via.via_name;
	fill_buf(&tmp_ptr, sip->local.domain);
	
  sip->via.port = sip->local.port;
  sip->via.rport = 0;

  call_state = STATE_OUTGOING;
  
  sip->cseq.ncseq++;
  memcpy_P(sip->cseq.req, PSTR("REGISTER\0"), 9);

  bufptr = buf;
  
  sip_req_msg(&bufptr, PSTR("REGISTER"), sip);
  sip_add_via(&bufptr, &sip->via);
  sip_add_from(&bufptr, &sip->local); 
  sip_add_to(&bufptr, &sip->local);
  sip_add_call_id(&bufptr, sip->call_id); 
  sip_add_cseq(&bufptr, &sip->cseq);
  sip_add_allow(&bufptr);
  sip_add_max_forwards(&bufptr);
  sip_add_contact(&bufptr, &sip->local); 
  sip_add_expires(&bufptr); 	
  sip_add_content_length(&bufptr, 0);	
  fill_buf_P(&bufptr, linebreak);
  
  len = bufptr - buf;
  
  ip->to_addr = sip->abon.ip_addr;
  udp->from_port = htons(sip->local.port);
  udp->to_port = htons(sip->abon.port);
  
  udp_send(frame,len);
  
  call_state = STATE_EARLY;
}

void sip_ack(void)
{
  eth_frame_t *frame = net_frame();
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
	char *buf = (void*)(udp->data);
  sip_message_t *sip = &sip_descriptor;
  char *bufptr;
  uint16_t len;

  memcpy_P(sip->cseq.req, PSTR("ACK\0"), 4);
  
  sip->via.rport = 0;
  bufptr = buf;
   
  sip_req_msg(&bufptr, PSTR("ACK"), sip);
  sip_add_via(&bufptr, &sip->via);
  sip_add_from(&bufptr, &sip->local);   
  sip_add_to(&bufptr, &sip->abon);
  sip_add_call_id(&bufptr, sip->call_id); 
  sip_add_cseq(&bufptr, &sip->cseq);
  sip_add_max_forwards(&bufptr);
  sip_add_content_length(&bufptr, 0 );	
  fill_buf_P(&bufptr, linebreak);

  len = bufptr - buf;
  
  ip->to_addr = sip->abon.ip_addr;
  udp->from_port = htons(sip->local.port);
  udp->to_port = htons(sip->abon.port);
  
  udp_send(frame,len);
}

void sip_bye(void)
{
  eth_frame_t *frame = net_frame();
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
	char *buf = (void*)(udp->data);
  sip_message_t *sip = &sip_descriptor;
  char *bufptr, *tmp_ptr;
  uint16_t len;
  
  call_state = STATE_TERMINATED;

  tmp_ptr = sip->via.via_name;
	fill_buf(&tmp_ptr, sip->local.domain);
	
	sip->via.port = sip->local.port;
  sip->via.rport = 0;
  
  sip->cseq.ncseq++;
  memcpy_P(sip->cseq.req, PSTR("BYE\0"), 4);
  
  bufptr = buf;
  
  sip_req_msg(&bufptr, PSTR("BYE"), sip);
  sip_add_via(&bufptr, &sip->via);
  sip_add_to(&bufptr, &sip->abon);
  sip_add_from(&bufptr, &sip->local); 
  sip_add_call_id(&bufptr, sip->call_id); 
  sip_add_cseq(&bufptr, &sip->cseq);
  sip_add_max_forwards(&bufptr); 
 	sip_add_content_length(&bufptr, 0 );	
  fill_buf_P(&bufptr, linebreak);

  len = bufptr - buf;
  
  ip->to_addr = sip->abon.ip_addr;
  udp->from_port = htons(sip->local.port);
  udp->to_port = htons(sip->abon.port);
  
  udp_send(frame,len);
}

void sip_cancel(void)   { };
void sip_option(void)   { };

//------------------------------
void sip_200_ok_ringing(void)
{
  eth_frame_t *frame = net_frame();
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
	char *buf = (void*)(udp->data);
  sip_message_t *sip = &sip_descriptor;
  char *bufptr;
  static uint16_t len;
  
  call_state = STATE_ESTABLISHED;
  
  bufptr = buf;

  sdp_connection(get_local_ip(), sip_local_mport());
  
  sip->content_len = sdp_add(bufptr);

  fill_buf_P(&bufptr, PSTR("SIP/2.0 200 OK"));
  fill_buf_P(&bufptr, linebreak);
  
  sip_add_via(&bufptr, &sip->via); 
  sip_add_from(&bufptr, &sip->abon); 
  sip_add_to(&bufptr, &sip->local);
  sip_add_call_id(&bufptr, sip->call_id);  
  sip_add_cseq(&bufptr, &sip->cseq); 
  sip_add_contact(&bufptr, &sip->local);
  sip_add_content_type(&bufptr);	
  sip_add_content_length(&bufptr, sip->content_len );	
  fill_buf_P(&bufptr, linebreak);
  
  sdp_add(bufptr);
  
  len = bufptr - buf;
  len += sip->content_len;
    
  ip->to_addr = sip->abon.ip_addr;
  udp->from_port = htons(sip->local.port);
  udp->to_port = htons(sip->abon.port);
  
  udp_send(frame,len);
}

void sip_200_ok(void)
{
  eth_frame_t *frame = net_frame();
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
	char *buf = (void*)(udp->data);

  sip_message_t *sip = &sip_descriptor;
  char *bufptr;
  uint16_t len;
  
  bufptr = buf;

  fill_buf_P(&bufptr, PSTR("SIP/2.0 200 OK"));
  fill_buf_P(&bufptr, linebreak);
  
  sip_add_via(&bufptr, &sip->via);
  sip_add_from(&bufptr, &sip->abon); 
  sip_add_to(&bufptr, &sip->local);
  sip_add_call_id(&bufptr, sip->call_id);
  sip_add_cseq(&bufptr, &sip->cseq);
  sip_add_user_agent(&bufptr); 
  sip_add_max_forwards(&bufptr);  
  sip_add_content_length(&bufptr, 0 );	
  fill_buf_P(&bufptr, linebreak);
  
  len = bufptr - buf;
  
  ip->to_addr = sip->abon.ip_addr;
  udp->from_port = htons(sip->local.port);
  udp->to_port = htons(sip->abon.port);
  
  udp_send(frame,len);
}

void sip_100_trying(void)
{
  eth_frame_t *frame = net_frame();
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
	char *buf = (void*)(udp->data);
  sip_message_t *sip = &sip_descriptor;
  char *bufptr;
  uint16_t len;
   
  bufptr = buf;

  fill_buf_P(&bufptr, PSTR("SIP/2.0 100 Trying"));
  fill_buf_P(&bufptr, linebreak);
  
  sip_add_via(&bufptr, &sip->via);
  sip_add_call_id(&bufptr, sip->call_id);  
  sip_add_from(&bufptr, &sip->abon); 
  sip_add_to(&bufptr, &sip->local);
  sip_add_cseq(&bufptr, &sip->cseq); 
  sip_add_content_length(&bufptr, 0 );	
  fill_buf_P(&bufptr, linebreak);
  
  len = bufptr - buf;
  
  ip->to_addr = sip->abon.ip_addr;
  udp->from_port = htons(sip->local.port);
  udp->to_port = htons(sip->abon.port);
  
  udp_send(frame,len);
}

void sip_180_ringing(void)
{
  eth_frame_t *frame = net_frame();
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
	char *buf = (void*)(udp->data);
  sip_message_t *sip = &sip_descriptor;
  char *bufptr, *tmp_ptr;
  uint16_t len;
  
  tmp_ptr = sip->local.tag;
  ltoa_buff(&tmp_ptr, rnd()); 
  
  bufptr = buf;

  fill_buf_P(&bufptr, PSTR("SIP/2.0 180 Ringing"));
  fill_buf_P(&bufptr, linebreak);
  
  sip_add_via(&bufptr, &sip->via);
  sip_add_call_id(&bufptr, sip->call_id);  
  sip_add_from(&bufptr, &sip->abon); 
  sip_add_to(&bufptr, &sip->local);
  sip_add_cseq(&bufptr, &sip->cseq);
  sip_add_contact(&bufptr, &sip->local); 
  sip_add_content_length(&bufptr, 0 );	
  fill_buf_P(&bufptr, linebreak);
  
  len = bufptr - buf;
  
  ip->to_addr = sip->abon.ip_addr;
  udp->from_port = htons(sip->local.port);
  udp->to_port = htons(sip->abon.port);
  
  udp_send(frame,len);
}


// ---------------------

void parse_via(char *value, char *end, sip_via_t *via)
{
        char *tmp_ptr;
        char *tmp_ptr1;
        char *via_ptr;
        uint8_t via_len;
        uint8_t len1; 
        uint8_t len = end - value;
              
        extract_token(&value, end);
        extract_token(&value, end);
   
        tmp_ptr = memchr(value, ':', len);
        via_len = tmp_ptr - value;      
        memcpy(via->via_name, value, via_len);        
        via_ptr = via->via_name + via_len;
        *via_ptr = '\0';
        
        tmp_ptr++;
        via->port = atou(tmp_ptr);

        
        tmp_ptr = memchr(value, ';', len);

        if (tmp_ptr) {

          tmp_ptr++;
          
          if(!memcmp_P(tmp_ptr, PSTR("branch="), 7)) { 
          tmp_ptr += 7;
          
          len1= end - tmp_ptr;
          tmp_ptr1 = memchr(tmp_ptr, ';', len1);  
          via_len =   tmp_ptr1  - tmp_ptr;    
          memcpy(via->branch, tmp_ptr, via_len);
          return;
          }
          
          len = end -tmp_ptr;
          tmp_ptr1 = 0;
          tmp_ptr1 = memchr(tmp_ptr, ';', len);
          if (tmp_ptr1)    {  
               tmp_ptr1++;      
          if(!memcmp_P(tmp_ptr1, PSTR("branch="), 7)) { 
          tmp_ptr1 += 7;
          via_len =   end  - tmp_ptr1;    
          memcpy(via->branch, tmp_ptr1, via_len);
          }
          }
        }
}

void parse_from_to(char *value, char *end, sip_abon_t *abon)
{
        char *tmp_ptr; 
        char *ptr, *ptr_n;           
        uint8_t len, len_msg;
           
        len = end - value;     
        
        tmp_ptr = memchr(value, '<', len); 
        tmp_ptr += 5;      
        ptr = memchr(value, '@', len);       
        len_msg =  ptr -  tmp_ptr;
        memcpy(abon->user_name, tmp_ptr, len_msg);         
        ptr_n = abon->user_name + len_msg;
        *ptr_n = '\0';
        
        tmp_ptr = memchr(value, '>', len);  
        ptr++;        
        len_msg =  tmp_ptr -  ptr;                  
        memcpy(abon->domain, ptr, len_msg);                   
        ptr_n = abon->domain + len_msg;
        *ptr_n = '\0';  
          
                       
        tmp_ptr = memchr(value, ';', len);
        
        if (tmp_ptr) 
        {
          tmp_ptr += 5;
          len = end - tmp_ptr;
          memcpy(abon->tag, tmp_ptr, len);
          ptr_n = abon->tag + len;
          *ptr_n = '\0';  
        }
}

void parse_call_id(char *value, char *end, sip_message_t *descr)
{
	  uint8_t len;
	  char *tmp;
    extract_token(&value, end);
    len = end - value;
    memcpy(descr->call_id, value, len);
    tmp = descr->call_id + len;
    *tmp = '\0';
}


void parse_cseq(char *value, char *end, sip_message_t *descr)
{
	  uint8_t len;
	  char *tmp;
    extract_token(&value, end);
    descr->cseq.ncseq = atou(value);
    extract_token(&value, end);
    len = end - value;
    memcpy(descr->cseq.req, value, len);
    tmp = descr->cseq.req + len;
    *tmp = '\0';
}

void parse_content_len(char *value, char *end, sip_message_t *descr)
{
    extract_token(&value, end);
    if (!atou(value)) extract_token(&value, end);
    if (!atou(value)) extract_token(&value, end);
    descr->content_len = atou(value);
}

headers_t sip_headers(char *data)
{
    if(!memcmp_P(data, PSTR("Via"), 2)) return VIA;
    if(!memcmp_P(data, PSTR("To"), 2)) return TO;
    if(!memcmp_P(data, PSTR("From"), 4)) return FROM;
    if(!memcmp_P(data, PSTR("Call-ID"), 7)) return CALL_ID;
    if(!memcmp_P(data, PSTR("CSeq"), 4)) return CSEQ;
    if(!memcmp_P(data, PSTR("Content-Length"), 14)) return CONTENT_L;
    if(!memcmp_P(data, PSTR("\r\n"), 2)) return SDP_MSG;
   return NONE;
}

void sip_parse_header(char *data, uint16_t len, headers_t parse)
{
  char *ptr, *s_end, *line_end, *next_line, *value;
	headers_t header;		
  ptr = data;
  s_end = data + len;
 
  sip_message_t *sip = &sip_descriptor;
  
  while (ptr && ptr < s_end - 1) 
  {
      value = ptr;
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
          
        header = sip_headers(ptr);  
            
      if ( header ==	parse) {	
        switch (header) {
        
            case VIA:     
              parse_via(value, line_end, &sip->via);
              return;
            break;
            
            case TO:
              parse_from_to(value, line_end, &sip->abon);
              return;
            break;

            case FROM:
              parse_from_to(value, line_end, &sip->abon);
              return;
            break;

            case CALL_ID:
              parse_call_id(value, line_end, sip);
              return;
            break;  

            case CSEQ:
              parse_cseq(value, line_end, sip);
              return;
            break;
              
            case CONTENT_L:
              parse_content_len(value, line_end, sip);
              return;
            break;  
            
            case SDP_MSG:
              sdp_parse((value + 2), sip->content_len);
              return;
            break;               
            
            default:
            break;				
        }  
		}
	  ptr= next_line;                     
	}
}

sip_answer_t sip_parse(char *data, uint16_t len)
{
  char *tmp_ptr = data;
  sip_answer_t answ = SIP_NOTHING;
  uint16_t mess_code;
  
  if(!memcmp_P(tmp_ptr, PSTR("SIP/2.0"), 7))     
  {
          extract_token(&tmp_ptr, data + len);
          mess_code = atou(tmp_ptr);
          
          		switch (mess_code) {

                  case 200:
                    answ =  SIP_200_OK;
                  break;

                  case 100:
                    answ =  SIP_100_Trying;
                  break;

                  case 180:
                    answ =  SIP_180_Ringing;
                  break;

                  case 486:
                    answ =  SIP_486_Busy_Here;
                  break;
    
                  case 603:
                    answ =  SIP_603_Decline;
                  break;

                  case 183:
                    answ =  SIP_183_Session_Progress;
                  break;

                  default:
                    answ =  SIP_NOTHING;                  
                  break;				
              }
   }
   else
   {
        if(!memcmp_P(tmp_ptr, PSTR("BYE"), 3))  answ =  SIP_BYE;
	      if(!memcmp_P(tmp_ptr, PSTR("INVITE"), 6)) answ =  SIP_INVITE;
        if(!memcmp_P(tmp_ptr, PSTR("ACK"), 3))   answ =  SIP_ACK;
        if(!memcmp_P(tmp_ptr, PSTR("CANCEL"), 6))   answ =  SIP_CANCEL;
        if(!memcmp_P(tmp_ptr, PSTR("NOTIFY"), 6))   answ =  SIP_NOTIFY;
        if(!memcmp_P(tmp_ptr, PSTR("SUBSCRIBE"), 9))   answ =  SIP_SUBSCRIBE;
   } 
return answ;
}

// ============== PHONE =====================
void sip_phone(eth_frame_t *frame, uint16_t len)
{
  ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
  char *data = (void*)(udp->data);  
    
  sip_answer_t msg_answer = sip_parse(data,len);

  sip_set_abon_port(ntohs(udp->from_port));
  sip_set_via_rport(ntohs(udp->from_port));
  
  switch(msg_answer)
			{
			case SIP_INVITE:
              call_state = STATE_INCOMING;
              sip_parse_header(data,len, VIA);  
              sip_parse_header(data,len, FROM); 
              sip_parse_header(data,len, CALL_ID); 
              sip_parse_header(data,len, CSEQ);               
              sip_parse_header(data,len, CONTENT_L); 
              sip_parse_header(data,len, SDP_MSG);
              sip_set_abon_mport(sdp_rport());
              sip_dom2ip();
              //sip_set_abon_ip(ip->from_addr);  
              sip_100_trying();
              call_state = STATE_RINGING;
              sip_180_ringing();                          
				break;
				
			case SIP_ACK:       
              call_state = STATE_ESTABLISHED;
              rtp_init_send(sip_abon_ip(), sip_abon_mport(), sip_local_mport(), PCMU, 160);
              rtp_init_receive(sip_local_mport());
              audio_run();               
				break;
								
			case SIP_BYE:
              sip_parse_header(data,len, VIA);
              sip_parse_header(data,len, CSEQ);  
              call_state = STATE_TERMINATED;   
              rtp_deinit_receive(sip_local_mport());
              audio_stop();            
              sip_200_ok();
              call_state = STATE_IDLE;
				break;

			case SIP_CANCEL:
              call_state = STATE_IDLE;
              sip_parse_header(data,len, VIA);
              sip_parse_header(data,len, FROM); 
              sip_parse_header(data,len, CALL_ID);   
              sip_parse_header(data,len, CSEQ);               
              sip_200_ok();
              call_state = STATE_IDLE;
				break;

			case SIP_NOTIFY:
              sip_parse_header(data,len, VIA);
              sip_parse_header(data,len, FROM);  
              sip_parse_header(data,len, CALL_ID);   
              sip_parse_header(data,len, CSEQ);               
              sip_200_ok();
				break;

			case SIP_SUBSCRIBE:
              sip_parse_header(data,len, VIA);
              sip_parse_header(data,len, FROM);  
              sip_parse_header(data,len, CALL_ID);   
              sip_parse_header(data,len, CSEQ);               
              sip_200_ok();
				break;
				
			case SIP_200_OK:
			
          if( call_state == STATE_OUTGOING)
          {
              call_state = STATE_ESTABLISHED;
              sip_parse_header(data,len, CONTENT_L); 
              sip_parse_header(data,len, SDP_MSG);
              sip_set_abon_mport(sdp_rport());            
              rtp_init_send(sdp_raddres(), sip_abon_mport(), sip_local_mport(), PCMU, 160);
              rtp_init_receive(sip_local_mport());
              audio_run();
              sip_ack();                
          }
          else
          {
              if( call_state == STATE_EARLY)
              {
            //  led_on(LED_2);
              }
              else
              {
              rtp_deinit_receive(sip_local_mport());
              audio_stop();           
              }
              call_state = STATE_IDLE;              
          }          
				break;

			case SIP_486_Busy_Here:		
            call_state = STATE_TERMINATED;  
            sip_parse_header(data,len, CSEQ);
            sip_ack();
            call_state = STATE_IDLE;       
				break;
				
			case SIP_603_Decline:		
            call_state = STATE_TERMINATED;  
            sip_parse_header(data,len, CSEQ);
            sip_ack();
            call_state = STATE_IDLE;       
				break;	
							
			case SIP_180_Ringing:	
            sip_parse_header(data,len, TO); 
            led_on(LED_2);  	 				     
				break;
				
			case SIP_183_Session_Progress:	
            sip_parse_header(data,len, TO); 
            led_on(LED_2);  	 				     
				break;

			case SIP_100_Trying:	 	     
				break;

      default:    
      break;	
			} 
}

uint32_t sip_abon_ip(void) { return sip_descriptor.abon.ip_addr; }
uint16_t sip_local_mport(void) { return sip_descriptor.local.mport; }
uint16_t sip_abon_mport(void) { return sip_descriptor.abon.mport; }
void sip_set_abon_ip(uint32_t ip) { sip_descriptor.abon.ip_addr = ip; }
void sip_set_via_rport(uint16_t port) { sip_descriptor.via.rport = port; }
void sip_set_abon_port(uint16_t port) { sip_descriptor.abon.port = port; }
void sip_set_abon_mport(uint16_t port) { sip_descriptor.abon.mport = port; }

// _______________tests____________________
void sip_test_call(void) { sip_invite(); }
void sip_test_bye(void)  { sip_bye(); }
void sip_test_ack(void)  { sip_200_ok_ringing();}
void sip_test_register(void) {  sip_register(); }

// _______________tests____________________

void sip_dom2ip(void)
{ 
  sip_descriptor.abon.ip_addr = atoip(sip_descriptor.abon.domain);
}
// --------------- init -------------------

void sip_put_my(char *name, uint8_t len)
{
   char *tmp;
   memcpy(sip_descriptor.local.user_name, name, len);
   tmp = sip_descriptor.local.user_name + len;
   *tmp = '\0';  
}

void sip_put_my_domain(char *name, uint8_t len)
{
   char *tmp;
   memcpy(sip_descriptor.local.domain, name, len);
   tmp = sip_descriptor.local.user_name + len;
   *tmp = '\0';  
}

void sip_put_abon(char *name, uint8_t len)
{
   char *tmp;
   memcpy(sip_descriptor.abon.user_name, name, len);
   tmp = sip_descriptor.abon.user_name + len;
   *tmp = '\0';  
}

void sip_put_abon_domain(char *name, uint8_t len)
{
   char *tmp;
   memcpy(sip_descriptor.abon.domain, name, len);
   tmp = sip_descriptor.abon.domain + len;
   *tmp = '\0'; 
   sip_descriptor.abon.port = SIP_PORT;
   sip_descriptor.abon.ip_addr = atoip(sip_descriptor.abon.domain); 
}


sip_state_t sip_call_state(void)
{
return call_state;
}

void sip_init(void)
{
  call_state = STATE_IDLE;  
  sip_message_t *sip = &sip_descriptor;

  //local
  memcpy_P(sip->local.user_name, PSTR("3000\0"), 4);
  sip->local.ip_addr = get_local_ip();
  sip->local.mport = (uint16_t)(rnd()>>20);
  sip->local.port = (uint16_t)(rnd()>>20);
  memcpy_P(sip->local.domain, PSTR("192.168.0.222\0"), 14);  
     
  // remote
  memcpy_P(sip->abon.user_name, PSTR("3001\0"), 5);
  memcpy_P(sip->abon.domain, PSTR("192.168.0.1\0"), 12);
  sip->abon.port = SIP_PORT;
  sip->abon.ip_addr = atoip(sip->abon.domain);
  
  udp_listen(SIP_PORT, sip_phone);
  udp_listen(sip->local.port, sip_phone);
}
// --------------- init -------------------




void test_sip(eth_frame_t *frame, uint16_t len)
{
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
  char *data = (void*)(udp->data);
  char *bufptr;
  bufptr = data;
   
  if(!memcmp_P(data, PSTR("INVITE"), 6)) {
  sip_test_call();
  }
  
  if(!memcmp_P(data, PSTR("BYE"), 3)) {
  sip_test_bye();  
  /*
  ip->to_addr = inet_addr(192,168,0,1);
  udp->from_port = htons(3333);
  udp->to_port = htons(3333);
  fill_buf_P(&bufptr, PSTR("rcv/procces packets: "));
  fill_buf_P(&bufptr, linebreak);
  ltoa_buff(&bufptr, rcv_packet);
  fill_buf_P(&bufptr, linebreak);
  ltoa_buff(&bufptr, proc_packet);
  len = bufptr - data;		
  udp_send(frame,len);
  */
  }
 
    if(!memcmp_P(data, PSTR("ACKR"), 4)) {
  sip_test_register();  
  }
 
  if(!memcmp_P(data, PSTR("ACK"), 3)) {
  sip_test_ack();  
  }
  
}

void sip_controll_init(void){ udp_listen(SIP_CONTROLL_PORT, test_sip); }



char *sip_get_m_name(void)
{
return sip_descriptor.local.user_name;
}

char *sip_get_m_domain(void)
{
return sip_descriptor.local.domain;
}

char *sip_get_a_name(void)
{
return sip_descriptor.abon.user_name;
}

char *sip_get_a_domain(void)
{
return sip_descriptor.abon.domain;
}
