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
#include "includes/DS1307.h"


unsigned int EEMEM totalCuposROM;
unsigned int totalCupos;

unsigned int cuposOcupados;
unsigned int cuposDisponibles;

uint8_t totalCuposArr[3];
uint8_t cuposDisponiblesArr[3];

uint8_t tiempoEspera[3]={0,0,0};

void calcularTiempoEspera();


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
		if(cuposDisponibles==0){
			calcularTiempoEspera();
		}
	}
}


void retiroVehiculoEstudiante(void){

	if(cuposOcupados>0){
	cuposOcupados--;
	cuposDisponibles = totalCupos-cuposOcupados;
		if(cuposDisponibles==1){
			calcularTiempoEspera();
		}

	}

}


void cambiarTotalCupos(int cupos){
	totalCupos = cupos;
	eeprom_write_word((uint16_t*)&totalCuposROM, totalCupos);
	eeprom_busy_wait();
	cuposDisponibles =  totalCupos-cuposOcupados;
}

void actualizarDisplayExterno(){

	actualizarInforArrays();
	
	USART0_Enviar('A');
	USART0_Enviar(cuposDisponiblesArr[0]);
	USART0_Enviar(cuposDisponiblesArr[1]);
	USART0_Enviar(cuposDisponiblesArr[2]);
	USART0_Enviar(tiempoEspera[0]);
	USART0_Enviar(tiempoEspera[1]);
	USART0_Enviar(tiempoEspera[2]);

	USART0_Enviar(DS1307_hora[0]-0x30);
	USART0_Enviar(DS1307_hora[1]-0x30);
	USART0_Enviar(DS1307_hora[3]-0x30);
	USART0_Enviar(DS1307_hora[4]-0x030);



}



void calcularTiempoEspera(){
if(cuposDisponibles==0){

	int cualHora = DS1307_hora[8]? 120+(DS1307_hora[0]-0x30)*100+ (DS1307_hora[1]-0x30)*10+ (DS1307_hora[3]-0x30) : (DS1307_hora[0]-0x30)*100+ (DS1307_hora[1]-0x30)*10+ (DS1307_hora[3]-0x30);

	if(cualHora<=70){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 0;
		tiempoEspera[2] = 1;
	}
	else if(cualHora<=75){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 0;
		tiempoEspera[2] = 3;
	}
	else if(cualHora<=83){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 1;
		tiempoEspera[2] = 5;
	}
	else if(cualHora<=93){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 2;
		tiempoEspera[2] = 5;
	}
	else if(cualHora<=100){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 2;
		tiempoEspera[2] = 5;
	}
	else if(cualHora<=110){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 1;
		tiempoEspera[2] = 5;
	}
	else if(cualHora<=130){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 1;
		tiempoEspera[2] = 0;
	}
	else if(cualHora<=143){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 0;
		tiempoEspera[2] = 8;
	}
	else if(cualHora<=163){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 0;
		tiempoEspera[2] = 5;
	}
	
	else if(cualHora<=173){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 1;
		tiempoEspera[2] = 5;
	}
	else if(cualHora<=203){
		tiempoEspera[0] = 0;
		tiempoEspera[1] = 2;
		tiempoEspera[2] = 5;
	}
	
	else {
		tiempoEspera[0] = 1;
		tiempoEspera[1] = 1;
		tiempoEspera[2] = 1;
	}
}else{
	tiempoEspera[0] = 0;
	tiempoEspera[1] = 0;
	tiempoEspera[2] = 0;
}

}
