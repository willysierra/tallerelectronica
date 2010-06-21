 /**
 * -------------------------------------------------------------------
 * Universidad de los Andes - Facultad de Ingenieria
 * Departamento de Ingenieria Electrica y Electronica
 * Proyecto Taller de Electrónica
 * -------------------------------------------------------------------
 *
 * Archivo:				CuposParqueadero.c
 * Fecha Creacion:		Junio 15 de 2010
 * Fecha Modificacion:	Junio 21 de 2010
 * Version (Revision):	0.1 (2)
 *
 * Descripcion: Contiene la programacion de las rutinas asociadas al manejo
 *              de los cupos dentro del parqueadero
 */


#include <stdio.h>			// Standard IO facilities	
#include <stdbool.h>		// Standar Boolean facilities
#include <stdint.h> 		// Standard Integer Types
#include <avr/io.h>			// AVR device-specific IO definitions
#include <util/delay.h>		// Convenience functions for busy-wait delay loops
#include <avr/pgmspace.h>	// Program Space Utilities
#include <avr/eeprom.h>		// EEPROM Handling

#include "funciones.h"
#include "includes/defines.h"
#include "includes/lcd_HD44780_4.h"	// Manejo del Display LCD
#include "includes/USART.h"			// Manejo para comunicacion por USART


unsigned int EEMEM totalCuposROM;
unsigned int totalCupos;

unsigned int cuposOcupados;
unsigned int cuposDisponibles;

uint8_t totalCuposArr[3];
uint8_t cuposDisponiblesArr[3];



void actualizarInforArrays(){

	totalCuposArr[0] = totalCupos/100;
	totalCuposArr[1] = (totalCupos-totalCuposArr[0]*100)/10;
	totalCuposArr[2] = totalCupos-totalCuposArr[0]*100-totalCuposArr[1]*10;

	cuposDisponiblesArr[0] = cuposDisponibles/100;
	cuposDisponiblesArr[1] = (cuposDisponibles-cuposDisponiblesArr[0]*100)/10;
	cuposDisponiblesArr[2] = cuposDisponibles-cuposDisponiblesArr[0]*100-cuposDisponiblesArr[1]*10;
}

void CUPOS_EscribirInfoLCD(){

	actualizarInforArrays();
	
	
	LCD_enviarDTA_Wait(cuposDisponiblesArr[0]+0x30);
	LCD_enviarDTA_Wait(cuposDisponiblesArr[1]+0x30);
	LCD_enviarDTA_Wait(cuposDisponiblesArr[2]+0x30);
	LCD_enviarDTA_Wait('/');
	LCD_enviarDTA_Wait(totalCuposArr[0]+0x30);
	LCD_enviarDTA_Wait(totalCuposArr[1]+0x30);
	LCD_enviarDTA_Wait(totalCuposArr[2]+0x30);


}


void ingresarVehiculoEstudiante(void){
	if(cuposDisponibles>0){
		cuposOcupados++;
		cuposDisponibles = totalCupos-cuposOcupados;
	}
}


void retiroVehiculoEstudiante(void){

}


void cambiarTotalCupos(int cupos){
	totalCupos = cupos;
	eeprom_write_word((uint16_t*)&totalCuposROM, totalCupos);
	eeprom_busy_wait();
	cuposDisponibles =  totalCupos-cuposOcupados;
}



