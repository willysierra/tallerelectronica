 /**
 * -------------------------------------------------------------------
 * Universidad de los Andes - Facultad de Ingenieria
 * Departamento de Ingenieria Electrica y Electronica
 * Proyecto Taller de Electrónica
 * -------------------------------------------------------------------
 *
 * Archivo:				funciones.h
 * Fecha Creacion:		Junio 10 de 2010
 * Fecha Modificacion:	Junio 14 de 2010
 * Version (Revision):	0.1 (2)
 *
 * Descripcion: Contiene la programacion de las diferentes funciones con las que cuenta
 *              el dispositivo desarrollado y que son encargadas por el micro ATmega644
 */


#ifndef _FUNCIONES_H
#define _FUNCIONES_H


#define CONTRASENA 0
#define DATOS_PARQUEADERO 1
#define FECHA_HORA 2
#define CONFIGURACIONES 3

void validarContrasena(void);
void adminDatosParqueadero(void);
void adminFechaHora(void);
void adminConfiguraciones(void);

#endif /* _FUNCIONES_H_ */
