/* 
 * File:   contadores.h
 * Author: pablo
 *
 * Created on July 19, 2023, 5:04 PM
 */

#ifndef CONTADORES_H
#define	CONTADORES_H

#ifdef	__cplusplus
extern "C" {
#endif


#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
    
#include "xprintf.h"
#include "ina3221.h"    
#include "pines.h"
#include "utils.h"

#define CNT0_PORT	PORTB
#define CNT0_PIN    1
#define CNT0_PIN_bm	PIN1_bm
#define CNT0_PIN_bp	PIN1_bp
    
#define CNT1_PORT	PORTB
#define CNT1_PIN    2
#define CNT1_PIN_bm	PIN2_bm
#define CNT1_PIN_bp	PIN2_bp
    
// Los CNTx son inputs
#define CNT0_CONFIG()    ( CNT0_PORT.DIR &= ~CNT0_PIN_bm )
#define CNT1_CONFIG()    ( CNT1_PORT.DIR &= ~CNT1_PIN_bm )

#define CNT_PARAMNAME_LENGTH	12
#define NRO_COUNTER_CHANNELS     2

#define MAX_RB_CAUDAL_STORAGE_SIZE  5
    
typedef enum { CAUDAL = 0, PULSOS } t_counter_modo;

// Configuracion de canales de contadores
typedef struct {
    bool enabled;
	char name[CNT_PARAMNAME_LENGTH];
	float magpp;
    t_counter_modo modo_medida;
    uint8_t rb_size;
} counter_channel_conf_t;

typedef struct {
    counter_channel_conf_t channel[NRO_COUNTER_CHANNELS];
} counters_conf_t;

void counters_init_outofrtos( SemaphoreHandle_t semph);
void counters_update_local_config( counters_conf_t *counters_system_conf);
void counters_read_local_config( counters_conf_t *counters_system_conf);
void counters_init( void );
void counters_config_defaults( void );
void counters_print_configuration( void );
bool counters_config_channel( uint8_t ch, char *s_enable, char *s_name, char *s_magpp, char *s_modo, char *s_rb_size );
void counters_config_debug(bool debug );
bool counters_read_debug(void);
void counter_FSM(uint8_t i );
void counters_clear(void);
void counters_convergencia(void);
uint8_t counters_read_pin(uint8_t cnt);
void counters_read( float *l_counters );
uint8_t CNT0_read(void);
uint8_t CNT1_read(void);
uint8_t counters_hash( void );

void counters_test_rb(char *data);

#ifdef	__cplusplus
}
#endif

#endif	/* CONTADORES_H */

