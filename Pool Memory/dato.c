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

	int size = strlen(value) + 1;

	dato->key 	 	= key;
	dato->value 	= malloc(size);
	memcpy(dato->value, value, size);
	dato->timestamp = timestamp;

	printf("DATO CUANDO FUE CREADO: %s\n", dato->value);

	return dato;
}

void liberar_dato(Dato dato_a_liberar){

	free(dato_a_liberar->value);
	free(dato_a_liberar);

}
