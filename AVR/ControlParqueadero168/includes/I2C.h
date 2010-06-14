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
#define I2C_ADDR 0x02

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

static unsigned char TWI_Buff[I2C_BUFFER_SIZE];
static unsigned char TWI_BytesRecividos;

//-------------------------------------------------------------------------------
//  FUNCIONES OFRECIDAS QUE PUEDEN SER LLAMADAS POR EL USUARIO
//-------------------------------------------------------------------------------

void I2C_Init(void);
uint8_t I2C_Enviar(uint8_t type);
int I2C_EscribirBytes(uint8_t sla_id, uint8_t sla_addr, uint8_t sla_mem_addr, int len, uint8_t *buf);
int I2C_LeerBytes(uint8_t sla_id, uint8_t sla_addr, uint8_t sla_mem_addr, int len, uint8_t *buf);


#endif /* _I2C_H */
