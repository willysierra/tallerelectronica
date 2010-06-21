/**
 * -------------------------------------------------------------------
 * Copyright (c) 2010 - Todos los derechos reservados
 * Manuel Eduardo Vieda Salomon <me.vieda372@gmail.com
 * Ingeniero Electronico, Ingeniero de Sistemas y Computacion
 * Universidad de los Andes. Bogota, Colombia.
 * -------------------------------------------------------------------
 *
 * Archivo:				USART.h
 * Fecha Creacion:		Mayo  15 de 2010
 * Fecha Modificacion:	Mayo  18 de 2010
 * Version (Revision):	0.1 (2)
 *
 * Descripcion:	Libreria que facilita el uso del modulo de comunicación
 *              asincrona y asincrona de los microcontroladores ATmega
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


#ifndef _USART_H_
#define _USART_H_

//-------------------------------------------------------------------------------
//  PARAMAETROS DE CONFIGURACION QUE PUEDEN SER MODIFICADOS POR EL USUARIO
//-------------------------------------------------------------------------------

/**
 *  Tasa de transmision en Baudios
 */
#define BAUDRATE 9600

/**
 * ASY_SYN=1  Selecciona el modo de operacion asincrono de la USART
 * ASY_SYN=0  Selecciona el modo de operacion sincrono de la USART
 */
#define ASY_SYN 1


/**
 * INT_TX_COMPLETA=1  Se genera una interrupcion(Si estan habilitadas globalmente) cuando
 *                    se completa la transmisión de lo que se encuentra en el registro de salida
 * ITN_TX_COMPLETA=0  No se genera la interrupcion
 */
#define INT_TX_COMPLETA 1

/**
 * INT_RX_COMPLETA=1  Se genera una interrupcion(Si estan habilitadas globalmente) cuando
 *                    se completa la recepcion de un dato en la USART
 * INT_RX_COMPLETA=0  No se genera la interrupcion
 */
#define INT_RX_COMPLETA 1

/**
 * SPEED_2X=1  Se habilita el doble de velocidad de transmision si se esta trabajanod en modo
 *             asincrono. Tener en cuenta que se realizaran menos muestro, por lo que solo debe
 *             activarse con un BAUDRATE adecuado y en medios de bajo ruido.
 * SPEED_2X=0  Velocidad de transmision normal
 */
#define SPEED_2X 0

/**
 * CONTROL_PARIEDAD=0  No se realiza ningun tipo de control de pariedad en la informacion transmitida
 * CONTROL_PARIEDAD=1  Se genera automaticamente un bit de pariedad PAR al enviar un dato y el
 *                     receptor se encarga de comprobarlo. Esta dato se revisa manualmente.
 * CONTROL_PARIEDAD=2  Se genera automaticamente un bit de pariedad IMPAR al enviar un dato y el
 *                     receptor se encarga de comprobarlo. Esta dato se revisa manualmente.
 */
#define CONTROL_PARIEDAD 0

/**
 * Se define el numero de bits de STOP que se envia en cada comunicacion. Puede tomar el valor de 1 o 2
 */
#define NUM_STOP 1

/**
 * Se define el tamañao en bits que conforma cada caracter enviado por la USART en cada una de las 
 * transimisiones. Este valor puede tomar valores de 5,6,7,8 y 9 bits. 
 */
#define CHAR_SIZE 8

/**
 * Se define la polaridad del reloj (Unicamente cuando se trabaja en modo sincrono). 
 * CLK_POL=0 Salida de Tx en flanco de subida, Rx en flanco de bajada
 * CLK_POL=1 Salida de Tx en flanco de bajada, Rx en flanco de subida
 */
#define CLK_POL 0

//-------------------------------------------------------------------------------
//  FUNCIONES OFRECIDAS QUE PUEDEN SER LLAMADAS POR EL USUARIO
//-------------------------------------------------------------------------------

/**
 * Se define el valor del registro que controla la tasa de transimision en baudios
 */
#define UBRRVAL ((F_CPU/(BAUDRATE*16UL))-1)


/**
 * USAR_init
 * Este metodo se encarga de realizar la unicializacion del modulo USART que entra por
 * parametro y en el modo selecccionado (Sincrono o asincrono), configurando la velocidad
 * y demás parámetros con la definicion de las constantes en el archivo de definiciones
 *
 * @PARAMS: Ninguno
 * @PRE:    Los parametros se encuentran bien definidos
 * @POST:   Se inicializa el puerto USART seleccionado en el modo seleccionado, quedando
 *          listo para enviar y recibir datos.
 * @RETURN: Void
 */
void USART0_init();


/**
 * USART0_EnableTx
 * @PARAMS: Ninguno
 * @PRE:    El puerto USART0 se encuentra inicializado
 * @POST:   El puerto USART0 se habilita para transmision 
 * @RETURN: Void
 */
void USART0_EnableTx();

/**
 * USART0_DisableTx
 * @PARAMS: Ninguno
 * @PRE:    El puerto USART0 se encuentra inicializado
 * @POST:   El puerto USART0 se deshabilita para transmision 
 * @RETURN: Void
 */
void USART0_DisableTx();

/**
 * USART0_EnableRx
 * @PARAMS: Ninguno
 * @PRE:    El puerto USART0 se encuentra inicializado
 * @POST:   El puerto USART0 se habilita para Recepsion 
 * @RETURN: Void
 */
void USART0_EnableRx();


/**
 * USART0_DisableRx
 * @PARAMS: Ninguno
 * @PRE:    El puerto USART0 se encuentra inicializado
 * @POST:   El puerto USART0 se deshabilita para Recepcion 
 * @RETURN: Void
 */
 void USART0_DisableRx();


 /**
  * USART0_Enviar
  * @PARAMS: uint8_t dato -> Dato que se desea enviar
  * @PRE:    El puerto USART0 se encuentra configurado y habilitado para el envio de mensajes
  * @POST:   Se coloca el mensaje en el buffer de salida de la USART0
  * @RETURN: Void
  */
void USART0_Enviar(uint8_t dato);


/**
 * USART0_Recibir
 * @PARAMS: Ninguno
 * @PRE:    El puerto USART0 se encuentra configurado y habilitado para la recepcion de mensajes
 * @POST:   Se retorno el mensaje que se almaceno en el buffer de entrada de la USART0
 * @RETURN: uint8_t dato -> El dato recibido en la transmision
 */
uint8_t USART0_Recibir();

void USART0_EnviarStrLen(int len, char *buf);

#define USART0_EnviarStr(str) USART0_EnviarStrLen(sizeof(str)-1, str)
#define USART0_EnviarLn(str) USART0_EnviarStrLen(sizeof(str)-1, str); USART0_Enviar(0x0D)





#endif /* _USART_H */
