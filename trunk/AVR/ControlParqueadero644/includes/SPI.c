/**
 * -------------------------------------------------------------------
 * Copyright (c) 2010 - Todos los derechos reservados
 * Manuel Eduardo Vieda Salomon <me.vieda372@gmail.com
 * Ingeniero Electronico, Ingeniero de Sistemas y Computacion
 * Universidad de los Andes. Bogota, Colombia.
 * -------------------------------------------------------------------
 *
 * Archivo:				SPI.c
 * Fecha Creacion:		Mayo  28 de 2010
 * Fecha Modificacion:	Mayo  28 de 2010
 * Version (Revision):	0.1 (1)
 *
 * Descripcion:	Libreria que facilita el uso del modulo de comunicación
 *              SPI de los microcontroladores ATmega 48/88/168/644
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
 #include "SPI.h"



#define PEGAR(a, b)		a##b
#define PORT(x)			PEGAR(PORT, x)
#define PIN(x)			PEGAR(PIN, x)
#define DDR(x)			PEGAR(DDR, x)

#define SPI_DDR  DDR(PUERTO_SPI)
#define SPI_PORT PORT(PUERTO_SPI)



 /**
 * SPISlave_init
 * Este metodo se encarga de realizar la inicializacion del modulo SPI como maestro o esclavo
 * del microcontrolador, teniendo en cuenta las opciones de configuracion que se tienen
 * en el archivo SPI.h
 *
 * @PARAMS: Ninguno
 * @PRE:    El voltaje ya se debe haber estabilizado tanto en el micro como en perifericos
 * @POS:    Se inicializa el modulo de comunicacion SPI del microcontrolador
 * @RETURN: Void
 */
void SPI_init(void){

	if(SPI_MSTR){
		// Se inicializa el microcontrolador como Maestro
		// Se configura el pin MISO como entrada; SCK, MOSI y SS como salida
		SPI_DDR &= ~_BV(SPI_MISO);
		SPI_DDR |= _BV(SPI_SS)|_BV(SPI_SCK)|_BV(SPI_MOSI);
		SPI_PORT |= _BV(SPI_SS)|_BV(SPI_MOSI)|_BV(SPI_MISO)|_BV(SPI_SCK);

	} else{
		// Se inicializa el microcontrolador como Esclavo
		// Se configura el pin SCK, MOSI y SS como entreda; MISO como salida
		SPI_DDR &= ~((_BV(SPI_SS))|(_BV(SPI_MOSI))|(_BV(SPI_SCK)));
		SPI_DDR |= _BV(SPI_MISO);
		SPI_PORT |= _BV(SPI_SS)|_BV(SPI_MOSI)|_BV(SPI_MISO)|_BV(SPI_SCK);

	}

	SPCR = 0x00;
	SPCR |= (SPI_INT)?_BV(SPIE):0;
	SPCR |= (SPI_DATA_ORDER)?_BV(DORD):0;
	SPCR |= (SPI_MSTR)?_BV(MSTR):0;
	SPCR |= (SPI_CPOL)?_BV(CPOL):0;
	SPCR |= (SPI_CHPA)?_BV(CPHA):0;

	switch(SPI_SPR){
		case 16:SPCR |= _BV(SPR0);break;
		case 64:SPCR |= _BV(SPR1);break;
		case 128:SPCR |= _BV(SPR0)|_BV(SPR1);break;
		default:break;
	}

	SPSR = 0x00;
	SPSR |= (SPI_2X)?_BV(SPI2X):0;

	SPCR |= _BV(SPE);

}



 /*
 * SPI_Enviar
 * Este metodo envia un byte a traves del modulo SPI
 * @PARAMS: uint8_t dato -> Dato que se desea enviar
 * @PRE:    El modulo SPI se encuentra configurado y habilitado para el envio de mensajes
 * @POST:   Se coloca el mensaje en el buffer de salida
 * @RETURN: Void
 */
void SPI_Enviar(uint8_t dato){
	
	// Se inicializa la transmision
	SPDR = dato;
	// Se espera a que la transmision se complete
	while(!(SPSR & _BV(SPIF)));
}

/**
 * SPI_Recibir
 * Este metodo recibe un byte a traves del modulo 
 * @PARAMS: Ninguno
 * @PRE:    El modulo SPI se encuentra configurado y habilitado para la recepcion de mensajes
 * @POST:   Se retorno el mensaje que se almaceno en el buffer de entrada del SPI
 * @RETURN: uint8_t dato -> El dato recibido en la transmision
 */
uint8_t SPI_Recibir(){

	// Se espera a que la transmision se complete
	while(!(SPSR & _BV(SPIF)));
	return SPDR;
}
