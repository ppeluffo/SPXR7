/*
 * l_nvm.h
 *
 *  Created on: 18 feb. 2019
 *      Author: pablo
 */

#ifndef SRC_SPX_LIBS_L_NVM_L_NVM_H_
#define SRC_SPX_LIBS_L_NVM_L_NVM_H_


#include "frtos-io.h"
#include "stdio.h"
#include "xprintf.h"

char *NVMEE_read_serial( void );
char *NVMEE_read_device_ID( void );

int16_t NVMEE_read  ( uint16_t dataAddress, char *data, uint8_t data_length );
int16_t NVMEE_write ( uint16_t dataAddress, char *data, uint8_t data_length );

void NVMEE_test_read( char *addr, char *size );
void NVMEE_test_write( char *addr, char *str );


#endif /* SRC_SPX_LIBS_L_NVM_L_NVM_H_ */
