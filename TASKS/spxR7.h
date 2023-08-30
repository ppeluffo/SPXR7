/* 
 * File:   spxR7.h
 * Author: pablo
 *
 * Created on August 23, 2023, 4:50 PM
 */

#ifndef SPXR7_H
#define	SPXR7_H

#ifdef	__cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------------
// INCLUDES
//------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <compat/deprecated.h>
#include <pgmspace.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sleep.h>
#include <avr_compiler.h>
#include <clksys_driver.h>
#include <pmic_driver.h>
#include <TC_driver.h>
#include <wdt_driver.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "croutine.h"
#include "semphr.h"
#include "timers.h"
#include "limits.h"
#include "portable.h"

#include "frtos-io.h"
    
#include "xprintf.h"
#include "led.h"
#include "xgetc.h"
#include "i2c.h"
#include "rtc79410.h"
#include "eeprom.h"
#include "ina3221.h"
    
//------------------------------------------------------------------------------
// DEFINES
//------------------------------------------------------------------------------
#define FW_REV "1.0.0"
#define FW_DATE "@ 20230830"
#define HW_MODELO "SPXR7 FRTOS R001 HW:XMEGA256A3B"
#define FRTOS_VERSION "FW:FreeRTOS V202111.00"
#define FW_TYPE "SPXR7"

#if configUSE_TICKLESS_IDLE == 2
#define SPX_FTROS_VERSION "FFW:FreeRTOS V202111.00 TICKLESS"
#else
#define SPX_FTROS_VERSION "FW:FRTOS10"
#endif

//#define F_CPU (32000000UL)
//#define SYSMAINCLK 2
//#define SYSMAINCLK 8
#define SYSMAINCLK 32
    
#define tkCtl_STACK_SIZE		384
#define tkCmd_STACK_SIZE		384
    
StaticTask_t xTask_Ctl_Buffer_Ptr;
StackType_t xTask_Ctl_Buffer [tkCtl_STACK_SIZE];

StaticTask_t xTask_Cmd_Buffer_Ptr;
StackType_t xTask_Cmd_Buffer [tkCmd_STACK_SIZE];

#define tkCtl_TASK_PRIORITY	 	( tskIDLE_PRIORITY + 1 )
#define tkCmd_TASK_PRIORITY 	( tskIDLE_PRIORITY + 1 )

TaskHandle_t xHandle_idle, xHandle_tkCtl, xHandle_tkCmd;

SemaphoreHandle_t sem_SYSVars;
StaticSemaphore_t SYSVARS_xMutexBuffer;

#define MSTOTAKESYSVARSSEMPH ((  TickType_t ) 10 )

void tkCtl(void * pvParameters);
void tkCmd(void * pvParameters);

bool run_tasks;

void system_init_outofrtos(void);
void reset(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SPXR7_H */

