#pragma once

#include "net.h"
#include "utils.h"
#include "sdp.h"
#include "rtp.h"
#include "rnd.h"
#include "audio.h"
#include <avr/pgmspace.h>
#include "led.h"


#define SIP_PORT		          	5060
#define SIP_CONTROLL_PORT       3333

#define SIP_DEFAULT_ABON_NAME		    
#define SIP_DEFAULT_ABON_DOMAIN	    

#define SIP_DEFAULT_LOCAL_NAME		    
#define SIP_DEFAULT_LOCAL_DOMAIN	    


#define SIP_USER_NAME_LEN		    10
#define SIP_TAG_LEN		          40
#define SIP_DOMAIN_LEN		      20
#define SIP_VIA_NAME_LEN		    20
#define SIP_BRANCH_LEN		      42
#define SIP_REQ_LEN		          10
#define SIP_CALL_ID_LEN		      64


typedef struct sip_abon {
	char user_name[SIP_USER_NAME_LEN];
	char tag[SIP_TAG_LEN];
	uint32_t ip_addr;
	uint16_t mport;
	uint16_t port;
	char domain[SIP_DOMAIN_LEN];
} sip_abon_t;


typedef struct sip_via {
	char via_name[SIP_VIA_NAME_LEN];
	char branch[SIP_BRANCH_LEN];
	uint16_t port;
	uint16_t rport;			
} sip_via_t;


typedef struct sip_cseq {
	char req[SIP_REQ_LEN];
	uint16_t ncseq;
} sip_cseq_t;


typedef struct sip_message {
	sip_via_t via;
	sip_abon_t local;
	sip_abon_t abon;
	char call_id[SIP_CALL_ID_LEN];
	sip_cseq_t cseq;
	uint16_t content_len;
} sip_message_t;


typedef enum headers {
  VIA,
	TO,
	FROM,
	CALL_ID,
	CSEQ,
	CONTENT_L,
	SDP_MSG,
	NONE
} headers_t;


typedef enum sip_answer {
  SIP_NOTHING = 0,
	SIP_INVITE,
	SIP_ACK,
	SIP_BYE,
	SIP_CANCEL,
	SIP_REGISTER,
	SIP_OPTION,
	SIP_NOTIFY,
	SIP_SUBSCRIBE,
	SIP_200_OK,
  SIP_100_Trying,
	SIP_180_Ringing,
	SIP_486_Busy_Here,
  SIP_603_Decline,
  SIP_183_Session_Progress
} sip_answer_t;


typedef enum sip_state {
	STATE_IDLE = 0,
	STATE_INCOMING,
	STATE_OUTGOING,
	STATE_RINGING,
	STATE_EARLY,
	STATE_ESTABLISHED,
	STATE_TERMINATED
} sip_state_t;


void sip_phone(eth_frame_t *frame, uint16_t len);
void sip_init(void);


uint32_t sip_abon_ip(void);
uint16_t sip_local_mport(void);
uint16_t sip_abon_mport(void);
void sip_set_abon_ip(uint32_t ip);
void sip_set_via_rport(uint16_t port);
void sip_set_abon_port(uint16_t port);
void sip_set_abon_mport(uint16_t port);
void sip_dom2ip(void);
sip_state_t sip_call_state(void);


void sip_controll_init(void);
void test_sip(eth_frame_t *frame, uint16_t len);
void sip_test_call(void);
void sip_test_bye(void);
void sip_test_ack(void);
void sip_test_register(void);


char *sip_get_m_name(void);
char *sip_get_m_domain(void);
char *sip_get_a_name(void);
char *sip_get_a_domain(void);

void sip_put_my(char *name, uint8_t len);
void sip_put_my_domain(char *name, uint8_t len);
void sip_put_abon(char *name, uint8_t len);
void sip_put_abon_domain(char *name, uint8_t len);