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
#include "CuposParqueadero.h"
#include "includes/defines.h"
#include "includes/lcd_HD44780_4.h"	// Manejo del Display LCD
#include "includes/USART.h"			// Manejo para comunicacion por USART
#include "includes/DS1307.h"		// Control del reloj de tiempo real RTC DS1307



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
						if(posCursor==10)
							LCD_enviarCMD(0x0C);LCD_esperarListo();
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
	static uint8_t posCursor;
	static uint8_t cuposIngresado[3];
	switch(estado){
		case 0:
			LCD_borrarPantalla();
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr(" DATOS PARQUEADERO");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("1. Cupos Totales");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("2. ");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("3. ");
			estado++;
			break;

		case 1:
			if(nuevaTecla){
				nuevaTecla=0;
				switch(teclaPresionada){
					case 1:
						estado=2;
						break;
					case 2:
					case 3:
					case 15:
						mascaraFunciones &= ~_BV(DATOS_PARQUEADERO);
						mascaraFunciones |= _BV(CONTRASENA);
						estado = 0;
						break;
					default:
						break;
				}
			}
			break;
		
		case 2:
			// Cambio de total de Cupos
			LCD_borrarPantalla();							// --------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr(" DATOS PARQUEADERO");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("Ingrese el numero de");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("cupos disponibles en");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("el parqueadero: ___");
			LCD_enviarCMD(0x0F); LCD_esperarListo();
			LCD_enviarCMD(0xD4+16); LCD_esperarListo();
			posCursor=0;
			cuposIngresado[0] = 0;
			cuposIngresado[1] = 0;
			cuposIngresado[2] = 0;
			estado++;
			break;
		
		case 3:
			if(nuevaTecla){
				nuevaTecla=0;
				if(posCursor<3){
					if(teclaPresionada<=9){
						cuposIngresado[posCursor++] = teclaPresionada;
						LCD_enviarDTA_Wait(teclaPresionada+0x30);
					}
					if(posCursor==3)
						LCD_enviarCMD(0x0C); LCD_esperarListo();
				}
				
				if((posCursor==3) && (teclaPresionada==11)){
					int cupos = cuposIngresado[2]+cuposIngresado[1]*10+cuposIngresado[0]*100;
					cambiarTotalCupos(cupos);
					estado++;
				}
			}
			break;

		case 4:
			// Cambio Exitoso 
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("   CAMBIO EXITOSO");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("  Presione OK para");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("     continuar");
			estado++;
			break;

		case 5:
			if(nuevaTecla){
				nuevaTecla = 0;
				if(teclaPresionada==11)
					estado = 0;
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
	static uint8_t posCursor;
	static uint8_t fechaIngresada[8];
	static uint8_t horaIngresada[5];
	switch(estado){
		case 0x00:
			LCD_borrarPantalla();
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("   FECHA Y HORA");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("1. Cambiar Fecha");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("2. Cambiar Hora");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("3. ");
			estado++;
			break;

		case 1:
			if(nuevaTecla){
				nuevaTecla=0;
				switch(teclaPresionada){
					case 1:
						estado = 0x02;
						break;
					case 2:
						estado = 0x09;
						break;
					case 3:
					case 15:
						mascaraFunciones &= ~_BV(FECHA_HORA);
						mascaraFunciones |= _BV(CONTRASENA);
						estado = 0;
						break;
					default:
						break;
				}
			}
			break;

		case 2:
			// Cambio de Fecha
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("INGRESE NUEVA FECHA");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("     Dia:   __");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("     Mes:   __");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("     Ano: 20__");

			LCD_enviarCMD(0xCC);LCD_esperarListo();
			LCD_enviarCMD(0x0F);LCD_esperarListo();

			posCursor = 0;
			estado++;
			break;
		
		case 3:
			// Ingresa los datos (dia, mes y anio)
			if(nuevaTecla){
				nuevaTecla=0;
				if(posCursor==0){
					if((teclaPresionada>=0) & (teclaPresionada<=3)){
						fechaIngresada[0] = teclaPresionada;
						LCD_enviarDTA_Wait(teclaPresionada+0x30);
						posCursor++;
					}
				}else if(posCursor==1){
					if((teclaPresionada>=0) & (teclaPresionada<=9)){
						fechaIngresada[1] = teclaPresionada;
						LCD_enviarDTA_Wait(teclaPresionada+0x30);
						posCursor++;
						LCD_enviarCMD(0xA0);LCD_esperarListo();
					}
				}else if(posCursor==2){
					if(teclaPresionada<=1){
						fechaIngresada[2] = teclaPresionada;
						LCD_enviarDTA_Wait(teclaPresionada+0x30);
						posCursor++;
					}
				}else if(posCursor==3){
					if((teclaPresionada>=0) & (teclaPresionada<=9)){
						fechaIngresada[3] = teclaPresionada;
						LCD_enviarDTA_Wait(teclaPresionada+0x30);
						posCursor++;
						LCD_enviarCMD(0xE0);LCD_esperarListo();
					}
				}else if(posCursor==4){
					if((teclaPresionada>=0) & (teclaPresionada<=9)){
						fechaIngresada[4] = teclaPresionada;
						LCD_enviarDTA_Wait(teclaPresionada+0x30);
						posCursor++;
					}
				}else if(posCursor==5){
					if((teclaPresionada>=0) & (teclaPresionada<=9)){
						fechaIngresada[5] = teclaPresionada;
						LCD_enviarDTA_Wait(teclaPresionada+0x30);
						LCD_enviarCMD(0x0C);LCD_esperarListo();
						estado++;
					}
				}

			}
			break;
		
		case 4:{
			// Actualizamos los registros del RTC
			uint8_t dia = (fechaIngresada[1]&0x0F)|((fechaIngresada[0]&0x0F)<<4);
			uint8_t mes = (fechaIngresada[3]&0x0F)|((fechaIngresada[2]&0x0F)<<4);
			uint8_t ano = (fechaIngresada[5]&0x0F)|((fechaIngresada[4]&0x0F)<<4);
			uint8_t bytesEscritos = DS1307_CambiarFecha(dia, mes, ano);
			estado = bytesEscritos==3? 7: 5;
			break;}

		case 5:
			// Hubo un problema cambiando la fecha
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("ERROR - CAMBIO FECHA");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("  Presione OK para");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("intentarlo de nuevo!");
			estado++;
			break;

		case 6:
			if(nuevaTecla){
				nuevaTecla = 0;
				if(teclaPresionada == 11){
					// Se presiono OK
					estado = 0x02;
				}else if(teclaPresionada == 15){
					mascaraFunciones &= ~_BV(FECHA_HORA);
					mascaraFunciones |= _BV(CONTRASENA);
					estado = 0x00;
				}
			}
			break;

		case 7:
			// Se cambia la fecha en el RTC
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("   CAMBIO EXITOSO");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("  Presione OK para");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("     continuar");
			estado++;
			break;

		case 8:
			if(nuevaTecla){
				nuevaTecla = 0;
				if(teclaPresionada==11)
					estado = 0;
			}
			break;

		
		case 9:

			// Cambio de Hora
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr(" INGRESE NUEVA HORA");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("Formato (1) 12Horas");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("        (2) 24Horas");
			estado++;
			posCursor = 0;
			break;
		
		case 10:
			if(nuevaTecla){
				if(teclaPresionada==1){
					LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("Formato (1) 12Hrs X");
					estado = 11;
				}else if(teclaPresionada==2){
					LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("        (2) 24Hrs X");
					LCD_enviarCMD(0x0F);LCD_esperarListo();
					estado = 17;
				}
				nuevaTecla=0;
			}
			break;

		case 11:
			// Se cambia la hora en foramto 12 horas:
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr(" INGRESE NUEVA HORA");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr(" en formato 12Horas");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("     __:__   A/P:_");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("       AM:1 PM: 2");
			LCD_enviarCMD(0x0F);LCD_esperarListo();
			LCD_enviarCMD(0x99);
			estado++;
			break;
		
		case 12:
			if(nuevaTecla){
				nuevaTecla = 0;
				switch(posCursor){
					case 0:
						if(teclaPresionada<=1){
							horaIngresada[0] = teclaPresionada;
							LCD_enviarDTA_Wait(teclaPresionada+0x30);
							posCursor++;
						}
						break;
					case 1:
						if((horaIngresada[0]==0) && (teclaPresionada<=9)){
							horaIngresada[1] = teclaPresionada;
							LCD_enviarDTA_Wait(teclaPresionada+0x30);
							LCD_enviarDTA_Wait(':');
							posCursor++;
						} else if((horaIngresada[0]==1) && (teclaPresionada<=2)){
							horaIngresada[1] = teclaPresionada;
							LCD_enviarDTA_Wait(teclaPresionada+0x30);
							LCD_enviarDTA_Wait(':');
							posCursor++;
						}
						break;
					case 2:
						if(teclaPresionada <=5){
							horaIngresada[2] = teclaPresionada;
							LCD_enviarDTA_Wait(teclaPresionada+0x30);
							posCursor++;
						}
						break;
					case 3:
						if(teclaPresionada <=9){
							horaIngresada[3] = teclaPresionada;
							LCD_enviarDTA_Wait(teclaPresionada+0x30);
							posCursor++;
							LCD_enviarCMD(0x94+17);
						}
						break;
					case 4:
						if(teclaPresionada==1)
							horaIngresada[4]=0;
						else if(teclaPresionada==2)
							horaIngresada[4]=1;
						
						uint8_t hrs = (horaIngresada[1]&0x0F)|((horaIngresada[0]&0x0F)<<4);
						uint8_t min = (horaIngresada[3]&0x0F)|((horaIngresada[2]&0x0F)<<4);
						uint8_t bytesEscritos = DS1307_CambiarHora(0x00, min, hrs, 0x01, horaIngresada[4]);
						estado = bytesEscritos==3? 13: 15;
						LCD_enviarCMD(0x0C);LCD_esperarListo();

						break;

					default:
						posCursor = 0;
				}
			}
			break;

		case 13:
			// Se cambio la fecha correctamente
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("   CAMBIO EXITOSO");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("  Presione OK para");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("     continuar");
			estado++;
			break;

		case 14:
			if(nuevaTecla){
				nuevaTecla = 0;
				if(teclaPresionada==11)
					estado = 0;
			}
			break;

		case 15:
			// Hubo un problema cambiando la hora
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("ERROR - CAMBIO HORA");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("  Presione OK para");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("intentarlo de nuevo!");
			estado++;
			break;

		case 16:
			if(nuevaTecla){
				nuevaTecla = 0;
				if(teclaPresionada == 11){
					// Se presiono OK
					estado = 0x09;
				}else if(teclaPresionada == 15){
					mascaraFunciones &= ~_BV(FECHA_HORA);
					mascaraFunciones |= _BV(CONTRASENA);
					estado = 0x00;
				}
			}
			break;

		case 17:
			// Se cambia la hora en formato 24 horas:
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr(" INGRESE NUEVA HORA");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr(" en formato 24horas");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("     __:__");
			LCD_enviarCMD(0x0F);LCD_esperarListo();
			LCD_enviarCMD(0x99);
			estado++;
			break;

		case 18:
			//
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
	static uint8_t posCursor;
	static uint8_t passIngresado[10];
	static uint8_t passIngresado2[10];
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
						estado = 2;
						break;
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

		case 2:
			// Cambiamos la contrasenia
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("CAMBIO DE CONTRASENA");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("Ingrese una contrana");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("de hasta 10caracters");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("     __________");
			LCD_enviarCMD(0xD4+5);LCD_esperarListo(); 
			LCD_enviarCMD(0x0C);
			estado++;
			posCursor = 0;
			for(int i=0; i<10; i++){
				passIngresado[i]=0xFF;
				passIngresado2[i]=0xFF;
			}
			LCD_esperarListo();
			break;
		
		case 3:
			if(nuevaTecla){
				nuevaTecla = 0;
				if(posCursor<=10){
					if(teclaPresionada<=9){
						passIngresado[posCursor++] = teclaPresionada;
						LCD_enviarDTA_Wait(teclaPresionada+0x30);
						if(posCursor==10){
							LCD_enviarCMD(0x0C);LCD_esperarListo();
						}
							
					}
				}
				if(teclaPresionada==11){
					
					LCD_borrarPantalla();							 //--------------------
					LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("CAMBIO DE CONTRASENA");
					LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("Repetir contrasena:");
					LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("     __________");
					LCD_enviarCMD(0xD4+5);LCD_esperarListo();
					posCursor = 0;
					estado++;
				}

			}
			break;

		
		case 4:
			// RepetirContrasena
			if(nuevaTecla){
				nuevaTecla = 0;
				if(posCursor<=10){
					if(teclaPresionada<=9){
						passIngresado2[posCursor++] = teclaPresionada;
						LCD_enviarDTA_Wait(teclaPresionada+0x30);
						if(posCursor==10){
							LCD_enviarCMD(0x0C);LCD_esperarListo();
						}
							
					}
				}
				if(teclaPresionada==11){
					int i =0;
					for( i=0; i<10; i++){
						if(passIngresado[i]!=passIngresado2[i])
							break;
					}
					estado = i==10? 7: 5;
					LCD_enviarCMD(0x0C);LCD_esperarListo();

				}

			}
			break;

		case 5:
			// Error repitiendo contrasena
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr(" ERROR - CAMBIO DE");
			LCD_linea2Pos0();LCD_esperarListo();LCD_EnviarStr("    CONTRASENA");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr(" Datos no Coinciden");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("   Presiones Ok");
			estado++;
			break;

		case 6:
			if(nuevaTecla){
				nuevaTecla = 0;
				if(teclaPresionada ==11)
					estado = 2;
				else if(teclaPresionada==15){
					mascaraFunciones &= ~_BV(CONFIGURACIONES);
					mascaraFunciones |= _BV(CONTRASENA);
					estado = 0x00;
				}
			}
			break;
		case 7:
			// Se presiona Ok
			for(int i=0; i<10; i++){
				eeprom_write_byte((uint8_t*)&passwordROM[i], passIngresado[i]);
				eeprom_busy_wait();
				password[i] = eeprom_read_byte((uint8_t*)&passwordROM[i]);
			}
			estado++;
			break;
				
		case 8:
			LCD_borrarPantalla();							 //--------------------
			LCD_linea1Pos0();LCD_esperarListo();LCD_EnviarStr("   CAMBIO EXITOSO");
			LCD_linea3Pos0();LCD_esperarListo();LCD_EnviarStr("  Presione OK para");
			LCD_linea4Pos0();LCD_esperarListo();LCD_EnviarStr("     continuar");
			estado++;
			break;

		case 9:
			if(nuevaTecla){
				nuevaTecla = 0;
				if(teclaPresionada==11)
					mascaraFunciones &= ~_BV(CONFIGURACIONES);
					mascaraFunciones |= _BV(CONTRASENA);
					estado = 0x00;
			}
			break;

		default:
			estado=0x00;
			break;
	}
}



