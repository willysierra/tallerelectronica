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

#define SEG1 _delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);
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

uint8_t display[10] = {0,0,0,0,0,0,0,0,0,0};
//  ------------------------------------------------------------------------
//	DECLARACION DE METODOS / PROCEDIMIENTOS INTERNOS
//  ------------------------------------------------------------------------
void initHardware();
void enviarHora(uint8_t numero);
void enviarDato(uint8_t numero1, uint8_t numero2);
void testDisplay();

void actualizarInfo();
uint8_t darValor(uint8_t num);



/**
 *  Manejo de la interrupcion generada cuando la USART0 recibe un dato
 */
ISR(USART_RX_vect) 
{

	static int i;
	uint8_t dato;
	dato = USART0_Recibir();

	if(dato=='A'){
		i=0;
	} else{

		if(i<10){
			display[i++] = dato;
			if(i==10){
				actualizarInfo();
				USART0_EnviarLn("Ok");
				}
		}
	}

	

}


ISR(USART_TX_vect) 
{

}



//  ------------------------------------------------------------------------
//	PROGRAMA PRINCIPAL DEL SISITEMA
//  ------------------------------------------------------------------------

int main(void) {

	_delay_ms(40);
	estado = 0x00;
	initHardware();
	sei();	// Enable the Global Interrupt Enable flag so that interrupts can be processed

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
	
	USART0_init();
	USART0_EnableRx();
	USART0_EnableTx();

	
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

void actualizarInfo(){

	enviarHora(darValor(display[6]));
	enviarHora(darValor(display[7]));
	enviarHora(darValor(display[8]));
	enviarHora(darValor(display[9]));


	enviarDato(darValor(display[0]), darValor(display[3]));
	enviarDato(darValor(display[1]), darValor(display[4]));
	enviarDato(darValor(display[2]), darValor(display[5]));

}
uint8_t darValor(uint8_t num){

	uint8_t resp;

	switch(num){
		case 1:
			resp = NUM1;
			break;
		case 2:
			resp = NUM2;
			break;
		case 3:
			resp = NUM3;
			break;
		case 4:
			resp = NUM4;
			break;
		case 5:
			resp = NUM5;
			break;
		case 6:
			resp = NUM6;
			break;
		case 7:
			resp = NUM7;
			break;
		case 8:
			resp = NUM8;
			break;
		case 9:
			resp = NUM9;
			break;
		case 0:
			resp = NUM0;
			break;
		case 10:
			resp = NUMOFF;
			break;
		default:
			resp = NUMOFF;
			break;
	}

	return resp;
}


void testDisplay(){
	int i =0;
	while(1)
	{
		SEG1

		switch(i++){
			case 0:
				enviarHora(NUM0);
				enviarHora(NUM0);
				enviarHora(NUM0);
				enviarHora(NUM0);
				enviarDato(NUM0, NUM0);
				enviarDato(NUM0, NUM0);
				enviarDato(NUM0, NUM0);
				break;
			case 1:
				enviarHora(NUM1);
				enviarHora(NUM1);
				enviarHora(NUM1);
				enviarHora(NUM1);
				enviarDato(NUM1, NUM1);
				enviarDato(NUM1, NUM1);
				enviarDato(NUM1, NUM1);
				break;
			case 2:
				enviarHora(NUM2);
				enviarHora(NUM2);
				enviarHora(NUM2);
				enviarHora(NUM2);
				enviarDato(NUM2, NUM2);
				enviarDato(NUM2, NUM2);
				enviarDato(NUM2, NUM2);
				break;
			case 3:
				enviarHora(NUM3);
				enviarHora(NUM3);
				enviarHora(NUM3);
				enviarHora(NUM3);
				enviarDato(NUM3, NUM3);
				enviarDato(NUM3, NUM3);
				enviarDato(NUM3, NUM3);
				break;
			case 4:
				enviarHora(NUM4);
				enviarHora(NUM4);
				enviarHora(NUM4);
				enviarHora(NUM4);
				enviarDato(NUM4, NUM4);
				enviarDato(NUM4, NUM4);
				enviarDato(NUM4, NUM4);
				break;
			case 5:
				enviarHora(NUM5);
				enviarHora(NUM5);
				enviarHora(NUM5);
				enviarHora(NUM5);
				enviarDato(NUM5, NUM5);
				enviarDato(NUM5, NUM5);
				enviarDato(NUM5, NUM5);
				break;
			case 6:
				enviarHora(NUM6);
				enviarHora(NUM6);
				enviarHora(NUM6);
				enviarHora(NUM6);
				enviarDato(NUM6, NUM6);
				enviarDato(NUM6, NUM6);
				enviarDato(NUM6, NUM6);
				break;
			case 7:
				enviarHora(NUM7);
				enviarHora(NUM7);
				enviarHora(NUM7);
				enviarHora(NUM7);
				enviarDato(NUM7, NUM7);
				enviarDato(NUM7, NUM7);
				enviarDato(NUM7, NUM7);
				break;
			
			case 8:
				enviarHora(NUM8);
				enviarHora(NUM8);
				enviarHora(NUM8);
				enviarHora(NUM8);
				enviarDato(NUM8, NUM8);
				enviarDato(NUM8, NUM8);
				enviarDato(NUM8, NUM8);
				break;

			case 9:
				enviarHora(NUM9);
				enviarHora(NUM9);
				enviarHora(NUM9);
				enviarHora(NUM9);
				enviarDato(NUM9, NUM9);
				enviarDato(NUM9, NUM9);
				enviarDato(NUM9, NUM9);

			default:
				i=0;
				break;
		}

	}
}
