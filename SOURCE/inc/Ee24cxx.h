#ifndef _EE24CXX_H
#define _EE24CXX_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"
#include "rtc.h"

/* Define --------------------------------------------------------------------*/


void Ee24cxxReadConfig(void);
void Ee24cxxRead(uint8_t* data);
void Ee24cxxWrite(uint8_t *data);
void Ee24cxxInitt(void);

#endif /* _EE24CXX_H */
