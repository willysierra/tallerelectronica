/**
 * -------------------------------------------------------------------
 * Copyright (c) 2010 - Todos los derechos reservados
 * Manuel Eduardo Vieda Salomon <me.vieda372@gmail.com
 * Ingeniero Electronico, Ingeniero de Sistemas y Computacion
 * Universidad de los Andes. Bogota, Colombia.
 * -------------------------------------------------------------------
 *
 * Archivo:				DS1307.c
 * Fecha Creacion:		Junio 01 de 2010
 * Fecha Modificacion:	Junio 01 de 2010
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



 #include "defines.h"
 #include <stdint.h>
 #include <avr/io.h>
 #include <avr/pgmspace.h>
 #include "I2C.h"
 #include "DS1307.h"
 #include "USART.h"
 #include <util/delay.h>		// Convenience functions for busy-wait delay loops



uint8_t hora[10];	// XXpm:XX:XX
uint8_t fecha[10];	// dd/mm/aaaa;


void DS1307_Init(void){

	

	uint8_t DS1307_data[8];
	int error;
	USART0_EnviarLn("  - Comienza leer");
	error = I2C_LeerBytes(DS1307_ID, DS1307_ADDR, 0x00, 8, DS1307_data);

	if(error == -1){
		USART0_EnviarLn("ERROR!!");
		}
	else{
		USART0_EnviarLn("Leer OK");
		}


//	USART0_EnviarLn("CODIGO ERROR: ");USART0_EnviarLn(error);

	
	USART0_EnviarLn("Datos del Reloj:");
	USART0_EnviarLn(DS1307_data[0]);
	USART0_EnviarLn(DS1307_data[1]);
	USART0_EnviarLn(DS1307_data[2]);
	USART0_EnviarLn(DS1307_data[3]);
	USART0_EnviarLn(DS1307_data[4]);
	USART0_EnviarLn(DS1307_data[5]);
	USART0_EnviarLn(DS1307_data[6]);
	USART0_EnviarLn(DS1307_data[7]);
	USART0_EnviarLn("Fin Datos Reloj");

	uint8_t DS1307_data2[8];

	DS1307_data2[0] = 0x00;
	DS1307_data2[1] = 0x00;
	DS1307_data2[2] = 0x00;
	DS1307_data2[3] = 0x00;
	DS1307_data2[4] = 0x00;
	DS1307_data2[5] = 0x00;
	DS1307_data2[6] = 0x00;
	DS1307_data2[7] = 0x00;


	error = I2C_EscribirBytes(DS1307_ID, DS1307_ADDR, 0x00, 1, DS1307_data2);




	while(1){
			_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);
			_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);_delay_ms(100);



			error = I2C_LeerBytes(DS1307_ID, DS1307_ADDR, 0x00, 8, DS1307_data);
			USART0_Enviar(DS1307_data[1]);USART0_Enviar(':');USART0_Enviar(DS1307_data[0]);USART0_Enviar(0x0D);

	}



}
