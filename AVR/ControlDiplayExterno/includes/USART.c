/**
 * -------------------------------------------------------------------
 * Copyright (c) 2010 - Todos los derechos reservados
 * Manuel Eduardo Vieda Salomon <me.vieda372@gmail.com
 * Ingeniero Electronico, Ingeniero de Sistemas y Computacion
 * Universidad de los Andes. Bogota, Colombia.
 * -------------------------------------------------------------------
 *
 * Archivo:				USART.c
 * Fecha Creacion:		Mayo  15 de 2010
 * Fecha Modificacion:	Mayo  18 de 2010
 * Version (Revision):	0.1 (2)
 *
 * Descripcion:	Libreria que facilita el uso del modulo de comunicación
 *              asincrona y asincrona de los microcontroladores ATmega 48/88/168/644
 *
 *
 * --------------------------------------------------------------------
 * LICENCIA:
 * 
 * Se encuentra totalmente permitido el uso y distribucion en forma de codigo y
 * binario bajo la condicion de cumplir con las siguientes condiciones de uso:
 *
 *   - Toda redistribucion del codigo fuente debe mantener sin ningun cambio la
 *     presente notificacion de copyright, la lista de condiciones y la descripcion
 *     del archivo.
 *
 *   - Ni el nombre del poseedor del presente copyright o el nombre
 *     de cualquier colaborador puede ser usado para la promocion de productos
 *     derivados de este programa/codigo sin una autorizacion escrita y especifica
 *     por parte del autor.
 *
 *   - Si considera que este contenido le ha sido bastante util, puedes invitarme
 *     a una ronda de cervezas. (The Beer-Ware License, Revision 42)
 * ---------------------------------------------------------------------
 */

 #include "defines.h"
 #include <stdint.h>
 #include <avr/io.h>
 #include "USART.h"


/**
 * USAR_init
 * Este metodo se encarga de realizar la unicializacion del modulo USART que entra por
 * parametro y en el modo selecccionado (Sincrono o asincrono), configurando la velocidad
 * y demás parámetros con la definicion de las constantes en el archivo de definiciones
 *
 * @PARAMS: Ninguno
 * @PRE:    Los parametros se encuentran bien definidos
 * @POST:   Se inicializa el puerto USART seleccionado en el modo seleccionado, quedando
 *          listo para enviar y recibir datos.
 * @RETURN: Void
 */
void USART0_init(){

	// Nos aseguramos que la USART se encuentre inactiva y configuracion default
	UCSR0A = 0x20;
	UCSR0B = 0x00;
	UCSR0C = 0x06;
	

	// Asignamos el valor del valor de la tasa de transmision en baudios
	UBRR0L=UBRRVAL;			//Parte baja del byte
	UBRR0H=(UBRRVAL>>8);	//Parte alta del byte

	if(ASY_SYN){
		//Configurado en modo Asincrono
		if(SPEED_2X){UCSR0A |= _BV(U2X0);}
		
	}else{
		// Configurado en modo Sincrono
		UCSR0C |= (1<<6);		
	}

	switch(CHAR_SIZE){
		case 5: UCSR0C &= ~(_BV(UCSZ00)|_BV(UCSZ01));break;
		case 6: UCSR0C &= ~(_BV(UCSZ01));break;
		case 7: UCSR0C &= ~(_BV(UCSZ00));break;
		case 9: UCSR0B|= _BV(UCSZ02);break;
		default:break;
	}

	UCSR0B |= (INT_TX_COMPLETA)?_BV(TXCIE0):0;
	UCSR0B |= (INT_RX_COMPLETA)?_BV(RXCIE0):0;

	switch(CONTROL_PARIEDAD){
		case 1: UCSR0C |= _BV(UPM01); break;
		case 2: UCSR0C |= (_BV(UPM01)|_BV(UPM00)); break;
		default: break;
	}

	if(NUM_STOP==2) UCSR0C |= _BV(USBS0);
	if(CLK_POL) UCSR0C |= _BV(UCPOL0);
}



/**
 * USART0_EnableTx
 * @PARAMS: Ninguno
 * @PRE:    El puerto USART0 se encuentra inicializado
 * @POST:   El puerto USART0 se habilita para transmision 
 * @RETURN: Void
 */
void USART0_EnableTx(){
	UCSR0B |= _BV(TXEN0);
}

/**
 * USART0_DisableTx
 * @PARAMS: Ninguno
 * @PRE:    El puerto USART0 se encuentra inicializado
 * @POST:   El puerto USART0 se deshabilita para transmision 
 * @RETURN: Void
 */
void USART0_DisableTx(){
	UCSR0B &= ~_BV(TXEN0);
}

/**
 * USART0_EnableRx
 * @PARAMS: Ninguno
 * @PRE:    El puerto USART0 se encuentra inicializado
 * @POST:   El puerto USART0 se habilita para Recepsion 
 * @RETURN: Void
 */
void USART0_EnableRx(){
	UCSR0B |= _BV(RXEN0);
}


/**
 * USART0_DisableRx
 * @PARAMS: Ninguno
 * @PRE:    El puerto USART0 se encuentra inicializado
 * @POST:   El puerto USART0 se deshabilita para Recepcion 
 * @RETURN: Void
 */
 void USART0_DisableRx(){
 	UCSR0B &= ~_BV(RXEN0);
 }


 /**
  * USART0_Enviar
  * @PARAMS uint8_t dato -> Dato que se desea enviar
  * @PRE:   El puerto USART0 se encuentra configurado y habilitado para el envio de mensajes+
  * @POST:  Se coloca el mensaje en el buffer de salida de la USART0
  * @RETURN: Void
  */
void USART0_Enviar(uint8_t dato){

	// Espera a que se se termina la transmision anterior si esta en curso
	while ( !( UCSR0A & (1<<UDRE0)) );

	// Coloca el dato a enviar en el buffer de salida
	UDR0 = dato;

}


/**
 * USART0_Recibir
 * @PARAMS: Ninguno
 * @PRE:    El puerto USART0 se encuentra configurado y habilitado para la recepcion de mensajes
 * @POST:   Se retorno el mensaje que se almaceno en el buffer de entrada de la USART0
 * @RETURN: uint8_t dato -> El dato recibido en la transmision
 */
uint8_t USART0_Recibir(){

	// Se espera a que se reciba un mensaje si esta no ha llegado todavia
	while ( !(UCSR0A & (1<<RXC0)) );

	return UDR0;
}


void USART0_EnviarStrLen(int len, char *buf){

	for (; len > 0; len--){
		USART0_Enviar(*buf++);
	}


}
