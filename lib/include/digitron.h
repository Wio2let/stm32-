#ifndef __DIGITRON_H
#define __DIGITRON_H

#include "main.h"

#ifdef DIGITRON_GLOBAL
	#define DIGITRON_EXTERN
#else
	#define DIGITRON_EXTERN extern
#endif

void latchClk(void);
void chooseDigitron(uint8_t);
void digitronPutBuf(uint8_t, uint8_t );
void putData(uint8_t);
void displayToken(uint8_t, uint8_t);
void showPause();
void showEnd();
void showStart();

#endif
