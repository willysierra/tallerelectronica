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



char DS1307_hora[10] = {' ', ' ', ':', ' ', ' ', ':', ' ', ' ', ' ', ' '};	// XX:XX:XXp
char DS1307_fecha[10] = {' ',' ',' ',' ',' ',' ','/',' ',' ',' ',};	// mmm dd/aa


uint8_t DS1307_Init(void){

	uint8_t DS1307_data[8];

	int bytesLeidos;
	bytesLeidos = I2C_LeerBytes(DS1307_ID, DS1307_ADDR, 0x00, 8, DS1307_data);

	// No fue posible leer del reloj
	if(bytesLeidos != 8)
		return 1;

	// Verificamos el bit de control CH (Oscilador)
	if((DS1307_data[0]&0x80))
	{
		
		// El oscilador no estaba encendido, asi que inicialiamos toda la configuracion por defecto
		DS1307_data[0] = 0x00;
		DS1307_data[1] = 0x00;
		DS1307_data[2] = (DS1307_12_24==1)?0x52:0x00;
		DS1307_data[3] = 0x06;	// Viernes
		DS1307_data[4] = 0x01;	// Dia = 01
		DS1307_data[5] = 0x01;	// Mes = Enero (01)
		DS1307_data[6] = 0x10;	// Ano = 2010 (10)
		DS1307_data[7] = 0x00;	// Sin salida

		bytesLeidos = I2C_EscribirBytes(DS1307_ID, DS1307_ADDR, 0x00, 8, DS1307_data);

		if(bytesLeidos!=8)
			return 1;
	}

	return 0;
}






void DS1307_ActualizarFecha(void){

	uint8_t data[3];
	int bytesLeidos;
	bytesLeidos = I2C_LeerBytes(DS1307_ID, DS1307_ADDR, 0x04, 3, data);

	if(bytesLeidos==3){

		DS1307_fecha[8] = ((data[2]&0x0F))+0x30;
		DS1307_fecha[7] = ((data[2]&0xF0)>>4)+0x30;
		DS1307_fecha[5] = ((data[0]&0x0F))+0x30;
		DS1307_fecha[4] = ((data[0]&0xF0)>>4)+0x30;
		switch(data[1]&0x1F){
			case 0x01:
				DS1307_fecha[0] = 'E';
				DS1307_fecha[1] = 'n';
				DS1307_fecha[2] = 'e';
				break;
			case 0x02:
				DS1307_fecha[0] = 'F';
				DS1307_fecha[1] = 'e';
				DS1307_fecha[2] = 'b';
				break;
			case 0x03:
				DS1307_fecha[0] = 'M';
				DS1307_fecha[1] = 'a';
				DS1307_fecha[2] = 'r';
				break;
			case 0x04:
				DS1307_fecha[0] = 'A';
				DS1307_fecha[1] = 'b';
				DS1307_fecha[2] = 'r';
				break;
			case 0x05:
				DS1307_fecha[0] = 'M';
				DS1307_fecha[1] = 'a';
				DS1307_fecha[2] = 'y';
				break;
			case 0x06:
				DS1307_fecha[0] = 'J';
				DS1307_fecha[1] = 'u';
				DS1307_fecha[2] = 'n';
				break;
			case 0x07:
				DS1307_fecha[0] = 'J';
				DS1307_fecha[1] = 'u';
				DS1307_fecha[2] = 'l';
				break;
			case 0x08:
				DS1307_fecha[0] = 'A';
				DS1307_fecha[1] = 'g';
				DS1307_fecha[2] = 'o';
				break;
			case 0x09:
				DS1307_fecha[0] = 'S';
				DS1307_fecha[1] = 'e';
				DS1307_fecha[2] = 'p';
				break;
			case 0x10:
				DS1307_fecha[0] = 'O';
				DS1307_fecha[1] = 'c';
				DS1307_fecha[2] = 't';
				break;
			case 0x11:
				DS1307_fecha[0] = 'N';
				DS1307_fecha[1] = 'o';
				DS1307_fecha[2] = 'v';
				break;
			case 0x12:
				DS1307_fecha[0] = 'D';
				DS1307_fecha[1] = 'i';
				DS1307_fecha[2] = 'c';
				break;

		}
	}

}


/**
 * Formato retornado (ss=segundos, mm=minutos, hh=horas, ap=AM o PM)
 * Si esta en formato 12 horas: ap hh mm ss
 * Si esta en formato 24 horas: 00 hh mm ss
 */
void DS1307_ActualizarHora(void){

	uint8_t data[3];
	int bytesLeidos;
	bytesLeidos = I2C_LeerBytes(DS1307_ID, DS1307_ADDR, 0x00, 3, data);

	if(bytesLeidos==3){

		DS1307_hora[7] = ((data[0]&0x0F))+0x30;
		DS1307_hora[6] = ((data[0]&0x70)>>4)+0x30;

		DS1307_hora[4] = ((data[1]&0x0F))+0x30;
		DS1307_hora[3] = ((data[1]&0x70)>>4)+0x30;


		
		DS1307_hora[1] = ((data[2]&0x0F))+0x30;

		if(data[2]&0x40){
			DS1307_hora[0] = ((data[2]&0x10)>>4)+0x30;
			DS1307_hora[8] = (data[2]&0x20)?'p':'a';
		}
		else
			DS1307_hora[0] = ((data[2]&0x30)>>4)+0x30;

		
		
	}

}


/**
 * Cambia la fecha en el RTC. 
 */
uint8_t DS1307_CambiarFecha(uint8_t dia, uint8_t mes, uint8_t ano){

	uint8_t data[3] = {dia, mes, ano};
	uint8_t bytesEscritos = 0;
	bytesEscritos = I2C_EscribirBytes(DS1307_ID, DS1307_ADDR, 0x04, 3, data);

	return bytesEscritos;
}

/**
 * Cabia la hora del RTC con los parametros ingresados
 */
uint8_t DS1307_CambiarHora(uint8_t seg, uint8_t min, uint8_t hrs, uint8_t formato, uint8_t ap){

	uint8_t data[3] = {seg, min, 0x00};
	data[2] = formato? (_BV(6)|(hrs&0x1F)|(ap?_BV(5):0)) : (hrs&0x3F); 
	uint8_t bytesEscritos = I2C_EscribirBytes(DS1307_ID, DS1307_ADDR, 0x00, 3, data);
	return bytesEscritos;
}
