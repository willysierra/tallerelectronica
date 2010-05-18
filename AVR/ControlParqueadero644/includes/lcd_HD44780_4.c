/**
 * -------------------------------------------------------------------
 * Copyright (c) 2010 -Todos los derechos reservados
 * Manuel Eduardo Vieda Salomon <me.vieda372@gmail.com
 * Ingeniero Electronico, Ingeniero de Sistemas y Computacion
 * Universidad de los Andes. Bogota, Colombia.
 * -------------------------------------------------------------------
 *
 * Archivo:				lcd_HD44780_4.c
 * Fecha Creacion:		Julio 12 de 2009
 * Fecha Modificacion:	Mayo  17 de 2010
 * Version (Revision):	1.1 (2)
 *
 * Descripcion:	Driver para display LCD compatible con HD44780.
 *				Este controlador ofrece una completa conectividad bajo el
 *				modo de 4-bits de datos.
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
#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd_hd44780_4.h"

#define PEGAR(a, b)		a##b
#define PORT(x)			PEGAR(PORT, x)
#define PIN(x)			PEGAR(PIN, x)
#define DDR(x)			PEGAR(DDR, x)


#define LCD_CMD_PORT_OUT	PORT(LCD_CMD_PORT)
#define LCD_CMD_PORT_IN		PIN(LCD_CMD_PORT)
#define LCD_CMD_PORT_DDR	DDR(LCD_CMD_PORT)

#define LCD_DTA_PORT_OUT	PORT(LCD_DTA_PORT)
#define LCD_DTA_PORT_IN		PIN(LCD_DTA_PORT)
#define LCD_DTA_PORT_DDR	DDR(LCD_DTA_PORT)

#define LCD_DTA_BITS (_BV(LCD_D4)|_BV(LCD_D5)|_BV(LCD_D6)|_BV(LCD_D7))

#define LCD_BUSYFLAG 0x80


/**
 * LCD_pulsoEnable
 * Envia un pulso de la señal E (Enable) para idicar el comienzo de la lectura
 * de datos. Este metodo contempla en su codigo las restricciones de tiempo por
 * parte del controlador del display y la frecuencia de operacion del CPU
 *
 * @PARAMS: bool lectura -> Indica si se retorna la lectura de datos despues del pulso
 * @PRE: La senales RS y RW deben tener el valor correcto para la operacion
 * @POS: Se envia un pulso al pin E del LCD con una duracion de aprox 500ns
 * @Return: 
 */
static inline uint8_t LCD_pulsoEnable(bool lectura) __attribute__((always_inline));

static inline uint8_t LCD_pulsoEnable(bool lectura){
	
	uint8_t dato;
	
	LCD_CMD_PORT_OUT |= _BV(LCD_E);

	/* Se realiza una espera para garantizar tiempo de 500nS en PW_EH */
	#if F_CPU > 4000000UL
  	_delay_us(0.5);
	#else
		if(lectura)
		__asm__ volatile("nop");
		#if F_CPU > 1000000UL
		__asm__ volatile("nop");
		#if F_CPU > 2000000UL
		__asm__ volatile("nop");
		__asm__ volatile("nop");
		#endif /* F_CPU > 2000000UL */
		#endif /* F_CPU > 1000000UL */
	#endif /* F_CPU > 4000000UL */

	if(lectura)
		dato = LCD_DTA_PORT_IN & LCD_DTA_BITS;
	else
		dato = 0x00;

	LCD_CMD_PORT_OUT &= ~_BV(LCD_E);
	return dato;

}


/**
 * LCD_enviarNibble
 * Este metodo se encarga de enviar un nibble (4bits) al controlador del display.
 *
 * @PARAMS:	uint8_t n  -> nibble que se desea enviar
 *			uint8_t rs -> Selector de registro LCD (1:Instruccion, 0:Dato)
 * @PRE: n y rs se encuentran bien definidos
 * @POS: Se envian los datos al LCD
 * @Return: Void
 */
static void LCD_enviarNibble(uint8_t n, uint8_t rs){

	LCD_CMD_PORT_OUT &= ~_BV(LCD_RW);

	if (rs)
		LCD_CMD_PORT_OUT |= _BV(LCD_RS);
	else
		LCD_CMD_PORT_OUT &= ~_BV(LCD_RS);
	
	LCD_DTA_PORT_OUT &= ~LCD_DTA_BITS;
	LCD_DTA_PORT_OUT |= (n<<LCD_D4) & LCD_DTA_BITS;
	//LCD_DTA_PORT_OUT |= ((n<<LCD_D4)& LCD_DTA_BITS);
	(void)LCD_pulsoEnable(false);
}



/**
 * LCD_enviarByte
 * Este metodo se encarga de enviar un byte (8bits) al controlador del display.
 * Como se esta trabajando con un bus de datos de 4 bits deben enviarse dos
 * nibles consecutivos (Parte alta y parte baja del byte)
 *
 * @PARAMS:	uint8_t b  -> Byte que se desea enviar
 *			uint8_t rs -> Selector de registro LCD (1:Instruccion, 0:Dato)
 * @PRE: Los parametros se encuentran bien definidos
 * @POS: Se envian dos nibbles para completar el dato/instruccion en el controlador del LCD
 * @RETURN: Void
 */

void LCD_enviarByte(uint8_t by, uint8_t rs){

	LCD_enviarNibble(by>>4, rs);		// Enviamos la parte alta b[7..4]
	LCD_enviarNibble(by & 0x0F, rs);	// Enviamos la parte baja b[3..0]
}




/**
 * LCD_recibirNibble
 * Este metodo se encarga de recibir un nibble (4bits) del controlador del display.
 *
 * @PARAMS:	uint8_t rs -> Selector de registro LCD (1:Instruccion, 0:Dato)
 * @PRE: rs se encuentra bien definido
 * @POS: Se reciben los datos al LCD
 * @Return: Void
 */
static uint8_t LCD_recibirNibble(uint8_t rs){
	
	uint8_t dato;

	LCD_CMD_PORT_OUT |= _BV(LCD_RW);

	LCD_DTA_PORT_DDR &= ~LCD_DTA_BITS;

	if (rs)
		LCD_CMD_PORT_OUT |= _BV(LCD_RS);
	else
		LCD_CMD_PORT_OUT &= ~_BV(LCD_RS);
	
	dato = LCD_pulsoEnable(true);

	LCD_DTA_PORT_DDR |= LCD_DTA_BITS;
	LCD_CMD_PORT_OUT &= ~_BV(LCD_RW);

	return (dato & LCD_DTA_BITS)>>LCD_D4;


}



/**
 * LCD_recibirByte
 * Este metodo se encarga de recibir un byte (8bits) del controlador del display.
 * Como se esta trabajando con un bus de datos de 4 bits deben recibir dos
 * nibbles consecutivos (Parte alta y parte baja del byte)
 *
 * @PARAMS:	uint8_t rs -> Selector de registro LCD (1:Instruccion, 0:Dato)
 * @PRE: Los parametros se encuentran bien definidos
 * @POS: Se reciben dos nibbles para completar el dato/instruccion proveniente del controlador del LCD
 * @RETURN: Void
 */

uint8_t LCD_recibirByte(uint8_t rs){
	
	uint8_t dato;
	dato = LCD_recibirNibble(rs) << 4;
	dato |= LCD_recibirNibble(rs);
	return dato;

}


/**
 * Este metodo genera una espera activa mientras se encuentra activada la bandera de ocupado
 * del controlador del LCD.
 */
void LCD_esperarListo(void){
	while( LCD_recibirByte(0) & LCD_BUSYFLAG);
}


/**
 * LCD_init
 * Este metodo se encarga de realizar el protocolo de inicializacion del LCD compatible
 * con HD44789. El metodo se encarga de seguir y respetar los retardos indicados
 * por el fabricante para que este pueda reconocer el protocolo y las opciones de
 * configuracion que se escogen.
 *
 * @PARAMS: Void
 * @PRE: El voltaje ya se debe haber estabilizado tanto en el micro como en el LCD
 * @POS: Se inicializa el protocolo de comunicacion con el controlador del LCD con 
 *		 las opciones escogidas en el archivo 'defines.h'
 * @RETURN: Void
 */
void LCD_init(void){
	
	LCD_CMD_PORT_DDR |= _BV(LCD_RS)|_BV(LCD_RW)|_BV(LCD_E);
	LCD_DTA_PORT_DDR |= LCD_DTA_BITS;
	
	_delay_ms(20);

	// Se realiza una inicializacion por instrucciones para evitar problemas si
	// la fuente no es capaz de generar una inicializacion por reset automatico

	LCD_enviarNibble(0x03, 0);
	_delay_ms(5);
	LCD_enviarNibble(0x03, 0);
	_delay_ms(0.5);
	LCD_enviarNibble(0x03, 0);
	_delay_ms(0.5);


	LCD_enviarNibble( LCD_FNSET>>4, 0);
	LCD_esperarListo();
	LCD_enviarByte( LCD_FNSET, 0);
	LCD_esperarListo();
	LCD_enviarByte( LCD_DSPCTR, 0);
	LCD_esperarListo();
	LCD_enviarByte( LCD_ENTMOD, 0);
	LCD_esperarListo();
	LCD_enviarByte( 0x01, 0);
	LCD_esperarListo();
	LCD_enviarByte( 0x02, 0);
	LCD_esperarListo();

}
