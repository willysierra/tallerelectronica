 /**
 * -------------------------------------------------------------------
 * Universidad de los Andes - Facultad de Ingenieria
 * Departamento de Ingenieria Electrica y Electronica
 * Proyecto Taller de Electrónica
 * -------------------------------------------------------------------
 *
 * Archivo:				defines.h
 * Fecha Creacion:		Mayo 10 de 2010
 * Fecha Modificacion:	Junio 11 de 2010
 * Version (Revision):	0.1 (2)
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
 * Definicion de parametros relacionados con el controlador del
 * teclado matricial 4x4
 * ---------------------------------------------------------------
 */

#define PUERTO_TECLADO B


#define KBR_PORT PORT(PUERTO_TECLADO)
#define KBR_PIN  PIN(PUERTO_TECLADO)
#define KBR_DDR  DDR(PUERTO_TECLADO)


/*
 * ---------------------------------------------------------------
 * Definicion de parametros relacionados con el controlador del
 * display LCD HD44780, JHD 204A  o similar
 * ---------------------------------------------------------------
 */

#define LCD_CMD_PORT A
#define LCD_DTA_PORT A

#define LCD_RS PORT1
#define LCD_RW PORT2
#define LCD_E  PORT3


/* Se debe garantizar que los pines de control se encuentren consecutivos y en 
   orden descendente cuando se trabaja con un bus de 4 bits.
   Ejemplo: Data4=Port2, Data5=Port3, Data6=Port4, Data7=Port5 */


#define LCD_D4 PORT4
#define LCD_D5 PORT5
#define LCD_D6 PORT6
#define LCD_D7 PORT7



/**
 * FUNCTION SET:			RS RW D7 D6 D5 D4 D3 D2 D1 D0
 * 							 0  0  0  0  1 DL  N  F  *  *
 *
 * DL: Interface Data Lenght [DL:0 -> 4 bits, DL:1 -> 8 bits]	
 * N : Number of display lines  [N:0 -> 1 Linea, N:1 -> 2 Lineas]
 * F : Character Font  [F:0 -> 5x8 puntos, F:1 -> 5x10 puntos]
 */
#define LCD_FNSET_DL 0
#define LCD_FNSET_N 1
#define LCD_FNSET_F 0

/**
 * ENTRY MODE SET:			RS RW D7 D6 D5 D4 D3 D2 D1 D0
 *							 0  0  0  0  0  0  0  1 ID  S
 *
 * ID: Increment or Decrement  [ID:0 -> Decrement, ID:1 -> Increment]
 * S : Display Shift [S:0 -> None, S:1 -> Accompanies Display Shift]
 */
#define LCD_ENTMOD_ID 1
#define LCD_ENTMOD_S  0

/**
 * DISPLAY CONTROL:			RS RW D7 D6 D5 D4 D3 D2 D1 D0
 * 							 0  0  0  0  0  0  1  D  C  B
 *
 * D: Display On/Off  [D:0 -> Off, D:1 -> On]
 * C: Cursor On/Off   [C:0 -> Off, D:1 -> On]
 * B: Blinking On/Off [B:0 -> Off, D:1 -> On]
 */
#define LCD_DSPCTR_D 1
#define LCD_DSPCTR_C 0
#define LCD_DSPCTR_B 0

extern unsigned char passwordROM[10];
extern unsigned char password[10];

extern unsigned char lecturaTeclado;
extern unsigned char teclaPresionada;
extern char nuevaTecla;

extern unsigned char mascaraFunciones;
extern unsigned char estadoMainProgram;


#endif /* _DEFINES_H */
