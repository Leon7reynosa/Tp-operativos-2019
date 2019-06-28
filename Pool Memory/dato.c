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

Dato decodificar_dato_de_memoria(void* dato_en_memoria){

	Dato dato;
	time_t timestamp;
	u_int16_t key;

	char* value = malloc(tamanio_value + 1);
	int desplazamiento = 0;

	memcpy(&timestamp , dato_en_memoria + desplazamiento, sizeof(time_t));
	desplazamiento += sizeof(time_t);

	memcpy(&key , dato_en_memoria + desplazamiento , sizeof(u_int16_t));
	desplazamiento += sizeof(u_int16_t);

	memcpy( value ,  dato_en_memoria + desplazamiento, tamanio_value);
	desplazamiento += tamanio_value;

	*(value + tamanio_value + 1) = '\0';

	dato = crear_dato(key, value, timestamp);

	free(value);

	return dato;
}

Dato crear_dato(u_int16_t key, char* value, time_t timestamp){

	struct DatoEstructura* dato = malloc(sizeof(struct DatoEstructura));

	int size = strlen(value) + 1;

	dato->timestamp = timestamp;
	dato->key 	 	= key;
	dato->value 	= malloc(size);
	memcpy(dato->value, value, size);

	return dato;
}

void liberar_dato(Dato dato_a_liberar){

	free(dato_a_liberar->value);
	free(dato_a_liberar);

}
