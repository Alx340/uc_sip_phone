#pragma once

#include "net.h"
#include "rnd.h"

#define RTP_DEFAULT_PORT      5000
#define RTCP_DEFAULT_PORT     5001

#define RTP_VERSION   2
#define RTP_HEADER_LEN   12
#define PCMA 8
#define PCMU 0

typedef struct rtp_packet {
  uint8_t csrc_count:4;  
  uint8_t extension:1;    
  uint8_t padding:1;     
  uint8_t version:2;       
  uint8_t payload_type:7;  
  uint8_t marker:1;        
  uint16_t seq;                  
  uint32_t timestamp;           
  uint32_t ssrc;                
	uint8_t data[];
} rtp_packet_t;

typedef enum rtp_status {
	RTP_IDLE,
	RTP_RECEIVE_PACKET,
	RTP_SEND_PACKET
} rtp_status_t;

typedef struct rtp_context {
	uint32_t timestamp;
	uint16_t seq;
	uint8_t pt;
	uint32_t ssrc;
	uint32_t remote_addr;
	uint16_t remote_port;
	uint16_t local_port;
	uint16_t nsample;
	uint16_t pt_len;
	rtp_status_t status;
} rtp_context_t;


void rtp_init_send(uint32_t remote_addr, uint16_t port, uint16_t lport, uint8_t pt, uint8_t nsample);
void rtp_init_receive(uint16_t port);
void rtp_deinit_receive(uint16_t port);

uint8_t *rtp_payload(eth_frame_t *frame);
uint8_t rtp_receive_len(void);

void rtp_send(eth_frame_t *frame, uint16_t len);
void rtp_receive(eth_frame_t *frame, uint16_t len);

uint8_t rtp_sync_read(void);
uint8_t rtp_sync_write(void);
void rtp_read(void);
void rtp_write(void);