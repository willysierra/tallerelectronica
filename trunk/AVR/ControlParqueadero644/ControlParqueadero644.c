/**
 * -------------------------------------------------------------------
 * Universidad de los Andes - Facultad de Ingenieria
 * Departamento de Ingenieria Electrica y Electronica
 * Proyecto Taller de Electrónica
 * -------------------------------------------------------------------
 *
 * Archivo:				ControlParqueadero644.c
 * Fecha Creacion:		Mayo 10 de 2010
 * Fecha Modificacion:	Junio 11 de 2010
 * Version (Revision):	0.1 (4)
 *
 * Descripcion:	Control principal proyecto Parqueadero Edificio SantoDomingo
 *
 */

/* Exportación de Librerias */

#include "includes/defines.h"

#include <stdio.h>			// Standard IO facilities	
#include <stdbool.h>		// Standar Boolean facilities
#include <stdint.h> 		// Standard Integer Types
#include <avr/io.h>			// AVR device-specific IO definitions
#include <util/delay.h>		// Convenience functions for busy-wait delay loops
#include <avr/interrupt.h>	// Interrupts handling
#include <avr/pgmspace.h>	// Program Space Utilities
#include <avr/eeprom.h>		// EEPROM Handling

#include "funciones.h"			// Contiene las rutinas de las diferentes funcionalidades (Req Funcionales)
#include "CuposParqueadero.h" 	// Contiene las rutinas del manejo de los cupos del parqueadero

#include "includes/lcd_HD44780_4.h"	// Manejo del Display LCD
#include "includes/USART.h"			// Manejo para comunicacion por USART
#include "includes/SPI.h"			// Manejo de comunicacion por el bus SPI
#include "includes/I2C.h"			// Manejo de comunicacion por el bus TWI
#include "includes/DS1307.h"		// Control del reloj de tiempo real RTC DS1307
#include "includes/SDCard.h"		// Control de tarjeta de almacenamiento MicroSD



//  ------------------------------------------------------------------------
//	VARIABLES GLOBALES DEL SISTEMA / BUFFERS
//  ------------------------------------------------------------------------

unsigned char mascaraFunciones;
unsigned char estadoMainProgram;

static uint8_t estadoTeclado;

unsigned char lecturaTeclado = 0x00;
unsigned char teclaPresionada = 0x00;
char nuevaTecla = 0;


unsigned char EEMEM passwordROM[10];
unsigned char password[10];




//  ------------------------------------------------------------------------
//	DECLARACION DE METODOS / PROCEDIMIENTOS INTERNOS
//  ------------------------------------------------------------------------
void initHardware(void);
void initPollingTimer(void);
void initVariables(void);

void vTeclado(void);
void vMainProgram(void);

/**
 * Rutina de atencion de interrupcion generada cuando la USART0 recibe un dato
 */
ISR(USART0_RX_vect) 
{
	uint8_t dato = USART0_Recibir();
	//LCD_enviarDTA(dato);LCD_esperarListo();
	USART0_Enviar(dato);
}


/**
 * Rutina de atencion de interrupcion generada cuando la USART0 completa la transmision de un dato
 */
ISR(USART0_TX_vect) 
{
	//LCD_enviarDTA(0xFF);LCD_esperarListo();
}


/**
 * Rutina de atencion de interrupcion generada cuando el dispositivo esta en comunicacion por el bus TWI/I2C
 */
ISR(TWI_vect)
{
	I2C_AtenderInterrupcion();
}



//  ------------------------------------------------------------------------
//	PROGRAMA PRINCIPAL DEL SISITEMA
//  ------------------------------------------------------------------------

int main(void) {

	_delay_ms(20);
	initHardware();
	initVariables();
	initPollingTimer();
	sei();	// Enable the Global Interrupt Enable flag so that interrupts can be processed 
	while(1){}

}


//  ------------------------------------------------------------------------
//	POLLING DEL SISTEMA OPERATIVO
//  ------------------------------------------------------------------------

/* Esta es la interrupción generada por Overflow del timer */
ISR(TIMER0_OVF_vect) {
	
	// Reiniciamos el contador de la interrupcion maestra
	vTeclado();
	vMainProgram();

	if(mascaraFunciones&_BV(CONTRASENA))
		validarContrasena();

	if(mascaraFunciones&_BV(DATOS_PARQUEADERO))
		adminDatosParqueadero();

	if(mascaraFunciones&_BV(FECHA_HORA))
		adminFechaHora();

	if(mascaraFunciones&_BV(CONFIGURACIONES))
		adminConfiguraciones();
}




/**
 * Metodo que se encarga de la inicializacion del hardware del proyecto. Dentro del 
 * hardware se encuentran tanto los periféricos externos como los internos del micro.
 * @PARAM:  Ninguno
 * @PRE:    Ninguno
 * @POST:   Se inicializa el hardware. Listo para uso rutinario
 * @RETURN: Void
 */
void initHardware(void){

	// Deshabilitamos Interrupciones Generales
	cli();

	// Inicializamos el LCD
	LCD_init();

	// Inicializamos el Teclado Matricial
	// Pines 0-3 Entrada (Resistencia a tierra)
	// Pines 4-7 Salida (Pulsador)
	KBR_DDR  = 0xF0; 
	KBR_PORT = 0x00;

	// Inicializamos Puerto Serial Sincrono (USART0) para comunicacion por modulo RF
	USART0_init();
	USART0_EnableTx();
	USART0_EnableRx();

	// Inicializamos el modulo I2C/TWI y el reloj de tiempo real (DS1307)
	I2C_Init();
	
	if(!(DS1307_Init()==0)){
			LCD_borrarPantalla();
			LCD_linea1Pos0();LCD_esperarListo();
			LCD_EnviarStr("PARQUEADERO EDIFICIO");
			LCD_EnviarStr("ERROR: No es posible");
			LCD_EnviarStr("    SANTODOMINGO    ");
			LCD_EnviarStr("comunicacion con RTC");
			while(1){}
	}



}


/**
 *
 */
void initVariables(void){	

	mascaraFunciones = 0x00;
	estadoMainProgram = 0x00;
	estadoTeclado = 0x00;

	for(int i=0; i<10; i++)
		password[i] = eeprom_read_byte((uint8_t*)&passwordROM[i]);

		
	totalCupos = eeprom_read_word((uint16_t*)&totalCuposROM);

	cuposOcupados = 0;
	cuposDisponibles = totalCupos;

}



/**
 *
 */
void initPollingTimer(void){

	// -------------------------------------------------
	// Configuración de TIMER0 para control de Polling
	// -------------------------------------------------

	// Tiempo de Ejecucipon de cada ciclo/in terrupcion: 0.51 mS

	TIMSK0 = _BV(TOIE0);	// Activamos interrupcion por Overflow del contador
	TCCR0B = _BV(CS02);
	TCNT0 = 0xC0;
}



/**
 * En esta tarea del sistema operativo se revisa el teclado para identificar el momento en que
 * se presiona una tecla y cual tecla es presionada. Solo hasta el momento en que la tecla es
 * liberada o deja de ser pulsada es atendida por el sistema (Se activa la bandera nuevaTecla
 * y se almacena en teclaPresionada para que sea procesada por otra tarea activa
 */
void vTeclado(void){

	switch(estadoTeclado){
		case 0:
			KBR_PORT = 0x10;KBR_PORT = 0x10;
			lecturaTeclado = (KBR_PIN&0x0F);
			switch(lecturaTeclado)
			{
				case 0x01: estadoTeclado=4; teclaPresionada=1;  break;
				case 0x02: estadoTeclado=4; teclaPresionada=2;  break;
				case 0x04: estadoTeclado=4; teclaPresionada=3;  break;
				case 0x08: estadoTeclado=4; teclaPresionada=12; break;
				default: estadoTeclado++; break;
			}
			break;

		case 1:
			KBR_PORT = 0x20;KBR_PORT = 0x20;
			lecturaTeclado = (KBR_PIN&0x0F);
			switch(lecturaTeclado)
			{
				case 0x01: estadoTeclado=4; teclaPresionada=4;  break;
				case 0x02: estadoTeclado=4; teclaPresionada=5;  break;
				case 0x04: estadoTeclado=4; teclaPresionada=6;  break;
				case 0x08: estadoTeclado=4; teclaPresionada=13; break;
				default: estadoTeclado++; break;
			}
			break;

		case 2:
			KBR_PORT = 0x40;KBR_PORT = 0x40;
			lecturaTeclado = (KBR_PIN&0x0F);
			switch(lecturaTeclado)
			{
				case 0x01: estadoTeclado=4; teclaPresionada=7;  break;
				case 0x02: estadoTeclado=4; teclaPresionada=8;  break;
				case 0x04: estadoTeclado=4; teclaPresionada=9;  break;
				case 0x08: estadoTeclado=4; teclaPresionada=14; break;
				default: estadoTeclado++; break;
			}
			break;

		case 3:
			KBR_PORT = 0x80;KBR_PORT = 0x80;
			lecturaTeclado = (KBR_PIN&0x0F);
			switch(lecturaTeclado)
			{
				case 0x01: estadoTeclado=4; teclaPresionada=10; break;
				case 0x02: estadoTeclado=4; teclaPresionada=0;  break;
				case 0x04: estadoTeclado=4; teclaPresionada=11; break;
				case 0x08: estadoTeclado=4; teclaPresionada=15; break;
				default: estadoTeclado=0; break;
			}
			break;

		case 4:
			// Se ha presionado una tecla, asi que esperamos a que esta sea liberada para efectuar accion
			KBR_PORT = 0xF0;
			lecturaTeclado = 0xFF;
			lecturaTeclado = (KBR_PIN&0x0F);

			if(lecturaTeclado==0x00){
				nuevaTecla = 0x01;
				estadoTeclado=0;
			}
			break;

		default:
			estadoTeclado=0;
			break;
	}
}



/**
 *
 */
void vMainProgram(void){

	static uint8_t i;
	
	switch(estadoMainProgram){
		case 0x00:
			// Este estado corresponde a la pantalla de inactividad (IdleScreen)

			// Esablecemos el contenido original
			LCD_borrarPantalla();
			LCD_linea1Pos0();LCD_esperarListo();
			LCD_EnviarStr("PARQUEADERO EDIFICIO");
			LCD_linea2Pos0();LCD_esperarListo();
			LCD_EnviarStr("    SANTODOMINGO    ");

			DS1307_ActualizarHora();
			DS1307_ActualizarFecha();
			LCD_linea3Pos0();LCD_esperarListo();
			LCD_EnviarStr(DS1307_fecha);
			LCD_EnviarStr("  ");
			LCD_EnviarStr(DS1307_hora);

			LCD_linea3Pos0();LCD_esperarListo();
			LCD_EnviarStr("Cupos Libres:000/000");

			estadoMainProgram++;
			break;

		case 0x01:
			
			i++;
			if(i>=30){
				DS1307_ActualizarHora();
				DS1307_ActualizarFecha();

				LCD_linea4Pos0();LCD_esperarListo();
				LCD_EnviarStr(DS1307_fecha);
				LCD_EnviarStr("  ");
				LCD_EnviarStr(DS1307_hora);

				LCD_linea3Pos0();LCD_esperarListo();
				LCD_EnviarStr("Cupos Libres:");
				CUPOS_EscribirInfoLCD();
				actualizarDisplayExterno();
				i=0;
			}
			estadoMainProgram++;
			break;

		case 0x02:
			estadoMainProgram--;
			if(nuevaTecla){
				nuevaTecla=0;
				switch(teclaPresionada){
					case 12: 
						estadoMainProgram = 0x03;
						mascaraFunciones |= _BV(CONTRASENA);
						break;
					default: break;
				}
			}
			break;
			
		case 0x03:
			break;

		default:
			estadoMainProgram = 0x00;
			break;
	}

}




