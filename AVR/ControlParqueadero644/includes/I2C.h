/**
 * -------------------------------------------------------------------
 * Copyright (c) 2010 - Todos los derechos reservados
 * Manuel Eduardo Vieda Salomon <me.vieda372@gmail.com
 * Ingeniero Electronico, Ingeniero de Sistemas y Computacion
 * Universidad de los Andes. Bogota, Colombia.
 * -------------------------------------------------------------------
 *
 * Archivo:				I2C.h
 * Fecha Creacion:		Mayo  28 de 2010
 * Fecha Modificacion:	Mayo  28 de 2010
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


#ifndef _I2C_H_
#define _I2C_H_



//-------------------------------------------------------------------------------
//  PARAMAETROS DE CONFIGURACION QUE PUEDEN SER MODIFICADOS POR EL USUARIO
//-------------------------------------------------------------------------------


/**  Definimos la frecuencia del reloj para la interfaz I2C/TWI
 *  Fcsl = Fcpu/(16+2*I2C_TWBR*1)   Asumiendo un presacler de 0
 * Con I2C_TWBR 0x20 obtenemos una frecuancia de 100kHz, pero para tener un margen 
 * de error escogemos  I2C_TWBR 0x21 que nos da una frecuencia de 97.56KHz.
 * , sin embargo, 
 */
#define I2C_TWBR 0x21	
	

/** Direccion de esclavo del dispositivo
 * Corresponde a un valor de 7 bits, sin incluir el valor 0000000, correspondiente
 * a un llamado a todos los dispositivos conectados al bus (llamado general). 
 */
#define I2C_ADDR 0x01

/**
 * Define si se quiere habilitar (I2C_GENCAL=1) o deshabilitar (I2C_GENCAL=0) el 
 * reconocimiento de llamadas generales hechas en el bus al cual se encuentra conectado
 */
#define I2C_GENCAL 0


/** 
 * Definimos el numero maximo de intentos que se hacen para intentar comenzar una transmision
 */
#define I2C_MAX_TRIES 10


/**
 * Definimos el tamanio del buffer de entrada (Esclavo) para datos de entrada
 */
#define I2C_BUFFER_SIZE 4 


//-------------------------------------------------------------------------------
//  VARIABLES USADAS POR EL PROTOCOLO DE COMUNICACION I2C/TWI
//-------------------------------------------------------------------------------

extern unsigned char TWI_Buff[I2C_BUFFER_SIZE];
extern unsigned char TWI_BytesRecividos;


/** \name TWSR values

  Mnemonics:
  <br>TW_MT_xxx - master transmitter
  <br>TW_MR_xxx - master receiver
  <br>TW_ST_xxx - slave transmitter
  <br>TW_SR_xxx - slave receiver
 
*/

/* Master */

#define TW_START		0x08		// Start condition transmitted
#define TW_REP_START		0x10		// Repeated start condition transmitted

/* Master Transmitter */

#define TW_MT_SLA_ACK		0x18		// SLA+W transmitted, ACK received
#define TW_MT_SLA_NACK		0x20		// SLA+W transmitted, NACK received
#define TW_MT_DATA_ACK		0x28		// Data transmitted, ACK received
#define TW_MT_DATA_NACK		0x30		// Data transmitted, NACK received
#define TW_MT_ARB_LOST		0x38		// Arbitration lost in SLA+W or data

/* Master Receiver */

#define TW_MR_ARB_LOST		0x38		// Arbitration lost in SLA+R or NACK
#define TW_MR_SLA_ACK		0x40		// SLA+R transmitted, ACK received
#define TW_MR_SLA_NACK		0x48		// SLA+R transmitted, NACK received
#define TW_MR_DATA_ACK		0x50		// Data received, ACK returned
#define TW_MR_DATA_NACK		0x58		// Data received, NACK returned

/* Slave Transmitter*/

#define TW_ST_SLA_ACK		0xA8		// SLA+R received, ACK returned
#define TW_ST_ARB_LOST_SLA_ACK	0xB0		// Arbitration lost in SLA+RW, SLA+R received, ACK returned
#define TW_ST_DATA_ACK		0xB8		// Data transmitted, ACK received
#define TW_ST_DATA_NACK		0xC0		// Data transmitted, NACK received
#define TW_ST_LAST_DATA		0xC8		// Last data byte transmitted, ACK received


/* Slave Receiver */

#define TW_SR_SLA_ACK		0x60		// SLA+W received, ACK returned
#define TW_SR_ARB_LOST_SLA_ACK	0x68		// Arbitration lost in SLA+RW, SLA+W received, ACK returned
#define TW_SR_GCALL_ACK		0x70		// General call received, ACK returned
#define TW_SR_ARB_LOST_GCALL_ACK 0x78		// Arbitration lost in SLA+RW, general call received, ACK returned
#define TW_SR_DATA_ACK		0x80		// Data received, ACK returned
#define TW_SR_DATA_NACK		0x88		// Data received, NACK returned
#define TW_SR_GCALL_DATA_ACK	0x90		// General call data received, ACK returned
#define TW_SR_GCALL_DATA_NACK	0x98		// General call data received, NACK returned
#define TW_SR_STOP		0xA0		// Stop or repeated start condition received while selected 

/* Misc */

#define TW_NO_INFO		0xF8		// No state information available
#define TW_BUS_ERROR		0x00		// Illegal start or stop condition

/**
 * The lower 3 bits of TWSR are reserved on the ATmega163.
 * The 2 LSB carry the prescaler bits on the newer ATmegas.
 */
#define TW_STATUS_MASK		(_BV(TWS7)|_BV(TWS6)|_BV(TWS5)|_BV(TWS4)|\
				_BV(TWS3))
/**
 * TWSR, masked by TW_STATUS_MASK
 */
#define TW_STATUS		(TWSR & TW_STATUS_MASK)

/**
 * name R/~W bit in SLA+R/W address field
 */
#define TW_READ		1
#define TW_WRITE	0

//-------------------------------------------------------------------------------
//  FUNCIONES OFRECIDAS QUE PUEDEN SER LLAMADAS POR EL USUARIO
//-------------------------------------------------------------------------------

void I2C_Init(void);
uint8_t I2C_Enviar(uint8_t type);
int I2C_EscribirBytes(uint8_t sla_id, uint8_t sla_addr, uint8_t sla_mem_addr, int len, uint8_t *buf);
int I2C_LeerBytes(uint8_t sla_id, uint8_t sla_addr, uint8_t sla_mem_addr, int len, uint8_t *buf);
void I2C_AtenderInterrupcion(void);


#endif /* _I2C_H */
