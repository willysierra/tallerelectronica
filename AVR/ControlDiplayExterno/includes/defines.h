 /**
 * -------------------------------------------------------------------
 * Universidad de los Andes - Facultad de Ingenieria
 * Departamento de Ingenieria Electrica y Electronica
 * Proyecto Taller de Electrónica
 * -------------------------------------------------------------------
 *
 * Archivo:				defines.h
 * Fecha Creacion:		Mayo 10 de 2010
 * Fecha Modificacion:	Mayo 17 de 2010
 * Version (Revision):	0.1 (1)
 *
 * Descripcion: Archivo de configuracion de las diferentes propiedades del proyecto,
 *				CPU, perifericos y opciones generales de compilacion.
 */


#ifndef _DEFINES_H_
#define _DEFINES_H_

/* Definimos la frecuencia del reloj de la CPU */
#define F_CPU 8000000UL


#define PEGAR(a, b)		a##b
#define PORT(x)			PEGAR(PORT, x)
#define PIN(x)			PEGAR(PIN, x)
#define DDR(x)			PEGAR(DDR, x)


/*
 * ---------------------------------------------------------------
 * Definicion del valor de los 8 bits conectados al display
 * (a, b, c, d, e, f, g) segun las conexiones fisica reales
 * ---------------------------------------------------------------
 */

#define NUM0 0xEE
#define NUM1 0x82
#define NUM2 0xDC
#define NUM3 0xD6
#define NUM4 0xB2
#define NUM5 0x76
#define NUM6 0x7E
#define NUM7 0xC2
#define NUM8 0xFF
#define NUM9 0xF6
#define NUMOFF 0x00
/*
 * ---------------------------------------------------------------
 * 
 * ---------------------------------------------------------------
 */

#define PUERTO_CLK_DATOS C
#define PIN_CLK_DATOS 4


#define PUERTO_CLK_HORA C
#define PIN_CLK_HORA 1

#define PUERTO_DATO_DATOS1 C
#define PIN_DATO_DATOS1 3

#define PUERTO_DATO_DATOS2 C
#define PIN_DATO_DATOS2 2

#define PUERTO_DATO_HORA C
#define PIN_DATO_HORA 0


#define CLK_DATO_PORT PORT(PUERTO_CLK_DATOS)
#define CLK_HORA_PORT PORT(PUERTO_CLK_HORA)
#define DATO1_PORT PORT(PUERTO_DATO_DATOS1)
#define DATO2_PORT PORT(PUERTO_DATO_DATOS2)
#define HORA_PORT PORT(PUERTO_DATO_HORA)

#define CLK_DATO_PIN PIN(PUERTO_CLK_DATOS)
#define CLK_HORA_PIN PIN(PUERTO_CLK_HORA)
#define DATO1_PIN PIN(PUERTO_DATO_DATOS1)
#define DATO2_PIN PIN(PUERTO_DATO_DATOS2)
#define HORA_PIN PIN(PUERTO_DATO_HORA)

#define CLK_DATO_DDR DDR(PUERTO_CLK_DATOS)
#define CLK_HORA_DDR DDR(PUERTO_CLK_HORA)
#define DATO1_DDR DDR(PUERTO_DATO_DATOS1)
#define DATO2_DDR DDR(PUERTO_DATO_DATOS2)
#define HORA_DDR DDR(PUERTO_DATO_HORA)

#endif /* _DEFINES_H */
