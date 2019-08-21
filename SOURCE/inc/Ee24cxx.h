#ifndef _EE24CXX_H
#define _EE24CXX_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "setting.h"


/* Define --------------------------------------------------------------------*/

void Ee24cxxRead(uint8_t* data);
void Ee24cxxWrite(uint8_t *data);
void Ee24cxxInit(void);

#endif /* _EEPROM24CXX_H */
