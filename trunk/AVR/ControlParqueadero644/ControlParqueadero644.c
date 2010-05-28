/**
 * -------------------------------------------------------------------
 * Universidad de los Andes - Facultad de Ingenieria
 * Departamento de Ingenieria Electrica y Electronica
 * Proyecto Taller de Electrónica
 * -------------------------------------------------------------------
 *
 * Archivo:				ControlParqueadero644.c
 * Fecha Creacion:		Mayo 10 de 2010
 * Fecha Modificacion:	Mayo 17 de 2010
 * Version (Revision):	0.1 (1)
 *
 * Descripcion:	Control principal proyecto Parqueadero Edificio SantoDomingo
 *
 */

/* Exportación de Librerias */

#include "includes/defines.h"

#include <stdio.h>			// Standard IO facilities	
#include <stdbool.h>		// Standar Boolean facilities
#include <avr/io.h>			// AVR device-specific IO definitions
#include <util/delay.h>		// Convenience functions for busy-wait delay loops
#include <avr/interrupt.h>	// Interrupts handling
#include <avr/pgmspace.h>	// Program Space Utilities
#include <avr/eeprom.h>		// EEPROM Handling

#include "includes/lcd_HD44780_4.h"
#include "includes/USART.h"
#include "includes/SPI.h"
#include "includes/I2C.h"
#include "includes/SDCard.h"

//  ------------------------------------------------------------------------
//	VARIABLES GLOBALES DEL SISTEMA / BUFFERS
//  ------------------------------------------------------------------------


//  ------------------------------------------------------------------------
//	DECLARACION DE METODOS / PROCEDIMIENTOS INTERNOS
//  ------------------------------------------------------------------------
void initHardware();

/**
 *  Manejo de la interrupcion generada cuando la USART0 recibe un dato
 */
ISR(USART0_RX_vect) 
{
	LCD_enviarDTA('X');LCD_esperarListo();
}


ISR(USART0_TX_vect) 
{
	LCD_enviarDTA(0xFF);LCD_esperarListo();
}


//  ------------------------------------------------------------------------
//	PROGRAMA PRINCIPAL DEL SISITEMA
//  ------------------------------------------------------------------------

int main(void) {


	_delay_ms(40);
	initHardware();
	sei();	// Enable the Global Interrupt Enable flag so that interrupts can be processed 

	
	LCD_linea1Pos0();LCD_esperarListo();
	uint8_t dato = '0';
	
	while(1){
		_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);
		USART0_Enviar(dato++);

	}

}




/**
 * Metodo que se encarga de la inicializacion del hardware del proyecto. Dentro del 
 * hardware se encuentran tanto los periféricos externos como los internos del micro.
 * @PARAM:  Ninguno
 * @PRE:    Ninguno
 * @POST:   Se inicializa el hardware. Listo para uso rutinario
 * @RETURN: Void
 */
void initHardware(){

	// Inicializamos el LCD
	LCD_init();

	// Inicializamos el Teclado Matricial
	KBR_DDR  = 0x0F; 
	KBR_PORT = 0x00;

	// Inicializamos Puerto Serial Sincrono (USART0) para comunicacion por modulo RF
	USART0_init();
	USART0_EnableTx();
}





