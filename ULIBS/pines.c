/*
 * l_iopines.c
 *
 *  Created on: 7 de jul. de 2017
 *      Author: pablo
 */

#include "pines.h"
#include "bytes.h"


// -----------------------------------------------------------------------------
void VSENSORS420_init(void)
{
    // Configura el pin del SENSORS420 como output
	VSENSORS420_PORT.DIR |= VSENSORS420_PIN_bm;	
	CLEAR_VSENSORS420();
}

// -----------------------------------------------------------------------------
// ENTRADAS DIGITALES ( SOLO EN SPX_5CH ya que el otro usa el MCP )

uint8_t IO_read_PA0(void)
{
	return( PORT_GetBitValue(&PA0_PORT, PA0_BITPOS));
}
//------------------------------------------------------------------------------------
uint8_t IO_read_PB7(void)
{
	return( PORT_GetBitValue(&PB7_PORT, PB7_BITPOS));
}
//------------------------------------------------------------------------------------
// GPRS

uint8_t IO_read_DCD(void)
{
	return( PORT_GetBitValue(&GPRS_DCD_PORT, GPRS_DCD_BITPOS));
}
//------------------------------------------------------------------------------------
uint8_t IO_read_RI(void)
{
	return( PORT_GetBitValue(&GPRS_RI_PORT, GPRS_RI_BITPOS));
}
//------------------------------------------------------------------------------------
uint8_t IO_read_CTS(void)
{
	return( PORT_GetBitValue(&GPRS_CTS_PORT, GPRS_CTS_BITPOS));
}
//------------------------------------------------------------------------------------

uint8_t IO_read_SLEEP_CTL(void)
{
	return( PORT_GetBitValue(&SLEEP_CTL_PORT, SLEEP_CTL_BITPOS));
}
//------------------------------------------------------------------------------------

/*
 *  This function configures interrupt 1 to be associated with a set of pins and
 *  sets the desired interrupt level.
 *
 *  port       The port to configure.
 *  intLevel   The desired interrupt level for port interrupt 1.
 *  pinMask    A mask that selects the pins to associate with port interrupt 1.
 */
/*
void PORT_ConfigureInterrupt0( PORT_t * port,PORT_INT0LVL_t intLevel,uint8_t pinMask )
{
	port->INTCTRL = ( port->INTCTRL & ~PORT_INT0LVL_gm ) | intLevel;
	port->INT0MASK = pinMask;
}

//------------------------------------------------------------------------------------
void PORT_ConfigureInterrupt1( PORT_t * port, PORT_INT1LVL_t intLevel,uint8_t pinMask )
{
	port->INTCTRL = ( port->INTCTRL & ~PORT_INT1LVL_gm ) | intLevel;
	port->INT1MASK = pinMask;
}
*/
