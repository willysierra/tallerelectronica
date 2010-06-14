/**
 * -------------------------------------------------------------------
 * Copyright (c) 2010 - Todos los derechos reservados
 * Manuel Eduardo Vieda Salomon <me.vieda372@gmail.com
 * Ingeniero Electronico, Ingeniero de Sistemas y Computacion
 * Universidad de los Andes. Bogota, Colombia.
 * -------------------------------------------------------------------
 *
 * Archivo:				SPI.h
 * Fecha Creacion:		Mayo  28 de 2010
 * Fecha Modificacion:	Mayo  28 de 2010
 * Version (Revision):	0.1 (1)
 *
 * Descripcion:	Libreria que facilita el uso del modulo de comunicación
 *              SPI de los microcontroladores ATmega
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


#ifndef _SPI_H_
#define _SPI_H_

//-------------------------------------------------------------------------------
//  PARAMAETROS DE CONFIGURACION QUE PUEDEN SER MODIFICADOS POR EL USUARIO
//-------------------------------------------------------------------------------

/**
 * Definicion del puerto donde se encuentra el modulo SPI, y de los pines de cada
 * una de las señales que conforman el protocolo
 */
#define PUERTO_SPI B
#define SPI_SS   PORT1
#define SPI_MOSI PORT3
#define SPI_MISO PORT4
#define SPI_SCK  PORT5


/*
 * Selecciona si se inicializa como un maestro (SPI_MSTR=1) o como un esclavo (SPI_MSTR=0)
 */
#define SPI_MSTR 1


/* Interrupcion
 * SPI_INT = 1 Se genera una interrupcion (Si estan habilitadas globalmente) cuando 
 *             se complete un proceso de comunicacion SPI
 * SPI_INT = 0 No se genera interrupcion
 */
#define SPI_INT 0

/* Definicion de orden de datos
 * SPI_DATA_ORDER 1  Se envia primero el bit menos significativo de la palabra de datos
 *                   (LSB ...... MSB)
 * SPI_DATA_ORDER 0  Se envia primero el bit mas significativo de la palabra de datos
 *                   (MSB ...... LSB)
 */
#define SPI_DATA_ORDER 0


/* Polaridad de Reloj
 * SPI_CPOL 0  El reloj de comunicacion (SCK) permanece en un estado bajo cuando no hay una
 *             comunicacion en curso. Flanco de subida como entrada y flanco de bajada como salida.  
 * SPI_CPOL 1  El reloj de comunicacion (SCK) permanece en un estado alto cuando no hay una
 *             comunicacion en curso. Flanco de bajada como entrada y flanco de subida como salida.
 */
#define SPI_CPOL 0

/* Fase del Reloj
 * SPI_CHPA 0 Se realiza el muestreo de los datos en el flanco de entrada del reloj.
 * SPI_CHPA 1 Se realiza el muestreo de los datos en el flanco de salida del reloj.
 */
#define SPI_CHPA 0

/* Frecuencia del reloj SCK
 * Seleccion de la relacion entre la frecuencia del reloj SPI (SCK) y la frecuencia del reloj
 * oscilador del microcontrolador (Fosc).
 * Se tiene la forma Fosc/X, donde X puede ser 4,16,64,128
 */
#define SPI_SPR 64

/*
 * Define si se habilita (SPI_2X=1) o no (SPI_2X=0) el control de doble velocidad de funcionamiento
 * del modulo SPI, cambiando la relacion del reloj a 2*SPR
 * Nota: En modo esclavo solo se garantiza un buen funcionamiento a Fosc/4 o menor.
 */
#define SPI_2X 0


//-------------------------------------------------------------------------------
//  FUNCIONES OFRECIDAS QUE PUEDEN SER LLAMADAS POR EL USUARIO
//-------------------------------------------------------------------------------


 /**
 * SPI_init
 * Este metodo se encarga de realizar la inicializacion del modulo SPI como maestro o esclavo
 * del microcontrolador, teniendo en cuenta las opciones de configuracion que se tienen
 * en el archivo SPI.h
 *
 * @PARAMS: Ninguno
 * @PRE:    El voltaje ya se debe haber estabilizado tanto en el micro como en perifericos
 * @POS:    Se inicializa el modulo de comunicacion SPI del microcontrolador
 * @RETURN: Void
 */
void SPI_init(void);

 /*
 * SPI_Enviar
 * Este metodo envia un byte a traves del modulo SPI
 * @PARAMS: uint8_t dato -> Dato que se desea enviar
 * @PRE:    El modulo SPI se encuentra configurado y habilitado para el envio de mensajes
 * @POST:   Se coloca el mensaje en el buffer de salida
 * @RETURN: Void
 */
void SPI_Enviar(uint8_t dato);

/**
 * SPI_Recibir
 * Este metodo recibe un byte a traves del modulo 
 * @PARAMS: Ninguno
 * @PRE:    El modulo SPI se encuentra configurado y habilitado para la recepcion de mensajes
 * @POST:   Se retorno el mensaje que se almaceno en el buffer de entrada del SPI
 * @RETURN: uint8_t dato -> El dato recibido en la transmision
 */
uint8_t SPI_Recibir();


#endif /* _SPI_H */
