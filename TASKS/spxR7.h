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
#include "nvmee.h"
    
#include "ainputs.h"
#include "contadores.h"

//------------------------------------------------------------------------------
// DEFINES
//------------------------------------------------------------------------------
#define FW_REV "1.0.0"
#define FW_DATE "@ 20230905"
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
#define tkSys_STACK_SIZE		384
    
StaticTask_t xTask_Ctl_Buffer_Ptr;
StackType_t xTask_Ctl_Buffer [tkCtl_STACK_SIZE];

StaticTask_t xTask_Cmd_Buffer_Ptr;
StackType_t xTask_Cmd_Buffer [tkCmd_STACK_SIZE];

StaticTask_t xTask_Sys_Buffer_Ptr;
StackType_t xTask_Sys_Buffer [tkSys_STACK_SIZE];


#define tkCtl_TASK_PRIORITY	 	( tskIDLE_PRIORITY + 1 )
#define tkCmd_TASK_PRIORITY 	( tskIDLE_PRIORITY + 1 )
#define tkSys_TASK_PRIORITY 	( tskIDLE_PRIORITY + 1 )

TaskHandle_t xHandle_idle, xHandle_tkCtl, xHandle_tkCmd, xHandle_tkSys;

SemaphoreHandle_t sem_SYSVars;
StaticSemaphore_t SYSVARS_xMutexBuffer;

#define MSTOTAKESYSVARSSEMPH ((  TickType_t ) 10 )

void tkCtl(void * pvParameters);
void tkCmd(void * pvParameters);
void tkSys(void * pvParameters);

typedef struct {
    float l_ainputs[NRO_ANALOG_CHANNELS];
    float l_counters[NRO_COUNTER_CHANNELS];
    float battery;
    RtcTimeType_t  rtc;	
} dataRcd_s;

struct {   
    bool debug;
    float ainputs[NRO_ANALOG_CHANNELS];
    float counters[NRO_COUNTER_CHANNELS];
    float battery;
} systemVars;

typedef enum { WAN_RS485B = 0, WAN_NBIOT } wan_port_t;

typedef enum { PWR_CONTINUO = 0, PWR_DISCRETO, PWR_MIXTO } pwr_modo_t;

struct {
    uint16_t timerpoll;
    uint16_t timerdial;
    pwr_modo_t pwr_modo;
    uint16_t pwr_hhmm_on;
    uint16_t pwr_hhmm_off;
    uint8_t samples_count;      // Nro. de muestras para promediar una medida
	ainputs_conf_t ainputs_conf;
    counters_conf_t counters_conf;
    
    // El checksum SIEMPRE debe ser el ultimo byte !!!!!
    uint8_t checksum;
    
} systemConf;
bool run_tasks;

#define TDIAL_MIN_DISCRETO  900

void system_init_outofrtos(void);
void reset(void);
void config_default(void);
bool config_debug( char *tipo, char *valor);
bool save_config_in_NVM(void);
bool load_config_from_NVM(void);
uint8_t checksum( uint8_t *s, uint16_t size );
bool config_timerdial ( char *s_timerdial );
bool config_timerpoll ( char *s_timerpoll );
bool config_samples ( char *s_samples );
bool poll_data(dataRcd_s *dataRcd);
void xprint_dr(dataRcd_s *dr);
dataRcd_s *get_system_dr(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SPXR7_H */

