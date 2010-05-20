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

//  ------------------------------------------------------------------------
//	VARIABLES GLOBALES DEL SISTEMA / BUFFERS
//  ------------------------------------------------------------------------


//  ------------------------------------------------------------------------
//	DECLARACION DE METODOS / PROCEDIMIENTOS INTERNOS
//  ------------------------------------------------------------------------
void initHardware();

//  ------------------------------------------------------------------------
//	PROGRAMA PRINCIPAL DEL SISITEMA
//  ------------------------------------------------------------------------

int main(void) {


	_delay_ms(40);
	initHardware();


	
	
	

	//LCD_enviarDTA('E');LCD_esperarListo();

	LCD_linea1Pos0();
	LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();


	LCD_linea2Pos0(); LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();





	LCD_linea3Pos0(); LCD_esperarListo();
		LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();




	LCD_linea4Pos0(); LCD_esperarListo();
		LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(' ');LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();
	LCD_enviarDTA(0xFF);LCD_esperarListo();


	while(1){
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

	// Inicializamos Puerto Serial Sincrono (USART-1) para comunicación con micro alterno

	// Inicializamos Puerto Serial Asincrono (UART-0) para modulo RF
	

}
