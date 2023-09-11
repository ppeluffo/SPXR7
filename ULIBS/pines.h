/*
 * l_iopines.h
 *
 *  Created on: 8 dic. 2018
 *      Author: pablo
 */

#ifndef SRC_SPX_LIBS_L_IOPINES_H_
#define SRC_SPX_LIBS_L_IOPINES_H_

#include <avr/io.h>

#define PORT_SetPinAsOutput( _port, _bitPosition ) ( (_port)->DIR = (_port)->DIR | (1 << _bitPosition) )
#define PORT_SetPinAsInput( _port, _bitPosition ) ( (_port)->DIR = (_port)->DIR & ~(1 << _bitPosition) )

#define PORT_SetOutputBit( _port, _bitPosition ) ( (_port)->OUT = (_port)->OUT | (1 << _bitPosition) )
#define PORT_ClearOutputBit( _port, _bitPosition ) ( (_port)->OUT = (_port)->OUT & ~(1 << _bitPosition) )

#define PORT_GetBitValue( _port , _bitPosition ) ( (((_port)->IN) >> _bitPosition ) & 0x01 )

#define PORT_TogglePins( _port, _toggleMask ) ( (_port)->OUTTGL = _toggleMask )

void PORT_ConfigureInterrupt0( PORT_t * port,PORT_INT0LVL_t intLevel,uint8_t pinMask);
void PORT_ConfigureInterrupt1( PORT_t * port,PORT_INT1LVL_t intLevel, uint8_t pinMask);
//------------------------------------------------------------------------------------
// Control de Frecuencia del TICK
//#define TICK_BITPOS		2
//#define TICK_PORT		PORTA

//#define IO_config_TICK()	PORT_SetPinAsOutput( &TICK_PORT, TICK_BITPOS)
//#define IO_set_TICK()		PORT_SetOutputBit( &TICK_PORT, TICK_BITPOS)
//#define IO_clr_TICK()		PORT_ClearOutputBit( &TICK_PORT, TICK_BITPOS)
//------------------------------------------------------------------------------
// TWI PE0(SDA)/PE1(SCL)
#define SCL_BITPOS		1
#define SCL_PORT		PORTE

#define IO_config_SCL()		PORT_SetPinAsOutput( &SCL_PORT, SCL_BITPOS)
#define IO_set_SCL()		PORT_SetOutputBit( &SCL_PORT, SCL_BITPOS)
#define IO_clr_SCL()		PORT_ClearOutputBit( &SCL_PORT, SCL_BITPOS)

//------------------------------------------------------------------------------
// ANALOG_IN: SENSOR VCC CONTROL

// Salida de prender/apagar sensores 4-20
#define VSENSORS420_PORT         PORTA
#define VSENSORS420              1
#define VSENSORS420_PIN_bm       PIN1_bm
#define VSENSORS420_PIN_bp       PIN1_bp

#define SET_VSENSORS420()       ( VSENSORS420_PORT.OUT |= VSENSORS420_PIN_bm )
//#define SET_VSENSORS420()		PORT_SetOutputBit( &VSENSORS420_PORT, VSENSORS420)
#define CLEAR_VSENSORS420()     ( VSENSORS420_PORT.OUT &= ~VSENSORS420_PIN_bm )
//#define CLEAR_VSENSORS420()		PORT_ClearOutputBit( &VSENSORS420_PORT, VSENSORS420)

void VSENSORS420_init(void);
//#define VSENSORS420_init()	PORT_SetPinAsOutput( &VSENSORS420_PORT, VSENSORS420_PIN_bm)

// ENTRADAS DIGITALES ( SOLO EN SPX_5CH ya que el otro usa el MCP )
// Solo niveles logicos

#define PA0_BITPOS		0
#define PA0_PORT		PORTA

#define PB7_BITPOS		7
#define PB7_PORT		PORTB

#define IO_config_PA0()		PORT_SetPinAsInput( &PA0_PORT, PA0_BITPOS)
#define IO_config_PB7()		PORT_SetPinAsInput( &PB7_PORT, PB7_BITPOS)

uint8_t IO_read_PA0(void);
uint8_t IO_read_PB7(void);

//------------------------------------------------------------------------------------
// GPRS

// Salidas de micro, entradas del SIM
#define GPRS_PWR_BITPOS			4
#define GPRS_PWR_PORT			PORTD
#define IO_config_GPRS_PWR()	PORT_SetPinAsOutput( &GPRS_PWR_PORT, GPRS_PWR_BITPOS)
#define IO_set_GPRS_PWR()		PORT_SetOutputBit( &GPRS_PWR_PORT, GPRS_PWR_BITPOS)
#define IO_clr_GPRS_PWR()		PORT_ClearOutputBit( &GPRS_PWR_PORT, GPRS_PWR_BITPOS)

#define GPRS_SW_BITPOS			5
#define GPRS_SW_PORT			PORTD
#define IO_config_GPRS_SW()		PORT_SetPinAsOutput( &GPRS_SW_PORT, GPRS_SW_BITPOS)
#define IO_set_GPRS_SW()		PORT_SetOutputBit( &GPRS_SW_PORT, GPRS_SW_BITPOS)
#define IO_clr_GPRS_SW()		PORT_ClearOutputBit( &GPRS_SW_PORT, GPRS_SW_BITPOS)

#define GPRS_CTS_BITPOS			4
#define GPRS_CTS_PORT			PORTE
#define IO_config_GPRS_CTS()	PORT_SetPinAsInput( &GPRS_CTS_PORT, GPRS_CTS_BITPOS)
uint8_t IO_read_CTS(void);

#define GPRS_RTS_BITPOS			5
#define GPRS_RTS_PORT			PORTE
#define IO_config_GPRS_RTS()	PORT_SetPinAsOutput( &GPRS_RTS_PORT, GPRS_RTS_BITPOS)
#define IO_set_GPRS_RTS()		PORT_SetOutputBit( &GPRS_RTS_PORT, GPRS_RTS_BITPOS)
#define IO_clr_GPRS_RTS()		PORT_ClearOutputBit( &GPRS_RTS_PORT, GPRS_RTS_BITPOS)

#define GPRS_DCD_BITPOS			6
#define GPRS_DCD_PORT			PORTD
#define IO_config_GPRS_DCD()	PORT_SetPinAsInput( &GPRS_DCD_PORT, GPRS_DCD_BITPOS)
uint8_t IO_read_DCD(void);

#define GPRS_RI_BITPOS			7
#define GPRS_RI_PORT			PORTD
#define IO_config_GPRS_RI()		PORT_SetPinAsInput( &GPRS_RI_PORT, GPRS_RI_BITPOS)
uint8_t IO_read_RI(void);

#define GPRS_DTR_BITPOS			6
#define GPRS_DTR_PORT			PORTB
#define IO_config_GPRS_DTR()	PORT_SetPinAsOutput( &GPRS_DTR_PORT, GPRS_DTR_BITPOS)
#define IO_set_GPRS_DTR()		PORT_SetOutputBit( &GPRS_DTR_PORT, GPRS_DTR_BITPOS)
#define IO_clr_GPRS_DTR()		PORT_ClearOutputBit( &GPRS_DTR_PORT, GPRS_DTR_BITPOS)

// Corresponde a ATXMEGA TXD (Output)
#define GPRS_RX_BITPOS			3
#define GPRS_RX_PORT			PORTE
#define IO_config_GPRS_RX()		PORT_SetPinAsOutput( &GPRS_RX_PORT, GPRS_RX_BITPOS)

// Corresponde a ATXMEGA RXD (Input)
#define GPRS_TX_BITPOS			2
#define GPRS_TX_PORT			PORTE
#define IO_config_GPRS_TX()		PORT_SetPinAsInput( &GPRS_TX_PORT, GPRS_TX_BITPOS)
//------------------------------------------------------------------------------------
// AUX1

// MAIN POWER SLEEP MODE

#define PWR_SLEEP_BITPOS	5
#define PWR_SLEEP_PORT	PORTB

#define IO_config_PWR_SLEEP()	PORT_SetPinAsOutput( &PWR_SLEEP_PORT, PWR_SLEEP_BITPOS)
#define IO_set_PWR_SLEEP()		PORT_SetOutputBit( &PWR_SLEEP_PORT, PWR_SLEEP_BITPOS)
#define IO_clr_PWR_SLEEP()		PORT_ClearOutputBit( &PWR_SLEEP_PORT, PWR_SLEEP_BITPOS)

//------------------------------------------------------------------------------------

#define SLEEP_CTL_BITPOS		0
#define SLEEP_CTL_PORT			PORTB

#define IO_config_SLEEP_CTL()		PORT_SetPinAsInput( &SLEEP_CTL_PORT, SLEEP_CTL_BITPOS)
uint8_t IO_read_SLEEP_CTL(void);
//------------------------------------------------------------------------------------
// RELES

#define RELE_K1_PORT         PORTD
#define RELE_K1              5
#define RELE_K1_PIN_bm       PIN4_bm
#define RELE_K1_PIN_bp       PIN4_bp

#define SET_RELE_K1()       ( RELE_K1_PORT.OUT |= RELE_K1_PIN_bm )
#define CLEAR_RELE_K1()     ( RELE_K1_PORT.OUT &= ~RELE_K1_PIN_bm )
#define TOGGLE_RELE_K1()    ( RELE_K1_PORT.OUT ^= 1UL << RELE_K1_PIN_bp);
    
#define RELE_K1_OPEN()      SET_RELE_K1() 
#define RELE_K1_CLOSE()     CLEAR_RELE_K1()

#define MODEM_PRENDER()     SET_RELE_K1() 
#define MODEM_APAGAR()      CLEAR_RELE_K1()

void RELE_K1_init(void);
    
#define RELE_K2_PORT         PORTD
#define RELE_K2              4 
#define RELE_K2_PIN_bm       PIN5_bm
#define RELE_K2_PIN_bp       PIN5_bp

#define SET_RELE_K2()       ( RELE_K2_PORT.OUT |= RELE_K2_PIN_bm )
#define CLEAR_RELE_K2()     ( RELE_K2_PORT.OUT &= ~RELE_K2_PIN_bm )
#define TOGGLE_RELE_K2()    ( RELE_K2_PORT.OUT ^= 1UL << RELE_K2_PIN_bp);
    
#define RELE_K2_OPEN()      SET_RELE_K2() 
#define RELE_K2_CLOSE()     CLEAR_RELE_K2()


void RELE_K2_init(void);

//------------------------------------------------------------------------------
// RTS485A

#define RTS_RS485A_PORT         PORTE
#define RTS_RS485A              5
#define RTS_RS485A_PIN_bm       PIN5_bm
#define RTS_RS485A_PIN_bp       PIN5_bp
#define SET_RTS_RS485A()        ( RTS_RS485A_PORT.OUT |= RTS_RS485A_PIN_bm )
#define CLEAR_RTS_RS485A()      ( RTS_RS485A_PORT.OUT &= ~RTS_RS485A_PIN_bm )

#define CONFIG_RTS_485A()       RTS_RS485A_PORT.DIR |= RTS_RS485A_PIN_bm;

//------------------------------------------------------------------------------
// DRV8814:

#define V12_OUTS_CTL_BITPOS			3
#define V12_OUTS_CTL_PORT			PORTA
#define IO_config_V12_OUTS_CTL()	PORT_SetPinAsOutput( & V12_OUTS_CTL_PORT,  V12_OUTS_CTL_BITPOS)
#define IO_set_V12_OUTS_CTL()		PORT_SetOutputBit( & V12_OUTS_CTL_PORT,  V12_OUTS_CTL_BITPOS)
#define IO_clr_V12_OUTS_CTL()		PORT_ClearOutputBit( & V12_OUTS_CTL_PORT,  V12_OUTS_CTL_BITPOS)

#define DRV8814_RESET_PORT      PORTA      
#define DRV8814_RESET           4
#define DRV8814_RESET_PIN_bm    PIN4_bm
#define DRV8814_RESET_PIN_bp    PIN4_bp
#define SET_DRV8814_RESET       ( DRV8814_RESET_PORT.OUT |= DRV8814_RESET_PIN_bm )
#define CLEAR_DRV8814_RESET     ( DRV8814_RESET_PORT.OUT &= ~DRV8814_RESET_PIN_bm )
#define CONFIG_DRV8814_RESET    ( DRV8814_RESET_PORT.DIR |= DRV8814_RESET_PIN_bm )

#define DRV8814_SLEEP_PORT      PORTA     
#define DRV8814_SLEEP           5
#define DRV8814_SLEEP_PIN_bm    PIN5_bm
#define DRV8814_SLEEP_PIN_bp    PIN5_bp
#define SET_DRV8814_SLEEP       ( DRV8814_SLEEP_PORT.OUT |= DRV8814_SLEEP_PIN_bm )
#define CLEAR_DRV8814_SLEEP     ( DRV8814_SLEEP_PORT.OUT &= ~DRV8814_SLEEP_PIN_bm )
#define CONFIG_DRV8814_SLEEP    ( DRV8814_SLEEP_PORT.DIR |= DRV8814_SLEEP_PIN_bm )

#define DRV8814_AEN_PORT      PORTA    
#define DRV8814_AEN           7
#define DRV8814_AEN_PIN_bm    PIN7_bm
#define DRV8814_AEN_PIN_bp    PIN7_bp
#define SET_DRV8814_AEN       ( DRV8814_AEN_PORT.OUT |= DRV8814_AEN_PIN_bm )
#define CLEAR_DRV8814_AEN     ( DRV8814_AEN_PORT.OUT &= ~DRV8814_AEN_PIN_bm )
#define CONFIG_DRV8814_AEN    ( DRV8814_AEN_PORT.DIR |= DRV8814_AEN_PIN_bm )

#define DRV8814_BEN_PORT      PORTB      
#define DRV8814_BEN           0
#define DRV8814_BEN_PIN_bm    PIN0_bm
#define DRV8814_BEN_PIN_bp    PIN0_bp
#define SET_DRV8814_BEN       ( DRV8814_BEN_PORT.OUT |= DRV8814_BEN_PIN_bm )
#define CLEAR_DRV8814_BEN     ( DRV8814_BEN_PORT.OUT &= ~DRV8814_BEN_PIN_bm )
#define CONFIG_DRV8814_BEN    ( DRV8814_BEN_PORT.DIR |= DRV8814_BEN_PIN_bm )

#define DRV8814_APH_PORT      PORTA      
#define DRV8814_APH           6
#define DRV8814_APH_PIN_bm    PIN6_bm
#define DRV8814_APH_PIN_bp    PIN6_bp
#define SET_DRV8814_APH       ( DRV8814_APH_PORT.OUT |= DRV8814_APH_PIN_bm )
#define CLEAR_DRV8814_APH     ( DRV8814_APH_PORT.OUT &= ~DRV8814_APH_PIN_bm )
#define CONFIG_DRV8814_APH    ( DRV8814_APH_PORT.DIR |= DRV8814_APH_PIN_bm )

#define DRV8814_BPH_PORT      PORTB      
#define DRV8814_BPH           1
#define DRV8814_BPH_PIN_bm    PIN1_bm
#define DRV8814_BPH_PIN_bp    PIN1_bp
#define SET_DRV8814_BPH       ( DRV8814_BPH_PORT.OUT |= DRV8814_BPH_PIN_bm )
#define CLEAR_DRV8814_BPH     ( DRV8814_BPH_PORT.OUT &= ~DRV8814_BPH_PIN_bm )
#define CONFIG_DRV8814_BPH    ( DRV8814_BPH_PORT.DIR |= DRV8814_BPH_PIN_bm )

//------------------------------------------------------------------------------

// Los pines de FinCarrera son entradas
#define FC1_PORT      PORTB    
#define FC1           7
#define FC1_PIN_bm    PIN7_bm
#define FC1_PIN_bp    PIN7_bp
#define CONFIG_FC1    ( FC1_PORT.DIR &= ~FC1_PIN_bm )

#define FC2_PORT      PORTB     
#define FC2           6
#define FC2_PIN_bm    PIN6_bm
#define FC2_PIN_bp    PIN6_bp
#define CONFIG_FC2    ( FC2_PORT.DIR &= ~FC2_PIN_bm )

uint8_t FC1_read(void);
uint8_t FC2_read(void);
#define FC_alta_read() FC1_read()
#define FC_baja_read() FC2_read()
void FCx_init(void);

//------------------------------------------------------------------------------

#endif /* SRC_SPX_LIBS_L_IOPINES_H_ */
