/**
 * -------------------------------------------------------------------
 * Copyright (c) 2010 - Todos los derechos reservados
 * Manuel Eduardo Vieda Salomon <me.vieda372@gmail.com
 * Ingeniero Electronico, Ingeniero de Sistemas y Computacion
 * Universidad de los Andes. Bogota, Colombia.
 * -------------------------------------------------------------------
 *
 * Archivo:				DS1307.h
 * Fecha Creacion:		Junio 01 de 2010
 * Fecha Modificacion:	Junio 11 de 2010
 * Version (Revision):	0.1 (1)
 *
 * Descripcion:	Libreria que facilita la interaccion con el reloj de tiempo
 *              real DS1307 bajo el protocolo I2C/TWI
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


#ifndef _DS1307_H_
#define _DS1307_H_


//-------------------------------------------------------------------------------
//  PARAMAETROS DE CONFIGURACION QUE PUEDEN SER MODIFICADOS POR EL USUARIO
//-------------------------------------------------------------------------------


/**
 * Definimos el ID y la direccion de esclavo para el reloj de tiempo real DS1307
 */
#define DS1307_ID 0x0D
#define DS1307_ADDR 0x00


/**
 * Definimos el formato de la Hora
 * DS1307_12_24 = 0 Formato 24 Horas (Militar)
 * DS1307_12_24 = 1 Formato 12 Horas (am/pm)
 */
#define DS1307_12_24 1



//-------------------------------------------------------------------------------
//  FUNCIONES OFRECIDAS QUE PUEDEN SER LLAMADAS POR EL USUARIO
//-------------------------------------------------------------------------------

extern char DS1307_hora[10];	// XX:XX:XXp
extern char DS1307_fecha[10];	// dd/mm/aaaa;


uint8_t DS1307_Init(void);

void DS1307_ActualizarFecha(void);
void DS1307_ActualizarHora(void);


uint8_t DS1307_CambiarFecha(uint8_t dia, uint8_t mes, uint8_t ano);
uint8_t DS1307_CambiarHora(uint8_t seg, uint8_t min, uint8_t hrs, uint8_t formato, uint8_t ap);

#endif /*_DS1301_H*/


