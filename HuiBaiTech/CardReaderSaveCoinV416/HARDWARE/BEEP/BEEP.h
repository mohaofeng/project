#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"


#ifdef BEEP_C	
#define BEEP_EXT 
#else
#define BEEP_EXT extern
#endif



#define NO_BEEP   0x00
#define BB   0x01
#define B   0x02
#define B_   0x03

#define BEEP_PIN PAout(8)// PB5   v1.3







void BeepTask(void);
void BEEP_Init(void);
void BeepCtr(uint16_t sta);

		 				    
#endif
