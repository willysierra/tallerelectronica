/**
 * -------------------------------------------------------------------
 * Copyright (c) 2010 -Todos los derechos reservados
 * Manuel Eduardo Vieda Salomon <me.vieda372@gmail.com
 * Ingeniero Electronico, Ingeniero de Sistemas y Computacion
 * Universidad de los Andes. Bogota, Colombia.
 * -------------------------------------------------------------------
 *
 * Archivo:				lcd_HD44780_4.h
 * Fecha Creacion:		Julio 12 de 2009
 * Fecha Modificacion:	Mayo  10 de 2010
 * Version (Revision):	1.0 (2)
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


#ifndef _LCD_HD44780_4_H_
#define _LCD_HD44780_4_H_


/*
 * Funcion que nos permite calcular el valor para el comando: Function Set
 */
#define LCD_FNSET (0x20 | ((LCD_FNSET_DL)? 0x10:0x00) | ((LCD_FNSET_N)? 0x08:0x00) | ((LCD_FNSET_F)? 0x04:0x00))

/*
 * Funcion que nos permite calcular el valor para el comando: Display Control
 */

#define LCD_DSPCTR (0x08 | (LCD_DSPCTR_D?0x04:0x00) | (LCD_DSPCTR_C?0x02:0x00) | (LCD_DSPCTR_B?0x01:0x00) ) 


/*
 * Funccion que nos permite calcular el valor para el comando: Entry Mode
 */
#define LCD_ENTMOD ( 0x04 | ((LCD_ENTMOD_ID)? 0x02:0x00) | ((LCD_ENTMOD_S)? 0x01:0x00))



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
void LCD_init(void);


/**
 * LCD_enviarByte
 * Este metodo se encarga de enviar un byte (8bits) al controlador del display.
 *
 * @PARAMS:	uint8_t b  -> Byte que se desea enviar
 *			uint8_t rs -> Selector de registro LCD (1:Instruccion, 0:Dato)
 * @PRE: Los parametros se encuentran bien definidos
 * @POS: Se envian dos nibbles para completar el dato/instruccion en el controlador del LCD
 * @RETURN: Void
 */
void LCD_enviarByte(uint8_t by, uint8_t rs);

/**
 * Este metodo genera una espera activa mientras se encuentra activada la bandera de ocupado
 * del controlador del LCD.
 */
void LCD_esperarListo(void);


/*
 * Enviar un comando al controlador del LCD (RS->0)
 */
#define LCD_enviarCMD(n) LCD_enviarByte((n) , 0)

/*
 * Enviar un dato al controlador del LCD (RS->1)
 */
#define LCD_enviarDTA(n) LCD_enviarByte((n), 1)


#define LCD_borrarPantalla() LCD_enviarCMD(0x01)
#define LCD_retornarOrigen() LCD_enviarCMD(0x02)
#define LCD_encender() LCD_enviarCMD((0x0C | (LCD_DSPCTR_C?0x02:0x00) | (LCD_DSPCTR_B?0x01:0x00) ))
#define LCD_apagar() LCD_enviarCMD((0x08 | (LCD_DSPCTR_C?0x02:0x00) | (LCD_DSPCTR_B?0x01:0x00) ))
#define LCD_linea1Pos0() LCD_enviarCMD(0x80)
#define LCD_linea3Pos0() LCD_enviarCMD(0xC0)
#define LCD_linea2Pos0() LCD_enviarCMD(0x94)
#define LCD_linea4Pos0() LCD_enviarCMD(0xD4)



#endif /* _LCD_HD44780_4_H */

