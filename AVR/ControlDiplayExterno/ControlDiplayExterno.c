/**
 * -------------------------------------------------------------------
 * Universidad de los Andes - Facultad de Ingenieria
 * Departamento de Ingenieria Electrica y Electronica
 * Proyecto Taller de Electrónica
 * -------------------------------------------------------------------
 *
 * Archivo:				ControlDisplayExterno.c
 * Fecha Creacion:		Abril 24 de 2010
 * Fecha Modificacion:	Abril 25 de 2010
 * Version (Revision):	0.1 (1)
 *
 * Descripcion:	Control diplay externo
 *
 */


/* Definición de Constantes del Sistema */
#define F_CPU 8000000UL

/* Exportación de Librerias */
#include "includes/defines.h"

#include <stdio.h>			// Standard IO facilities	
#include <stdbool.h>		// Standar Boolean facilities
#include <avr/io.h>			// AVR device-specific IO definitions
#include <util/delay.h>		// Convenience functions for busy-wait delay loops
#include <avr/interrupt.h>	// Interrupts handling
#include <avr/pgmspace.h>	// Program Space Utilities

#include "includes/USART.h"

//  ------------------------------------------------------------------------
//	VARIABLES GLOBALES DEL SISTEMA / BUFFERS
//  ------------------------------------------------------------------------
uint8_t estado;
//  ------------------------------------------------------------------------
//	DECLARACION DE METODOS / PROCEDIMIENTOS INTERNOS
//  ------------------------------------------------------------------------
void initHardware();
void enviarHora(uint8_t numero);
void enviarDato(uint8_t numero1, uint8_t numero2);




/**
 *  Manejo de la interrupcion generada cuando la USART0 recibe un dato
 */
ISR(USART_RX_vect) 
{
	uint8_t dato;
	dato = USART0_Recibir();

	switch(estado){
		case 0x00: 	enviarHora(NUM0); estado=0x01; break;
		case 0x01: 	enviarHora(NUM1); estado=0x00; break;
		default: estado = 0x00;
	}
}



//  ------------------------------------------------------------------------
//	PROGRAMA PRINCIPAL DEL SISITEMA
//  ------------------------------------------------------------------------

int main(void) {

	_delay_ms(40);
	estado = 0x00;
	initHardware();
	sei();	// Enable the Global Interrupt Enable flag so that interrupts can be processed

	while(1){}


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
	
	USART0_init();
	USART0_EnableRx();

	
	//Inicializamos puerto para datos del display
	CLK_DATO_DDR |= _BV(PIN_CLK_DATOS);
	CLK_HORA_DDR |= _BV(PIN_CLK_HORA);
	DATO1_DDR    |= _BV(PIN_DATO_DATOS1);
	DATO2_DDR    |= _BV(PIN_DATO_DATOS2);
	HORA_DDR     |= _BV(PIN_DATO_HORA);

}


void enviarDato(uint8_t numero1, uint8_t numero2){
	int i=0;
	for(i=0; i<8; i++){
		CLK_DATO_PORT &= ~(_BV(PIN_CLK_DATOS));
		DATO1_PORT &= ~(_BV(PIN_DATO_DATOS1));
		DATO2_PORT &= ~(_BV(PIN_DATO_DATOS2));
		if(numero1&(_BV(i))){
			DATO1_PORT |= _BV(PIN_DATO_DATOS1);
		}
		if(numero2&(_BV(i))){
			DATO2_PORT |= _BV(PIN_DATO_DATOS2);
		}
		CLK_DATO_PORT |= _BV(PIN_CLK_DATOS);
	}

}

void enviarHora(uint8_t numero){

	int i=0;
	for(i=0; i<8; i++){
		CLK_HORA_PORT &= ~(_BV(PIN_CLK_HORA));
		HORA_PORT &= ~(_BV(PIN_DATO_HORA));
		if(numero&(_BV(i))){
			HORA_PORT |= _BV(PIN_DATO_HORA);
		}
		CLK_HORA_PORT |= (_BV(PIN_CLK_HORA));
	}

}

