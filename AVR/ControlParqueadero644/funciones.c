 /**
 * -------------------------------------------------------------------
 * Universidad de los Andes - Facultad de Ingenieria
 * Departamento de Ingenieria Electrica y Electronica
 * Proyecto Taller de Electrónica
 * -------------------------------------------------------------------
 *
 * Archivo:				funciones.c
 * Fecha Creacion:		Junio 10 de 2010
 * Fecha Modificacion:	Junio 14 de 2010
 * Version (Revision):	0.1 (2)
 *
 * Descripcion: Contiene la programacion de las diferentes funciones con las que cuenta
 *              el dispositivo desarrollado y que son encargadas por el micro ATmega644
 */


#include <stdio.h>			// Standard IO facilities	
#include <stdbool.h>		// Standar Boolean facilities
#include <stdint.h> 		// Standard Integer Types
#include <avr/io.h>			// AVR device-specific IO definitions
#include <util/delay.h>		// Convenience functions for busy-wait delay loops
#include <avr/interrupt.h>	// Interrupts handling
#include <avr/pgmspace.h>	// Program Space Utilities
#include <avr/eeprom.h>		// EEPROM Handling

#include "funciones.h"
#include "includes/lcd_HD44780_4.h"	// Manejo del Display LCD
#include "includes/USART.h"			// Manejo para comunicacion por USART
#include "includes/defines.h"


/**
 *
 */
void validarContrasena(void){

	static uint8_t estado;
	static uint8_t posCursor;
	static char passwordIngresado[10];
		
	switch(estado){
		case 0x00:
			
			LCD_borrarPantalla();
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("INGRESO SEGURO AREA");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("   ADMINISTRACION");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr(" Ingrese Contrasena");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("     __________");

			LCD_enviarCMD(0xD9);LCD_esperarListo();
			LCD_enviarCMD(0x0F);LCD_esperarListo();

			posCursor = 0;

			for(int i=0; i<10; i++)
				passwordIngresado[i]=0xFF;
			estado++;
			break;
		
		case 0x01:
			if(nuevaTecla){
				nuevaTecla=0;
				if((teclaPresionada>=0) & (teclaPresionada<=9)){
					// Se ingreso un numero, correspondiente a un digito de la contrasena
					if(posCursor<10){
						passwordIngresado[posCursor++] = teclaPresionada;
						LCD_enviarDTA_Wait('*');
					}
				}else if(teclaPresionada==11){
					// Se presiondo tecla numeral, correspondiente a la tecla OK -> Validar
					LCD_enviarCMD(0x0C);LCD_esperarListo();
					estado++;

				}else if(teclaPresionada==15){
					LCD_enviarCMD(0x0C);LCD_esperarListo();
					estadoMainProgram = 0x00;
					mascaraFunciones &= ~_BV(CONTRASENA);
					estado = 0x00;
				}
			}
		
			break;

		case 0x02: {
			estado++;
			// Se valida la contrasena ingresada
			uint8_t passwdOk = 1;
			for(int j=0; j<posCursor; j++)
				if(passwordIngresado[j]!=password[j])
					 passwdOk = 0;
			
			for(int j = posCursor; j<10; j++)
				if(password[j]!=0xFF)
					passwdOk = 0;
			
			if(passwdOk==0){
				// La constrasena no es correcta

				LCD_borrarPantalla();
				LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr(" ERROR - CONTRASENA");
				LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("     INCORRECTA    ");
				LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr(" Intentelo de nuevo");
				LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("     __________");

				LCD_enviarCMD(0xD9);LCD_esperarListo();
				LCD_enviarCMD(0x0F);LCD_esperarListo();

				posCursor = 0;

				for(int i=0; i<10; i++)
					passwordIngresado[i]=0xFF;
				
				estado = 0x01;
			}
			
			break;}

		case 0x03:
			LCD_borrarPantalla();
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("   ADMINISTRACION");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("1. Datos Parqueadero");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("2. Fecha y Hora");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("3. Configuraciones");
			estado++;
			break;
		
		case 0x04:
		if(nuevaTecla){
			nuevaTecla=0;
			switch(teclaPresionada){
				case 1:
					mascaraFunciones &= ~_BV(CONTRASENA);
					mascaraFunciones |= _BV(DATOS_PARQUEADERO);
					estado = 0x03;
					break;
				case 2:
					mascaraFunciones &= ~_BV(CONTRASENA);
					mascaraFunciones |= _BV(FECHA_HORA);
					estado = 0x03;
					break;
				case 3:
					mascaraFunciones &= ~_BV(CONTRASENA);
					mascaraFunciones |= _BV(CONFIGURACIONES);
					estado = 0x03;
					break;
				case 15:
					estadoMainProgram = 0x00;
					mascaraFunciones &= ~_BV(CONTRASENA);
					estado = 0x00;
				default:
					break;
			}

		}
		break;

		default:
			estado = 0x00;
	}

}




/**
 *
 */
void adminDatosParqueadero(void){
	static uint8_t estado;
	switch(estado){
		case 0x00:
			LCD_borrarPantalla();
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr(" DATOS PARQUEADERO");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("1. Cupos Totales");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("2. ");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("3. ");
			estado++;
			break;

		case 0x01:
			if(nuevaTecla){
				nuevaTecla=0;
				switch(teclaPresionada){
					case 1:
					case 2:
					case 3:
					case 15:
						mascaraFunciones &= ~_BV(DATOS_PARQUEADERO);
						mascaraFunciones |= _BV(CONTRASENA);
						estado = 0x00;
						break;
					default:
						break;
				}
			}
			break;

		default:
			estado=0x00;
			break;
	}

}



/**
 *
 */
void adminFechaHora(void){
	static uint8_t estado;
	switch(estado){
		case 0x00:
			LCD_borrarPantalla();
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("   FECHA Y HORA");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("1. Cambiar Fecha");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("2. Cambiar Hora");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("3. ");
			estado++;
			break;

		case 0x01:
			if(nuevaTecla){
				nuevaTecla=0;
				switch(teclaPresionada){
					case 1:
					case 2:
					case 3:
					case 15:
						mascaraFunciones &= ~_BV(FECHA_HORA);
						mascaraFunciones |= _BV(CONTRASENA);
						estado = 0x00;
						break;
					default:
						break;
				}
			}
			break;

		default:
			estado=0x00;
			break;
	}
}


/**
 *
 */
void adminConfiguraciones(void){
	static uint8_t estado;
	switch(estado){
		case 0x00:
			LCD_borrarPantalla();
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("  CONFIGURACIONES");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("1. Contrasena");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("2. Reset Fabrica");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("3. Autocomprobacion");
			estado++;
			break;

		case 0x01:
			if(nuevaTecla){
				nuevaTecla=0;
				switch(teclaPresionada){
					case 1:
					case 2:
					case 3:
					case 15:
						mascaraFunciones &= ~_BV(CONFIGURACIONES);
						mascaraFunciones |= _BV(CONTRASENA);
						estado = 0x00;
						break;
					default:
						break;
				}
			}
			break;

		default:
			estado=0x00;
			break;
	}
}



