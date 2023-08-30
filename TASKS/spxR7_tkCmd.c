
#include "spxR7.h"
#include "frtos_cmd.h"

static void cmdClsFunction(void);
static void cmdHelpFunction(void);
static void cmdResetFunction(void);
static void cmdStatusFunction(void);
static void cmdWriteFunction(void);
static void cmdReadFunction(void);
static void cmdTestFunction(void);

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
        
    }  else if ( !strcmp_P( strupr(argv[1]), PSTR("READ"))) {
		xprintf_P( PSTR("-read:\r\n"));
        xprintf_P( PSTR("  (ee,nvmee,rtcram) {pos} {lenght} {debug}\r\n"));
        xprintf_P( PSTR("  rtc {long,short}\r\n"));
        xprintf_P( PSTR("  ina {a|b} {conf|chXshv|chXbusv|mfid|dieid}\r\n"));
        
    }  else if ( !strcmp_P( strupr(argv[1]), PSTR("CONFIG"))) {
		xprintf_P( PSTR("-config:\r\n"));
   
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
         
}
//------------------------------------------------------------------------------
static void cmdReadFunction(void)
{
    
    FRTOS_CMD_makeArgv();
    
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
        pv_snprintfP_ERR();
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
    
    // CMD NOT FOUND
	xprintf("ERROR\r\nCMD NOT DEFINED\r\n\0");
	return;
 
}
//------------------------------------------------------------------------------
static void cmdWriteFunction(void)
{

    FRTOS_CMD_makeArgv();
         
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
