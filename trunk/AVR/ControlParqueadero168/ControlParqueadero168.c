/**
 * -------------------------------------------------------------------
 * Universidad de los Andes - Facultad de Ingenieria
 * Departamento de Ingenieria Electrica y Electronica
 * Proyecto Taller de Electrónica
 * -------------------------------------------------------------------
 *
 * Archivo:				ControlParqueadero168.c
 * Fecha Creacion:		Mayo 28 de 2010
 * Fecha Modificacion:	Mayo 28 de 2010
 * Version (Revision):	0.1 (1)
 *
 * Descripcion:	Control principal proyecto Parqueadero Edificio SantoDomingo
 *               Microcontrolador Esclavo
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


//  ------------------------------------------------------------------------
//	PROGRAMA PRINCIPAL DEL SISITEMA
//  ------------------------------------------------------------------------

int main(void) {


	_delay_ms(40);
	initHardware();

	uint8_t DS1307_data2[8];

	DS1307_data2[0] = 0x05;
	DS1307_data2[1] = 0x06;
	DS1307_data2[2] = 0x00;
	DS1307_data2[3] = 0x00;
	DS1307_data2[4] = 0x00;
	DS1307_data2[5] = 0x00;
	DS1307_data2[6] = 0x00;
	DS1307_data2[7] = 0x00;

	while(1){
		_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);
		_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);
		_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);
		_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);
		_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);
		_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);

		I2C_EscribirBytes(0x00, 0x01, 0x00, 2, DS1307_data2);

		DS1307_data2[0] = DS1307_data2[0]++;
		DS1307_data2[1] = DS1307_data2[0];
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

	// Deshabilitamos Interrupciones Generales
	cli();

	// Inicializamos el modulo I2C/TWI
	I2C_Init();

	// Inicializamos el modulo de comunicacion SPI
	//SPI_init();


	/*

DDRD = 0xFF;
PORTD = 0xFF;
PORTD  = 0xAA;

	
	
	
PORTD = 0x55;
SD_init();

*/


}


