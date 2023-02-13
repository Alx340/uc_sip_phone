#include "rnd.h"


uint32_t rs_randseed;


void srnd(void){
    rs_randseed = 31415;
	//if (gettc()) rs_randseed = gettc();
}


uint32_t rnd(void){
	rs_randseed++;
	return (rs_randseed*0xF02CE551UL) + 0x7FF2742BUL;
}
