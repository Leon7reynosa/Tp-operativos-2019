/*
 * Lissandra.c
 *
 *  Created on: 8 abr. 2019
 *      Author: utnso
 */
#include "Lissandra.h"

void inicializar_memtable() {
    memtable = dictionary_create();
}

//tenemos que hacer que la memtable siempre este inicializada.
void ingresar_a_memtable(dato_t *dato_a_ingresar, char *nombre_tabla) {

    t_list *lista_tabla;

    if (!dictionary_has_key(memtable, nombre_tabla)) {

        lista_tabla = list_create();
        dictionary_put(memtable, nombre_tabla, lista_tabla);
        list_add(lista_tabla, dato_a_ingresar);

    }else{

    lista_tabla = dictionary_get(memtable, nombre_tabla);
    list_add(lista_tabla, dato_a_ingresar);

    }
}



t_list *obtener_tabla(char *nombre_tabla) {
    t_list *lista_tabla = (t_list *) dictionary_get(memtable, nombre_tabla);
    return lista_tabla; //DEVUELVE NULL SI LA TABLA NO EXISTE
}



dato_t *obtener_dato_con_mayor_timestamp_tabla(char *nombre_tabla, u_int16_t key) {
    t_list *tabla_a_filtrar = obtener_tabla(nombre_tabla);

    if (tabla_a_filtrar == NULL) {
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

    tabla_a_filtrar = list_sorted(tabla_a_filtrar, comparador);

    return (dato_t *) list_get(tabla_a_filtrar, 0);
}






