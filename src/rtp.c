#include "rtp.h"

rtp_context_t rtp_pool;
rtp_context_t rtp_source_pool;
rtp_status_t rtp_wstatus;
rtp_status_t rtp_rstatus;

void rtp_init_send(uint32_t remote_addr, uint16_t port, uint16_t lport, uint8_t pt, uint8_t nsample)
{
	  rtp_context_t *context = &rtp_pool;
		context->remote_addr = remote_addr;
    context->local_port = lport;
    context->remote_port = port;
    context->timestamp = rnd();
    context->seq = rnd();
    context->ssrc = rnd();
    context->pt = pt;
    context->nsample = nsample;
}

void rtp_init_receive(uint16_t port)
{
udp_listen(port, rtp_receive);
}

void rtp_deinit_receive(uint16_t port)
{
rtp_source_pool.status = RTP_IDLE;
udp_unlisten(port);
}

uint8_t *rtp_payload(eth_frame_t *frame)
{
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
  rtp_packet_t *rtp = (void*)(udp->data);
  uint8_t *data = (void*)(rtp->data);
  return data;
}

uint8_t rtp_receive_len(void)
{
  return (rtp_source_pool.pt_len);
}

void rtp_send(eth_frame_t *frame, uint16_t len)
{
	rtp_context_t *context = &rtp_pool;
	ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
  rtp_packet_t *rtp = (void*)(udp->data);
  
  ip->to_addr = context->remote_addr;
  udp->from_port = htons(context->local_port);
  udp->to_port = htons(context->remote_port);
  
  rtp->version = RTP_VERSION;  
  rtp->padding = 0;  
  rtp->extension = 0;
  rtp->csrc_count = 0;
  rtp->marker = 0;
  rtp->payload_type = context->pt;
  rtp->seq = htons(context->seq);
  rtp->timestamp = htonl(context->timestamp);
  rtp->ssrc = htonl(context->ssrc);
  
  context->seq++;
  context->timestamp += context->nsample;
	len += RTP_HEADER_LEN; 
	
  udp_send(frame,len);
}

void rtp_receive(eth_frame_t *frame, uint16_t len)
{
  ip_packet_t *ip = (void*)(frame->data);
	udp_packet_t *udp = (void*)(ip->data);
  rtp_packet_t *rtp = (void*)(udp->data);
  rtp_context_t *context = &rtp_source_pool;
  
  if (rtp->version != RTP_VERSION) return;
  
	if (context->ssrc != ntohl(rtp->ssrc)) {
	
       if (context->status == RTP_IDLE) 
              {
                context->status = RTP_RECEIVE_PACKET;
                context->ssrc = ntohl(rtp->ssrc);
                context->remote_port = ntohs(udp->from_port);
                context->local_port = ntohs(udp->to_port);
                context->remote_addr = ip->from_addr;
                context->timestamp = ntohl(rtp->timestamp);
                context->pt = rtp->payload_type;
                context->pt_len = len - RTP_HEADER_LEN; 
              }     
  }
  else 
  {
       if(ip->from_addr == context->remote_addr) 
       {
          context->timestamp = ntohl(rtp->timestamp);
          context->pt_len = len - RTP_HEADER_LEN; 
          rtp_read(); ;
          //rcv_packet++; 
       }
  }   
}

uint8_t rtp_sync_read(void)
{
  if(rtp_rstatus == RTP_RECEIVE_PACKET) 
  {
    rtp_rstatus = RTP_IDLE;
    return 1;
  }
  return 0;
}

uint8_t rtp_sync_write(void)
{
  if(rtp_wstatus == RTP_SEND_PACKET) 
  {
    rtp_wstatus = RTP_IDLE;
    return 1;
  }
  return 0;
}

void rtp_read(void)  { rtp_rstatus = RTP_RECEIVE_PACKET;}
void rtp_write(void) { rtp_wstatus = RTP_SEND_PACKET;}
