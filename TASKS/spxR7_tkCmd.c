
#include "spxR7.h"
#include "frtos_cmd.h"

static void cmdClsFunction(void);
static void cmdHelpFunction(void);
static void cmdResetFunction(void);
static void cmdStatusFunction(void);
static void cmdWriteFunction(void);
static void cmdReadFunction(void);
static void cmdTestFunction(void);
static void cmdConfigFunction(void);

static void pv_snprintfP_OK(void );
static void pv_snprintfP_ERR(void );

//------------------------------------------------------------------------------
void tkCmd(void * pvParameters)
{

	// Esta es la primer tarea que arranca.

( void ) pvParameters;

    while ( ! run_tasks )
        vTaskDelay( ( TickType_t)( 100 / portTICK_PERIOD_MS ) );

uint8_t c = 0;

    FRTOS_CMD_init();

    FRTOS_CMD_register( "cls", cmdClsFunction );
	FRTOS_CMD_register( "help", cmdHelpFunction );
    FRTOS_CMD_register( "reset", cmdResetFunction );
    FRTOS_CMD_register( "status", cmdStatusFunction );
    FRTOS_CMD_register( "write", cmdWriteFunction );
    FRTOS_CMD_register( "read", cmdReadFunction );
    FRTOS_CMD_register( "test", cmdTestFunction );
    FRTOS_CMD_register( "config", cmdConfigFunction );
    
    xprintf_P(PSTR("Starting tkCmd..\r\n" ));
    xprintf_P(PSTR("Spymovil %s %s %s %s \r\n") , HW_MODELO, FRTOS_VERSION, FW_REV, FW_DATE);
       
	// loop
	for( ;; )
	{
         
		c = '\0';	// Lo borro para que luego del un CR no resetee siempre el timer.
		// el read se bloquea 10ms. lo que genera la espera.
		//while ( frtos_read( fdTERM, (char *)&c, 1 ) == 1 ) {
        while ( xgetc( (char *)&c ) == 1 ) {
            FRTOS_CMD_process(c);
        }
        
        // Espero 10ms si no hay caracteres en el buffer
        vTaskDelay( ( TickType_t)( 10 / portTICK_PERIOD_MS ) );
               
	}    
}
//------------------------------------------------------------------------------
static void cmdTestFunction(void)
{

    FRTOS_CMD_makeArgv();

    xprintf_P( PSTR("Test: poll, readrcd {pos},readdrcd {pos}, fsdebugon, fsdebugoff, write, kill {wan,sys}\r\n"));
    return;
       
}
//------------------------------------------------------------------------------
static void cmdHelpFunction(void)
{

    FRTOS_CMD_makeArgv();
        
    if ( !strcmp_P( strupr(argv[1]), PSTR("WRITE"))) {
		xprintf_P( PSTR("-write:\r\n"));
        xprintf_P( PSTR("  (ee,nvmee,rtcram) {pos string} {debug}\r\n"));
        xprintf_P( PSTR("  rtc YYMMDDhhmm\r\n"));
        xprintf_P( PSTR("  ina {a|b} {confValue}\r\n"));
        xprintf_P( PSTR("  vsensors420 {on/off}\r\n"));
        
    }  else if ( !strcmp_P( strupr(argv[1]), PSTR("READ"))) {
		xprintf_P( PSTR("-read:\r\n"));
        xprintf_P( PSTR("  (ee,nvmee,rtcram) {pos} {lenght} {debug}\r\n"));
        xprintf_P( PSTR("  rtc {short | long }\r\n"));
        xprintf_P( PSTR("  ina {a|b} {conf|chXshv|chXbusv|mfid|dieid}\r\n"));
        xprintf_P( PSTR("  serial,devid\r\n"));
        xprintf_P( PSTR("  ainput {n}\r\n"));
        xprintf_P( PSTR("  cnt {0,1}\r\n"));
        
    }  else if ( !strcmp_P( strupr(argv[1]), PSTR("CONFIG"))) {
		xprintf_P( PSTR("-config:\r\n"));
        xprintf_P( PSTR("  default\r\n"));
        xprintf_P( PSTR("  save,load\r\n"));
        xprintf_P( PSTR("  timerpoll, timerdial, samples {1..10}\r\n"));
        xprintf_P( PSTR("  pwrmodo {continuo,discreto,mixto}, pwron {hhmm}, pwroff {hhmm}\r\n"));
        xprintf_P( PSTR("  debug {analog,counters,comms,modbus,piloto,none} {true/false}\r\n"));
        xprintf_P( PSTR("  ainput {0..%d} enable{true/false} aname imin imax mmin mmax offset\r\n"),( NRO_ANALOG_CHANNELS - 1 ) );
        xprintf_P( PSTR("  counter {0..%d} enable{true/false} cname magPP modo(PULSO/CAUDAL),rbsize\r\n"), ( NRO_COUNTER_CHANNELS - 1 ) );
 
        
	} else if (!strcmp_P( strupr(argv[1]), PSTR("RESET"))) {
		xprintf_P( PSTR("-reset\r\n"));
        xprintf_P( PSTR("  memory {soft|hard}\r\n"));
        
    } else if (!strcmp_P( strupr(argv[1]), PSTR("TEST"))) {
		xprintf_P( PSTR("-test\r\n"));  
        
    }  else {
        // HELP GENERAL
        xprintf("Available commands are:\r\n");
        xprintf("-cls\r\n");
        xprintf("-help\r\n");
        xprintf("-status\r\n");
        xprintf("-reset\r\n");
        xprintf("-config...\r\n");
    }
   
	xprintf("Exit help \r\n");

}
//------------------------------------------------------------------------------
static void cmdClsFunction(void)
{
	// ESC [ 2 J
	xprintf("\x1B[2J\0");
}
//------------------------------------------------------------------------------
static void cmdResetFunction(void)
{
    
    FRTOS_CMD_makeArgv();
    
    // Reset memory ??
    if (!strcmp_P( strupr(argv[1]), PSTR("MEMORY"))) {
       
    }
    
    xprintf("Reset..\r\n");
    reset();
    pv_snprintfP_OK();
}
//------------------------------------------------------------------------------
static void cmdStatusFunction(void)
{

    // https://stackoverflow.com/questions/12844117/printing-defined-constants

    xprintf("Spymovil %s %s TYPE=%s, VER=%s %s \r\n" , HW_MODELO, FRTOS_VERSION, FW_TYPE, FW_REV, FW_DATE);
 
    xprintf_P(PSTR("Config:\r\n"));
    xprintf_P(PSTR(" date: %s\r\n"), RTC_logprint(FORMAT_LONG));
    xprintf_P(PSTR(" timerdial=%d\r\n"), systemConf.timerdial);
    xprintf_P(PSTR(" timerpoll=%d\r\n"), systemConf.timerpoll);
    xprintf_P(PSTR(" samples=%d\r\n"), systemConf.samples_count);

	ainputs_print_configuration();
    counters_print_configuration();
    
    xprintf_P(PSTR("Values:\r\n"));
    xprintf_P(PSTR(" Frame: "));
    xprint_dr( get_system_dr());
}
//------------------------------------------------------------------------------
static void cmdReadFunction(void)
{
    
    FRTOS_CMD_makeArgv();
  
    // CNT{0,1}
	// read cnt
	if (!strcmp_P( strupr(argv[1]), PSTR("CNT")) ) {
        if ( atoi(argv[2]) == 0 ) {
            xprintf_P(PSTR("CNT0=%d\r\n"), CNT0_read());
            pv_snprintfP_OK();
            return;
        }
        if ( atoi(argv[2]) == 1 ) {
            xprintf_P(PSTR("CNT1=%d\r\n"), CNT1_read());
            pv_snprintfP_OK();
            return;
        }
        pv_snprintfP_ERR();
        return;
	}
    
    // AINPUT
    // read ainput {n}
	if (!strcmp_P( strupr(argv[1]), PSTR("AINPUT"))  ) {
        ainputs_test_read_channel( atoi(argv[2]) ) ? pv_snprintfP_OK(): pv_snprintfP_ERR();
		return;
	}
    
    // EE
	// read ee address length
	if (!strcmp_P( strupr(argv[1]), PSTR("EE")) ) {
        EE_test_read( argv[2], argv[3], argv[4] );
		return;
	}
    
    // RTC
	// read rtc { long | short }
    if (!strcmp_P( strupr(argv[1]), PSTR("RTC")) ) {
        if (!strcmp_P( strupr(argv[2]), PSTR("LONG")) ) {
            RTC_read_time(FORMAT_LONG);
            pv_snprintfP_OK();
            return;
        }
        if (!strcmp_P( strupr(argv[2]), PSTR("SHORT")) ) {
            RTC_read_time(FORMAT_SHORT);
            pv_snprintfP_OK();
            return;
        }
        
        RTC_read_time(FORMAT_LONG);
        pv_snprintfP_OK();
        return;
    }
        
	// RTC SRAM
	// read rtcram address length
	if (!strcmp_P( strupr(argv[1]), PSTR("RTCRAM"))) {
		RTCSRAM_test_read ( argv[2], argv[3] );
		return;
	}    
    
    // INA
	// read ina regName
	if (!strcmp_P( strupr(argv[1]), PSTR("INA"))  ) {
		INA_awake();
		INA_test_read ( argv[2], argv[3] );
		INA_sleep();
		return;
	}
    
    // NVMEE
	// read nvmee address length
	if (!strcmp_P( strupr(argv[1]), PSTR("NVMEE")) ) {
		NVMEE_test_read ( argv[2], argv[3] );
		return;
	}  
    
    // SERIAL
    if (!strcmp_P( strupr(argv[1]), PSTR("SERIAL")) ) {
		xprintf_P( PSTR("uSERIAL=%s\r\n"), NVMEE_read_serial() );
		return;
	}  

    // DEVID
    if (!strcmp_P( strupr(argv[1]), PSTR("DEVID")) ) {
        if ( strstr( NVMEE_read_device_ID(), "1e9843") ) {
            xprintf_P(PSTR("(AVR XMEGA256 A3BU)\r\n"));
        } else 	if ( strstr( NVMEE_read_device_ID(), "1e9842") ) {
            xprintf_P(PSTR("(AVR XMEGA256 A3U)\r\n"));
        } else {
            xprintf_P(PSTR("(AVR)\r\n"));
        }
		return;
	}
    
    // CMD NOT FOUND
	xprintf("ERROR\r\nCMD NOT DEFINED\r\n");
	return;
 
}
//------------------------------------------------------------------------------
static void cmdWriteFunction(void)
{

    FRTOS_CMD_makeArgv();
       
    // write VSENSORS420 on/off
	if (!strcmp_P( strupr(argv[1]), PSTR("VSENSORS420")) ) {
        if (!strcmp_P( strupr(argv[2]), PSTR("ON")) ) {
            SET_VSENSORS420();
            pv_snprintfP_OK();
            return;
        }
        if (!strcmp_P( strupr(argv[2]), PSTR("OFF")) ) {
            CLEAR_VSENSORS420();
            pv_snprintfP_OK();
            return;
        }
        pv_snprintfP_ERR();
        return;
	}

   	// EE
	// write ee pos string
	if ((strcmp_P( strupr(argv[1]), PSTR("EE")) == 0) ) {
		( EE_test_write ( argv[2], argv[3], argv[4] ) > 0)?  pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}

    // RTC
	// write rtc YYMMDDhhmm
	if ( strcmp_P( strupr(argv[1]), PSTR("RTC")) == 0 ) {
		( RTC_write_time( argv[2]) > 0)?  pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}
    
	// RTC SRAM
	// write rtcram pos string
	if ( (strcmp_P( strupr(argv[1]), PSTR("RTCRAM")) == 0)  ) {
		( RTCSRAM_test_write ( argv[2], argv[3] ) > 0)?  pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}
    
    // INA
	// write ina rconfValue
	// Solo escribimos el registro 0 de configuracion.
	if ((strcmp_P( strupr(argv[1]), PSTR("INA")) == 0) ) {
        INA_awake();
		( INA_test_write ( argv[2], argv[3] ) > 0)?  pv_snprintfP_OK() : pv_snprintfP_ERR();
        INA_sleep();
		return;
	}
    
    // NVMEE
	// write nvmee pos string
	if ( (strcmp_P( strupr(argv[1]), PSTR("NVMEE")) == 0)) {
		NVMEE_test_write ( argv[2], argv[3] );
		pv_snprintfP_OK();
		return;
	}
    
    // CMD NOT FOUND
	xprintf("ERROR\r\nCMD NOT DEFINED\r\n");
	return;
 
}
//------------------------------------------------------------------------------
static void cmdConfigFunction(void)
{
    
    FRTOS_CMD_makeArgv();
    
 
    // DEFAULT
	// config default
	if (!strcmp_P( strupr(argv[1]), PSTR("DEFAULT"))) {
		config_default();
		pv_snprintfP_OK();
		return;
	}

	// SAVE
	// config save
	if (!strcmp_P( strupr(argv[1]), PSTR("SAVE"))) {       
		( save_config_in_NVM() != -1 )?  pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}
    
    // LOAD
	// config load
	if (!strcmp_P( strupr(argv[1]), PSTR("LOAD"))) {
		load_config_from_NVM();
		pv_snprintfP_OK();
		return;
	}

    // TIMERPOLL
    // config timerpoll val
	if (!strcmp_P( strupr(argv[1]), PSTR("TIMERPOLL")) ) {
        config_timerpoll(argv[2]) ? pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}
    
    // TIMERDIAL
    // config timerdial val
	if (!strcmp_P( strupr(argv[1]), PSTR("TIMERDIAL")) ) {
		config_timerdial(argv[2]) ? pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
	}
    
    // AINPUT
	// ainput {0..%d} enable aname imin imax mmin mmax offset
	if (!strcmp_P( strupr(argv[1]), PSTR("AINPUT")) ) {
		ainputs_config_channel ( atoi(argv[2]), argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]);
        pv_snprintfP_OK();
		return;
	}
        
    // COUNTER
    // counter {0..%d} enable cname magPP modo(PULSO/CAUDAL),rbsize
	if (!strcmp_P( strupr(argv[1]), PSTR("COUNTER")) ) {
        counters_config_channel( atoi(argv[2]), argv[3], argv[4], argv[5], argv[6], argv[7]  );
        pv_snprintfP_OK();
		return;
	}
    
    // DEBUG
    // config debug (ainput, counter, comms) (true,false)
    if (!strcmp_P( strupr(argv[1]), PSTR("DEBUG")) ) {
        config_debug( argv[2], argv[3]) ? pv_snprintfP_OK() : pv_snprintfP_ERR();
		return;
    }
    
    // CMD NOT FOUND
	xprintf("ERROR\r\nCMD NOT DEFINED\r\n\0");
	return;
 
}
//------------------------------------------------------------------------------
static void pv_snprintfP_ERR(void)
{
	xprintf("error\r\n\0");
}
//------------------------------------------------------------------------------
static void pv_snprintfP_OK(void )
{
	xprintf("ok\r\n\0");
}
//------------------------------------------------------------------------------
