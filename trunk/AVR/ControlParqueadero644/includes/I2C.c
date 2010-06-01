/**
 * -------------------------------------------------------------------
 * Copyright (c) 2010 - Todos los derechos reservados
 * Manuel Eduardo Vieda Salomon <me.vieda372@gmail.com
 * Ingeniero Electronico, Ingeniero de Sistemas y Computacion
 * Universidad de los Andes. Bogota, Colombia.
 * -------------------------------------------------------------------
 *
 * Archivo:				I2C.c
 * Fecha Creacion:		Mayo  28 de 2010
 * Fecha Modificacion:	Junio 01 de 2010
 * Version (Revision):	0.1 (1)
 *
 * Descripcion:	Libreria que facilita el uso del modulo de comunicación
 *              bajo el protocolo I2C/TWI de los microcontroladores ATmega
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
 #include <avr/pgmspace.h>
 #include <util/delay.h>		// Convenience functions for busy-wait delay loops
 #include <util/twi.h>
 #include "I2C.h"

#define I2C_START 0
#define I2C_DATA  1
#define I2C_STOP  2



void I2C_Init(void){

	// Iniciamos todos los registros en sus valores por defecto
	TWBR = 0x00;	// TWI Bit Rate Register
	TWCR = 0x00;	// TWI Control Register
	TWSR = 0xF8;	// TWI Status Register
	TWAMR = 0x00;	// TWI Address Mask Register

	TWBR = I2C_TWBR;
	TWAR = (I2C_ADDR<<1) | I2C_GENCAL;

	// Se inicializa como Esclavo, atento a recibir un llamado con ACK
	//TWCR = _BV(TWEA)|_BV(TWEN)|_BV(TWIE);

}


uint8_t I2C_Enviar(uint8_t type){

	switch(type) {
		case I2C_START:    // Send Start Condition
			TWCR = _BV(TWINT)|_BV(TWSTA)|_BV(TWEN);
			break;
			
		case I2C_DATA:     // Send Data
			TWCR = _BV(TWINT)|_BV(TWEN);
			break;

		case I2C_STOP:     // Send Stop Condition
			TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWSTO);
			return 0;
			break;
	}

	// Wait for TWINT flag set in TWCR Register
	while (!(TWCR & _BV(TWINT)));
	// Return TWI Status Register, mask the prescaler bits (TWPS1,TWPS0)
	return (TW_STATUS);
}


int I2C_EscribirBytes(uint8_t sla_id, uint8_t sla_addr, uint8_t sla_mem_addr, int len, uint8_t *buf){
	
	uint8_t n=0;
	uint8_t I2C_TWSR;
	int bytesEscritos =0;

	// Deshabilitamos la recepcion de datos
	TWCR &= ~(_BV(TWEA)|_BV(TWEN)|_BV(TWIE));

	restart:
		if(n++ >= I2C_MAX_TRIES)
			return -1;
		
	begin:
		// Enviamos condicion de START
		TWCR = _BV(TWINT)|_BV(TWSTA)|_BV(TWEN);		// Enviamos condicion de START
		while (!(TWCR & _BV(TWINT)));				// Esperamos a que finlaice la transmision
		I2C_TWSR = TW_STATUS;

		switch(I2C_TWSR){
			case TW_REP_START:		break;
			case TW_START:			break;
			case TW_MT_ARB_LOST:	goto begin;
			default:				return -1; break;
		}

		// Enviamos la direccion del esclavo  (SLA+W)
		TWDR = (((sla_id & 0x0F)<<4) | ((sla_addr & 0x07)<<1)) | TW_WRITE;
		TWCR = _BV(TWINT)|_BV(TWEN);				// Enviamos la direccion del esclavo con el bit R/~W para escribir
		while (!(TWCR & _BV(TWINT)));				// Esperamos a que finlaice la transmision
		I2C_TWSR = TWSR & 0xF8;

		switch(I2C_TWSR){
			case TW_MT_SLA_ACK:		break;			// Recibimos un ACK, el esclavo respondio al llamado
			case TW_MT_SLA_NACK:	goto restart;	// Se recibio NACK, se reincia handshake para invocar al esclavo
			case TW_MT_ARB_LOST:	goto begin;		// Se perdio control del bus, se repite todo el proceso
			default:				goto error;
		}

		// Enviamos la direccion del esclavo donde desamos comenzar a escribir
		TWDR = sla_mem_addr;
		TWCR = _BV(TWINT)|_BV(TWEN);				// Enviamos la direccion de memoria del esclavo donde inicia escritura
		while (!(TWCR & _BV(TWINT)));				// Esperamos a que finalice la transmision
		I2C_TWSR = TW_STATUS;

		switch(I2C_TWSR){
			case TW_MT_DATA_ACK:	break;
			case TW_MT_DATA_NACK:	goto quit;
			case TW_MT_ARB_LOST:	goto begin;
			default:				goto error;
		}

		// Enviamos los bytes que desamos escribir
		for(;len>0; len--){
			
			TWDR = *buf++;
			TWCR = _BV(TWINT)|_BV(TWEN);				// Enviamos el byte apuntado por buf 
			while (!(TWCR & _BV(TWINT)));				// Esperamos a que finlaice la transmision
			I2C_TWSR = TW_STATUS;

			switch(I2C_TWSR){
				case TW_MT_DATA_ACK:	bytesEscritos++; break;
				case TW_MT_DATA_NACK:	goto error;		// Esclavo no permite escritura
				default:				goto error;
			}
	

		}

	//Enviamos condicion de STOP y retornamos el numero de bytes escritos
	quit:
		TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWSTO);		// Enviamos condicion de STOP
		_delay_us(5);
		// Se inicializa como Esclavo, atento a recibir un llamado con ACK
		TWCR = _BV(TWEA)|_BV(TWEN)|_BV(TWIE);


		return bytesEscritos;

	// Se genera un error, enviamos condicion de STOP para liberar bus y retornamos -1 indicando el error
	error:
		TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWSTO);		// Enviamos condicion de STOP
		_delay_us(5);
		// Se inicializa como Esclavo, atento a recibir un llamado con ACK
		TWCR = _BV(TWEA)|_BV(TWEN)|_BV(TWIE);
		return -1;

}




int I2C_LeerBytes(uint8_t sla_id, uint8_t sla_addr, uint8_t sla_mem_addr, int len, uint8_t *buf){

	uint8_t n=0;
	uint8_t I2C_TWSR;
	uint8_t I2C_TWCR;
	int bytesLeidos = 0;

	// Deshabilitamos la recepcion de datos
	TWCR &= ~(_BV(TWEA)|_BV(TWEN)|_BV(TWIE));

	restart:
		if(n++ >= I2C_MAX_TRIES)
			return -1;


	begin:
		// Enviamos condicion de START
		TWCR = _BV(TWINT)|_BV(TWSTA)|_BV(TWEN);		// Enviamos condicion de START
		while (!(TWCR & _BV(TWINT)));				// Esperamos a que finlaice la transmision
		I2C_TWSR = TW_STATUS;

		switch(I2C_TWSR){
			case TW_REP_START:		break;
			case TW_START:			break;
			case TW_MT_ARB_LOST:	goto begin;
			default:				return -1; break;
		}


		// Enviamos la direccion del esclavo  (SLA+W)
		TWDR = (((sla_id & 0x0F)<<4) | ((sla_addr & 0x07)<<1)) | TW_WRITE;
		TWCR = _BV(TWINT)|_BV(TWEN);				// Enviamos la direccion del esclavo con el bit R/~W para escribir
		while (!(TWCR & _BV(TWINT)));				// Esperamos a que finlaice la transmision
		I2C_TWSR = TW_STATUS;

		switch(I2C_TWSR){
			case TW_MT_SLA_ACK:		break;			// Recibimos un ACK, el esclavo respondio al llamado
			case TW_MT_SLA_NACK:	goto restart;	// Se recibio NACK, se reincia handshake para invocar al esclavo
			case TW_MT_ARB_LOST:	goto begin;		// Se perdio control del bus, se repite todo el proceso
			default:				goto error;
		}

		// Enviamos la direccion del esclavo donde desamos comenzar a leer
		TWDR = sla_mem_addr;
		TWCR = _BV(TWINT)|_BV(TWEN);				// Enviamos la direccion de memoria del esclavo donde inicia escritura
		while (!(TWCR & _BV(TWINT)));				// Esperamos a que finalice la transmision
		I2C_TWSR = TW_STATUS;

		switch(I2C_TWSR){
			case TW_MT_DATA_ACK:	break;
			case TW_MT_DATA_NACK:	goto quit;
			case TW_MT_ARB_LOST:	goto begin;
			default:				goto error;
		}

		// Ahora se debe cambiar de modo Master Transmission al modo Master Receiver
		// Enviamos condicion de REPEATED START
		TWCR = _BV(TWINT)|_BV(TWSTA)|_BV(TWEN);		// Enviamos condicion de START
		while (!(TWCR & _BV(TWINT)));				// Esperamos a que finalice la transmision
		I2C_TWSR = TW_STATUS;

		switch(I2C_TWSR){
			case TW_REP_START:		break;
			case TW_START:			break;
			case TW_MR_ARB_LOST:	goto begin;
			default:				return -1; break;
		}

		// Enviamos la direccion del esclavo  (SLA+R)
		TWDR = (((sla_id & 0x0F)<<4) | ((sla_addr & 0x07)<<1)) | TW_READ;
		TWCR = _BV(TWINT)|_BV(TWEN);				// Enviamos la direccion del esclavo con el bit R/~W para leer
		while (!(TWCR & _BV(TWINT)));				// Esperamos a que finlaice la transmision
		I2C_TWSR = TW_STATUS;

		switch(I2C_TWSR){
			case TW_MR_SLA_ACK:		break;			// Recibimos un ACK, el esclavo respondio al llamado
			case TW_MR_SLA_NACK:	goto quit;		// Se recibio NACK, se reincia handshake para invocar al esclavo
			case TW_MR_ARB_LOST:	goto begin;		// Se perdio control del bus, se repite todo el proceso
			default:				goto error;
		}

		for(I2C_TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWEA) ; len>0; len--){

		
			if(len==1)
				I2C_TWCR = _BV(TWINT) | _BV(TWEN); 	// En el ultimo byte a leer enviamos un NACK para terminar comunicacion
			
			TWCR = I2C_TWCR;
			while (!(TWCR & _BV(TWINT)));				// Esperamos a que finalice la transmision
			I2C_TWSR = TW_STATUS;

			switch(I2C_TWSR){
				case TW_MR_DATA_NACK:	len=0;
				case TW_MR_DATA_ACK:	
										*buf++ = TWDR;
										bytesLeidos++;
										break;
				default:				goto error;
			}

		}

				
		//Enviamos condicion de STOP y retornamos el numero de bytes escritos
		// Leemos todos los bytes desde el esclavo y los almacenamos en el contenedor apuntado por buf

	quit:
		TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWSTO);		// Enviamos condicion de STOP
		_delay_us(5);
		// Se inicializa como Esclavo, atento a recibir un llamado con ACK
		TWCR = _BV(TWEA)|_BV(TWEN)|_BV(TWIE);
		return bytesLeidos;

	// Se genera un error, enviamos condicion de STOP para liberar bus y retornamos -1 indicando el error
	error:
		TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWSTO);		// Enviamos condicion de STOP
		_delay_us(5);
		// Se inicializa como Esclavo, atento a recibir un llamado con ACK
		TWCR = _BV(TWEA)|_BV(TWEN)|_BV(TWIE);
		return -1;
}

