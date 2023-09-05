
#include "spxR7.h"

void RTC32_ToscEnable( bool use1khz );
void configure_systemMainClock(void);
void configure_RTC32(void);

//------------------------------------------------------------------------------
void system_init_outofrtos(void)
{
    
 	// Clock principal del sistema
	configure_systemMainClock();
    
#if configUSE_TICKLESS_IDLE == 2
	configure_RTC32();
#endif

	// Configuramos y habilitamos el watchdog a 8s.
	WDT_EnableAndSetTimeout(  WDT_PER_8KCLK_gc );
	if ( WDT_IsWindowModeEnabled() )
		WDT_DisableWindowMode();

	// Las interrupciones de UART GPRS deben ser HighLevelPriority
	PMIC_EnableHighLevel();

	set_sleep_mode(SLEEP_MODE_PWR_SAVE);

	LED_init();
    XPRINTF_init();
    I2C_init();
    
    // Creo los semaforos
	sem_SYSVars = xSemaphoreCreateMutexStatic( &SYSVARS_xMutexBuffer );

    ainputs_init_outofrtos(sem_SYSVars);
    counters_init_outofrtos(sem_SYSVars);
    
}
//------------------------------------------------------------------------------
void configure_systemMainClock(void)
{
/*	Configura el clock principal del sistema
	Inicialmente se arranca en 2Mhz.
	La configuracion del reloj tiene 2 componentes: el clock y el oscilador.
	OSCILADOR:
	Primero debo elejir cual oscilador voy a usar para alimentar los prescalers que me den
	el clock del sistema y esperar a que este este estable.
	CLOCK:
	Elijo cual oscilador ( puedo tener mas de uno prendido ) va a ser la fuente principal
	del closck del sistema.
	Luego configuro el prescaler para derivar los clocks de los perifericos.
	Puedo por ultimo 'lockear' esta configuracion para que no se cambie por error.
	Los registros para configurar el clock son 'protegidos' por lo que los cambio
	utilizando la funcion CCPwrite.

	Para nuestra aplicacion vamos a usar un clock de 32Mhz.
	Como vamos a usar el ADC debemos prestar atencion al clock de perifericos clk_per ya que luego
	el ADC clock derivado del clk_per debe estar entre 100khz y 1.4Mhz ( AVR1300 ).

	Opcionalmente podriamos deshabilitar el oscilador de 2Mhz para ahorrar energia.
*/

#if SYSMAINCLK == 32
	// Habilito el oscilador de 32Mhz
	OSC.CTRL |= OSC_RC32MEN_bm;

	// Espero que este estable
	do {} while ( (OSC.STATUS & OSC_RC32MRDY_bm) == 0 );

	// Seteo el clock para que use el oscilador de 32Mhz.
	// Uso la funcion CCPWrite porque hay que se cuidadoso al tocar estos
	// registros que son protegidos.
	CCPWrite(&CLK.CTRL, CLK_SCLKSEL_RC32M_gc);
	//
	// El prescaler A ( CLK.PSCCTRL ), B y C ( PSBCDIV ) los dejo en 0 de modo que no
	// hago division y con esto tengo un clk_per = clk_sys. ( 32 Mhz ).
	//
#endif

#if SYSMAINCLK == 8
	// Habilito el oscilador de 32Mhz y lo divido por 4
	OSC.CTRL |= OSC_RC32MEN_bm;

	// Espero que este estable
	do {} while ( (OSC.STATUS & OSC_RC32MRDY_bm) == 0 );

	// Seteo el clock para que use el oscilador de 32Mhz.
	// Uso la funcion CCPWrite porque hay que se cuidadoso al tocar estos
	// registros que son protegidos.
	CCPWrite(&CLK.CTRL, CLK_SCLKSEL_RC32M_gc);
	//
	// Pongo el prescaler A por 4 y el B y C en 0.
	CLKSYS_Prescalers_Config( CLK_PSADIV_4_gc, CLK_PSBCDIV_1_1_gc );

	//
#endif

#if SYSMAINCLK == 2
	// Este es el oscilador por defecto por lo cual no tendria porque configurarlo.
	// Habilito el oscilador de 2Mhz
	OSC.CTRL |= OSC_RC2MEN_bm;
	// Espero que este estable
	do {} while ( (OSC.STATUS & OSC_RC2MRDY_bm) == 0 );

	// Seteo el clock para que use el oscilador de 2Mhz.
	// Uso la funcion CCPWrite porque hay que se cuidadoso al tocar estos
	// registros que son protegidos.
	CCPWrite(&CLK.CTRL, CLK_SCLKSEL_RC2M_gc);
	//
	// El prescaler A ( CLK.PSCCTRL ), B y C ( PSBCDIV ) los dejo en 0 de modo que no
	// hago division y con esto tengo un clk_per = clk_sys. ( 2 Mhz ).
	//
#endif

//#ifdef configUSE_TICKLESS_IDLE
	// Para el modo TICKLESS
	// Configuro el RTC con el osc externo de 32Khz
	// Pongo como fuente el xtal externo de 32768 contando a 32Khz.
	//CLK.RTCCTRL = CLK_RTCSRC_TOSC32_gc | CLK_RTCEN_bm;
	//do {} while ( ( RTC.STATUS & RTC_SYNCBUSY_bm ) );

	// Disable RTC interrupt.
	// RTC.INTCTRL = 0x00;
	//
	// Si uso el RTC32, habilito el oscilador para 1ms.

	RTC32_ToscEnable(true);
//#endif

	// Lockeo la configuracion.
	CCPWrite( &CLK.LOCK, CLK_LOCK_bm );

}
//------------------------------------------------------------------------------
void configure_RTC32(void)
{
	// El RTC32 lo utilizo para despertarme en el modo tickless.
	// V-bat needs to be reset, and activated
	VBAT.CTRL |= VBAT_ACCEN_bm;
	// Este registro esta protegido de escritura con CCP.
	CCPWrite(&VBAT.CTRL, VBAT_RESET_bm);

	// Pongo el reloj en 1.024Khz.
	VBAT.CTRL |=  VBAT_XOSCSEL_bm | VBAT_XOSCFDEN_bm ;

	// wait for 200us see AVR1321 Application note page 8
	_delay_us(200);

	// Turn on 32.768kHz crystal oscillator
	VBAT.CTRL |= VBAT_XOSCEN_bm;

	// Wait for stable oscillator
	while(!(VBAT.STATUS & VBAT_XOSCRDY_bm));

	// Disable RTC32 module before setting counter values
	RTC32.CTRL = 0;

	// Wait for sync
	do { } while ( RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm );

	// EL RTC corre a 1024 hz y quiero generar un tick de 10ms,
	RTC32.PER = 1024;
	RTC32.CNT = 0;

	// Interrupt: on Overflow
	RTC32.INTCTRL = RTC32_OVFINTLVL_LO_gc;

	// Enable RTC32 module
	RTC32.CTRL = RTC32_ENABLE_bm;

	/* Wait for sync */
	do { } while ( RTC32.SYNCCTRL & RTC32_SYNCBUSY_bm );
}
//------------------------------------------------------------------------------
void RTC32_ToscEnable( bool use1khz )
{
	/* Enable 32 kHz XTAL oscillator, with 1 kHz or 1 Hz output. */
	if (use1khz)
		VBAT.CTRL |= ( VBAT_XOSCEN_bm | VBAT_XOSCSEL_bm );
	else
		VBAT.CTRL |= ( VBAT_XOSCEN_bm );

	RTC32.PER = 10;
	RTC32.CNT = 0;

	/* Wait for oscillator to stabilize before returning. */
//	do { } while ( RTC32_ToscBusy() );
}
//------------------------------------------------------------------------------
void reset(void)
{
    xprintf_P(PSTR("ALERT !!!. Going to reset...\r\n"));
    vTaskDelay( ( TickType_t)( 100 / portTICK_PERIOD_MS ) );
	/* Issue a Software Reset to initilize the CPU */
	CCPWrite( &RST.CTRL, RST_SWRST_bm );   /* Issue a Software Reset to initilize the CPU */
                                           
}
//------------------------------------------------------------------------------
void config_default(void)
{

    // Configuro a default todas las configuraciones locales
    // y luego actualizo el systemConf
      
    systemConf.timerpoll = 60;
    systemConf.timerdial = 0;
    
    systemConf.samples_count = 1;
    
    systemConf.pwr_modo = PWR_CONTINUO;
    systemConf.pwr_hhmm_on = 2330;
    systemConf.pwr_hhmm_off = 630;
 
    // Actualizo las configuraciones locales a default
    ainputs_config_defaults();
    counters_config_defaults();
     
    // Actualizo las configuraciones locales en el systemConf
    ainputs_read_local_config(&systemConf.ainputs_conf);
    counters_read_local_config(&systemConf.counters_conf);
     
}
//------------------------------------------------------------------------------
bool config_debug( char *tipo, char *valor)
{
    /*
     * Configura las flags de debug para ayudar a visualizar los problemas
     */
    
    if (!strcmp_P( strupr(tipo), PSTR("NONE")) ) {
        ainputs_config_debug(false);
        return(true); 
    }

    if (!strcmp_P( strupr(tipo), PSTR("ANALOG")) ) {
        if (!strcmp_P( strupr(valor), PSTR("TRUE")) ) {
            ainputs_config_debug(true);
            return(true);
        }
        if (!strcmp_P( strupr(valor), PSTR("FALSE")) ) {
            ainputs_config_debug(false);
            return(true);
        }
    }

    if (!strcmp_P( strupr(tipo), PSTR("COUNTERS")) ) {
        if (!strcmp_P( strupr(valor), PSTR("TRUE")) ) {
            counters_config_debug(true);
            return(true);
        }
        if (!strcmp_P( strupr(valor), PSTR("FALSE")) ) {
            counters_config_debug(false);
            return(true);
        }
    }
    return(false);
    
}
//------------------------------------------------------------------------------
bool save_config_in_NVM(void)
{
   
int8_t retVal;
uint8_t cks;


    // Actualizo las configuraciones locales en systemConf
    ainputs_read_local_config(&systemConf.ainputs_conf);
    counters_read_local_config(&systemConf.counters_conf);
    
    cks = checksum ( (uint8_t *)&systemConf, ( sizeof(systemConf) - 1));
    systemConf.checksum = cks;    
    retVal = NVMEE_write( 0x00, (char *)&systemConf, sizeof(systemConf) );
    
    //xprintf_P(PSTR("DEBUG: Save in NVM OK\r\n"));
    
    if (retVal == -1 ) {
        xprintf_P( PSTR("ERROR: Save_config_NVM\r\n"));
		return(false);
    }
    
    return(true);
   
}
//------------------------------------------------------------------------------
bool load_config_from_NVM(void)
{

uint8_t rd_cks, calc_cks;
    
    NVMEE_read( 0x00, (char *)&systemConf, sizeof(systemConf) );
    rd_cks = systemConf.checksum;
    
    calc_cks = checksum ( (uint8_t *)&systemConf, ( sizeof(systemConf) - 1)); 
    if ( calc_cks != rd_cks ) {
		xprintf_P( PSTR("ERROR: Checksum systemVars failed: calc[0x%0x], read[0x%0x]\r\n"), calc_cks, rd_cks );
		return(false);
	}
    
    // Actualizo las configuraciones locales en el systemConf
    ainputs_update_local_config(&systemConf.ainputs_conf);
    counters_update_local_config(&systemConf.counters_conf);
    
    return(true);
}
//------------------------------------------------------------------------------
uint8_t checksum( uint8_t *s, uint16_t size )
{
	/*
	 * Recibe un puntero a una estructura y un tamaño.
	 * Recorre la estructura en forma lineal y calcula el checksum
	 */

uint8_t *p = NULL;
uint8_t cks = 0;
uint16_t i = 0;

	cks = 0;
	p = s;
	for ( i = 0; i < size ; i++) {
		 cks = (cks + (int)(p[i])) % 256;
	}

	return(cks);
}
//------------------------------------------------------------------------------
bool config_timerdial ( char *s_timerdial )
{
	// El timer dial puede ser 0 si vamos a trabajar en modo continuo o mayor a
	// 15 minutos.
	// Es una variable de 32 bits para almacenar los segundos de 24hs.

uint16_t l_timerdial;
    
    l_timerdial = atoi(s_timerdial);
    if ( (l_timerdial > 0) && (l_timerdial < TDIAL_MIN_DISCRETO ) ) {
        xprintf_P( PSTR("TDIAL warn: continuo TDIAL=0, discreto TDIAL >= 900)\r\n"));
        l_timerdial = TDIAL_MIN_DISCRETO;
    }
    
	systemConf.timerdial = atoi(s_timerdial);
	return(true);
}
//------------------------------------------------------------------------------
bool config_timerpoll ( char *s_timerpoll )
{
	// Configura el tiempo de poleo.
	// Se utiliza desde el modo comando como desde el modo online
	// El tiempo de poleo debe estar entre 15s y 3600s


	systemConf.timerpoll = atoi(s_timerpoll);

	if ( systemConf.timerpoll < 15 )
		systemConf.timerpoll = 15;

	if ( systemConf.timerpoll > 3600 )
		systemConf.timerpoll = 300;

	return(true);
}
//------------------------------------------------------------------------------
bool config_samples ( char *s_samples )
{
	// Configura el numero de muestras de c/poleo.
	// Debe ser mayor a 1 y menor a 20.


	systemConf.samples_count= atoi(s_samples);

	if ( systemConf.samples_count < 1 ) {
		systemConf.samples_count = 1;
        xprintf_P(PSTR("ALERT: samples default to 1 !!!\r\n"));
    }
    

	if ( systemConf.samples_count > 10 ) {
		systemConf.samples_count = 10;
        xprintf_P(PSTR("ALERT: samples default to 10 !!!\r\n"));
    }

	return(true);
}
//------------------------------------------------------------------------------
void xprint_dr(dataRcd_s *dr)
{
    /*
     * Imprime en pantalla el dataRcd pasado
     */
    
uint8_t i, channel;


//    xprintf_P( PSTR("ID:%s;TYPE:%s;VER:%s;"), systemConf.dlgid, FW_TYPE, FW_REV);
 
    // Clock
    xprintf_P( PSTR("DATE:%02d%02d%02d;"), dr->rtc.year, dr->rtc.month, dr->rtc.day );
    xprintf_P( PSTR("TIME:%02d%02d%02d;"), dr->rtc.hour, dr->rtc.min, dr->rtc.sec);
    
    // Analog Channels:
    for ( i=0; i < NRO_ANALOG_CHANNELS; i++) {
        //if ( strcmp ( systemConf.ainputs_conf[channel].name, "X" ) != 0 ) {
        if ( systemConf.ainputs_conf.channel[i].enabled ) {
            xprintf_P( PSTR("%s:%0.2f;"), systemConf.ainputs_conf.channel[i].name, dr->l_ainputs[i]);
        }
    }
   
    // Counter Channels:
    for ( channel=0; channel < NRO_COUNTER_CHANNELS; channel++) {
        if ( strcmp ( systemConf.counters_conf.channel[channel].name, "X" ) != 0 ) {
            xprintf_P( PSTR("%s:%0.3f;"), systemConf.counters_conf.channel[channel].name, dr->l_counters[channel]);
        }
    }

    // Battery
    xprintf_P( PSTR("bt:%0.2f;"), dr->battery);
    
    xprintf_P( PSTR("\r\n"));
}
//------------------------------------------------------------------------------

