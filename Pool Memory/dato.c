/*
 * dato.c
 *
 *  Created on: 4 jun. 2019
 *      Author: utnso
 */


#include"dato.h"
/*
struct DatoEstructura{

	u_int16_t key;
	char* value;
	time_t timestamp;

};
*/
Dato crear_dato(u_int16_t key, char* value, time_t timestamp){

	struct DatoEstructura* dato = malloc(sizeof(struct DatoEstructura));

	dato->key 	 	= key;
	dato->value 	= value;
	dato->timestamp = timestamp;

	return dato;
}

void liberar_dato(Dato dato_a_liberar){

	free(dato_a_liberar->value);
	free(dato_a_liberar);

}
