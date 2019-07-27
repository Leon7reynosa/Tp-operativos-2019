/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */
#include "Lissandra.h"

void liberar_memtable(void){

	dictionary_destroy_and_destroy_elements(memtable, eliminar_dato_t);

}

void inicializar_memtable() {
    memtable = dictionary_create();
    pthread_rwlock_init(&(lock_memtable), NULL);
}

//tenemos que hacer que la memtable siempre este inicializada.
void ingresar_a_memtable(dato_t *dato_a_ingresar, char *nombre_tabla) {

    t_list *lista_tabla;

    pthread_rwlock_wrlock(&(lock_memtable));

    if (!dictionary_has_key(memtable, nombre_tabla)) {

        lista_tabla = list_create();
        dictionary_put(memtable, nombre_tabla, lista_tabla);
        list_add(lista_tabla, dato_a_ingresar);

    }else{

    lista_tabla = dictionary_get(memtable, nombre_tabla);
    list_add(lista_tabla, dato_a_ingresar);

    }

    pthread_rwlock_unlock(&(lock_memtable));
    log_info(logger_lissandra, "Ingresamos un dato a la tabla %s\n", nombre_tabla);

}



t_list *obtener_tabla(char *nombre_tabla) {
    t_list *lista_tabla = (t_list *) dictionary_get(memtable, nombre_tabla);
    return lista_tabla; //DEVUELVE NULL SI LA TABLA NO EXISTE
}



dato_t *obtener_dato_con_mayor_timestamp_tabla(char *nombre_tabla, u_int16_t key) {

	pthread_rwlock_rdlock(&(lock_memtable));

	printf("[BUSQUEDA] BUSCO EN MEMTABLE\n");

    t_list *tabla_a_filtrar = obtener_tabla(nombre_tabla);
    t_list *tabla_ordenada;
    dato_t* dato_mayor;

    if (tabla_a_filtrar == NULL) {
    	pthread_rwlock_unlock(&(lock_memtable));
    	printf("[BUSQUEDA] Termine de buscar en memtable\n");
        return NULL;
    }

    bool condicion(void *dato) {
        dato_t *dato_analizar = (dato_t *) dato;

        return dato_analizar->key == key;
    }

    tabla_a_filtrar = list_filter(tabla_a_filtrar, condicion);


    bool comparador(void *dato1, void *dato2) {
        dato_t *dato1_analizar = (dato_t *) dato1;
        dato_t *dato2_analizar = (dato_t *) dato2;

        return dato1_analizar->timestamp > dato2_analizar->timestamp;
    }

    tabla_ordenada = list_sorted(tabla_a_filtrar, comparador);

//    list_destroy(tabla_a_filtrar);

    dato_mayor = (dato_t *)list_get(tabla_ordenada, 0);

    if(dato_mayor != NULL){

    	printf("[RESULTADO] Se encontro el dato en memtable\n");

    	dato_t* dato_encontrado = crear_dato(dato_mayor->key, dato_mayor->value, dato_mayor->timestamp);

    	pthread_rwlock_unlock(&(lock_memtable));

    	list_destroy(tabla_ordenada);

    	return dato_encontrado;                // RETURN

    }else{
    	printf("[RESULTADO] No se encontro el dato en memtable\n");
    	pthread_rwlock_unlock(&(lock_memtable));
    }

    list_destroy(tabla_ordenada);

    printf("[BUSQUEDA] Termine de buscar en memtable\n");

    return dato_mayor;                        //RETURN
}


void eliminar_dato_t(dato_t* dato ){

	free(dato->value);
	free(dato);

}




