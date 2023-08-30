/*
 * frtos-io.c
 *
 *  Created on: 11 jul. 2018
 *      Author: pablo
 */

#include "frtos-io.h"

#define USART_IsTXDataRegisterEmpty(_usart) (((_usart)->STATUS & USART_DREIF_bm) != 0)
#define USART_IsTXShiftRegisterEmpty(_usart) (((_usart)->STATUS & USART_TXCIF_bm) != 0)
#define USART_PutChar(_usart, _data) ((_usart)->DATA = _data)

//------------------------------------------------------------------------------
int16_t frtos_open( file_descriptor_t fd, uint32_t flags)
{
	// Funcion general para abrir el puerto que invoca a una mas
	// especializada para c/periferico.
	// Es la que invoca la aplicacion.
	// Retorna -1 en error o un nro positivo ( fd )

int16_t xRet = -1;

	switch(fd) {
	case fdXCOMMS:
		//frtos_open_uart0( flags );
        xRet = 0;
		break;
	case fdRS485A:
		//frtos_open_uart1( flags );
        xRet = 0;
		break;
	case fdTERM:
		frtos_open_uart2( flags );
        xRet = 0;
		break;
    case fdI2C0:
		xRet = frtos_open_i2c( &xBusI2C0, fd, &I2C0_xMutexBuffer, flags );
		break;
	default:
		break;
	}

	return(xRet);
}
//------------------------------------------------------------------------------
int16_t frtos_ioctl( file_descriptor_t fd, uint32_t ulRequest, void *pvValue )
{

int16_t xRet = -1;

	switch(fd) {
	case fdXCOMMS:
		//xRet = frtos_ioctl_uart0( ulRequest, pvValue );
		break;
	case fdRS485A:
		//xRet = frtos_ioctl_uart1( ulRequest, pvValue );
		break;
	case fdTERM:
		xRet = frtos_ioctl_uart2( ulRequest, pvValue );
		break;
    case fdI2C0:
        xRet = frtos_ioctl_i2c( &xBusI2C0, ulRequest, pvValue );
        break;
    default:
		break;
	}

	return(xRet);
}
//------------------------------------------------------------------------------
int16_t frtos_write( file_descriptor_t fd ,const char *pvBuffer, const uint16_t xBytes )
{

int16_t xRet = -1;

	switch(fd) {
	case fdXCOMMS:
		//xRet = frtos_write_uart0( pvBuffer, xBytes );
		break;
	case fdRS485A:
		//xRet = frtos_write_uart1( pvBuffer, xBytes );
		break;
	case fdTERM:
		xRet = frtos_write_uart2( pvBuffer, xBytes );;
		break;
    case fdI2C0:
		xRet = frtos_write_i2c( &xBusI2C0, pvBuffer, xBytes );
		break;
	default:
		break;
	}

	return(xRet);
}
//------------------------------------------------------------------------------
int16_t frtos_read( file_descriptor_t fd , char *pvBuffer, uint16_t xBytes )
{

int16_t xRet = -1;

	switch(fd) {
	case fdXCOMMS:
		//xRet = frtos_read_uart0( pvBuffer, xBytes );
		break;
	case fdRS485A:
		//xRet = frtos_read_uart1( pvBuffer, xBytes );
		break;
	case fdTERM:
		xRet = frtos_read_uart2( pvBuffer, xBytes );
		break;
    case fdI2C0:
		xRet = frtos_read_i2c( &xBusI2C0, pvBuffer, xBytes );
		break;
	default:
		break;
	}

	return(xRet);
}
//------------------------------------------------------------------------------
// FUNCIONES ESPECIFICAS DE UART's
//------------------------------------------------------------------------------
void frtos_open_uart2( uint32_t baudrate)
{
    drv_uart2_init( baudrate );
}
//------------------------------------------------------------------------------
int16_t frtos_write_uart2( const char *pvBuffer, const uint16_t xBytes )
{
    /*
     * Transmito por poleo escribiendo de a 1 byte en el puerto serial
     * No uso interrupciones
     *
     */

uint16_t i;
    
    for( i = 0; i < xBytes; i++) {
        while(! USART_IsTXDataRegisterEmpty(&USARTF0) )
            ;
        USART_PutChar(&USARTF0, pvBuffer[i]);
    }
    vTaskDelay( ( TickType_t)( 1 ) );
    return(xBytes);   

}
//------------------------------------------------------------------------------
int16_t frtos_ioctl_uart2( uint32_t ulRequest, void *pvValue )
{

int16_t xReturn = 0;

	switch( ulRequest )
	{

		case ioctl_UART_CLEAR_RX_BUFFER:
			rBchar_Flush(&RXRB_uart2);
			break;
		case ioctl_UART_CLEAR_TX_BUFFER:
			rBchar_Flush(&TXRB_uart2);
			break;
		default :
			xReturn = -1;
			break;
	}

	return xReturn;

}
//------------------------------------------------------------------------------
int16_t frtos_read_uart2( char *pvBuffer, uint16_t xBytes )
{
	// Lee caracteres de la cola de recepcion y los deja en el buffer.
	// El timeout lo fijo con ioctl.

	// Lee caracteres de la cola de recepcion y los deja en el buffer.
	// El timeout lo fijo con ioctl.

int16_t xBytesReceived = 0U;
TickType_t xTicksToWait = 10;
TimeOut_t xTimeOut;

     /* Initialize xTimeOut.  This records the time at which this function was
        entered. 
      */
	vTaskSetTimeOutState( &xTimeOut );

	// Are there any more bytes to be received?
	while( xBytesReceived < xBytes )
	{

        if( rBchar_Pop( &RXRB_uart2, &((char *)pvBuffer)[ xBytesReceived ] ) == true ) {
			xBytesReceived++;
            /*
             Recibi un byte. Re-inicio el timeout.
             */
            vTaskSetTimeOutState( &xTimeOut );
			//taskYIELD();
            //vTaskDelay( ( TickType_t)( 1 ) );
		} else {
			// Espero xTicksToWait antes de volver a chequear
			vTaskDelay( ( TickType_t)( 1 ) );

            // Time out has expired ?
            if( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) != pdFALSE )
            {
                break;
            }
        }

    }

	return ( xBytesReceived );

}
//------------------------------------------------------------------------------
// FUNCIONES ESPECIFICAS DEL BUS I2C/TWI
//------------------------------------------------------------------------------
int16_t frtos_open_i2c( periferico_i2c_port_t *xI2c, file_descriptor_t fd, StaticSemaphore_t *i2c_semph, uint32_t flags)
{
	// Asigno las funciones particulares ed write,read,ioctl
	xI2c->fd = fd;
	xI2c->xBusSemaphore = xSemaphoreCreateMutexStatic( i2c_semph );
	xI2c->xBlockTime = (10 / portTICK_PERIOD_MS );
	xI2c->i2c_error_code = I2C_OK;
	//
	// Abro e inicializo el puerto I2C solo la primera vez que soy invocado
	drv_I2C_init();
	return(1);
}
//------------------------------------------------------------------------------
int16_t frtos_ioctl_i2c( periferico_i2c_port_t *xI2c, uint32_t ulRequest, void *pvValue )
{

int16_t xReturn = 0;
    
uint32_t *p = NULL;

	p = pvValue;

	switch( ulRequest )
	{
		case ioctl_OBTAIN_BUS_SEMPH:
			// Espero el semaforo en forma persistente.
			while ( xSemaphoreTake(xI2c->xBusSemaphore, ( TickType_t ) 5 ) != pdTRUE )
				taskYIELD();
			break;
			case ioctl_RELEASE_BUS_SEMPH:
				xSemaphoreGive( xI2c->xBusSemaphore );
				break;
			case ioctl_SET_TIMEOUT:
				xI2c->xBlockTime = *p;
				break;
			case ioctl_I2C_SET_DEVADDRESS:
				xI2c->devAddress = (int8_t)(*p);
				break;
			case ioctl_I2C_SET_DATAADDRESS:
				xI2c->dataAddress = (uint16_t)(*p);
				break;
			case ioctl_I2C_SET_DATAADDRESSLENGTH:
				xI2c->dataAddress_length = (int8_t)(*p);
				break;
			case ioctl_I2C_GET_LAST_ERROR:
				xReturn = xI2c->i2c_error_code;
				break;
            case ioctl_I2C_SET_DEBUG:
				drv_I2C_config_debug( true);
				break;
            case ioctl_I2C_CLEAR_DEBUG:
				drv_I2C_config_debug( false);
				break;
            case ioctl_I2C_RESET:
				drv_I2C_reset();
				break;                
			default :
				xReturn = -1;
				break;
		}

	return xReturn;

}
//------------------------------------------------------------------------------
int16_t frtos_read_i2c( periferico_i2c_port_t *xI2c, char *pvBuffer, const uint16_t xBytes )
{

int16_t xReturn = 0U;

	if ( ( xReturn = drv_I2C_master_read(xI2c->devAddress, xI2c->dataAddress, xI2c->dataAddress_length, (char *)pvBuffer, xBytes)) > 0 ) {
		xI2c->i2c_error_code = I2C_OK;
	} else {
		// Error de escritura indicado por el driver.
		xI2c->i2c_error_code = I2C_RD_ERROR;
	}
    return(xReturn);
}
//------------------------------------------------------------------------------
int16_t frtos_write_i2c( periferico_i2c_port_t *xI2c, const char *pvBuffer, const uint16_t xBytes )
{

int16_t xReturn = 0U;

	if ( ( xReturn = drv_I2C_master_write( xI2c->devAddress, xI2c->dataAddress, xI2c->dataAddress_length, (char *)pvBuffer, xBytes) ) > 0 ) {
		xI2c->i2c_error_code = I2C_OK;
	} else {
		// Error de escritura indicado por el driver.
		xI2c->i2c_error_code = I2C_WR_ERROR;
	}

	return(xReturn);

}
//------------------------------------------------------------------------------