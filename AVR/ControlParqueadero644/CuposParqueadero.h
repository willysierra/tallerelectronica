 /**
 * -------------------------------------------------------------------
 * Universidad de los Andes - Facultad de Ingenieria
 * Departamento de Ingenieria Electrica y Electronica
 * Proyecto Taller de Electrónica
 * -------------------------------------------------------------------
 *
 * Archivo:				CuposParqueadero.h
 * Fecha Creacion:		Junio 15 de 2010
 * Fecha Modificacion:	Junio 21 de 2010
 * Version (Revision):	0.1 (2)
 *
 * Descripcion: Contiene la programacion de las rutinas asociadas al manejo
 *              de los cupos dentro del parqueadero
 */


#ifndef _CUPOSPARQUEADERO_H_
#define _CUPOSPARQUEADERO_H_


extern unsigned int totalCuposROM;
extern unsigned int totalCupos;
extern unsigned int cuposOcupados;
extern unsigned int cuposDisponibles;

void cambiarTotalCupos(int cupos);
void ingresarVehiculoEstudiante(void);
void retiroVehiculoEstudiante(void);

void CUPOS_EscribirInfoLCD(void);

void actualizarDisplayExterno();


#endif /* _CUPOSPARQUEADERO_H_*/
