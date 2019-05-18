/*
 * API_Pool.h
 *
 *  Created on: 13 may. 2019
 *      Author: utnso
 */

#ifndef API_POOL_H_
#define API_POOL_H_

select(char* nombre_tabla, int key);
/*
 * 1)Verificar si existe el segmento de la tabla solicitada y buscar en las paginas
 * del mismo si contiene la key solicitada. Si la contiene, devuelve su valor y
 * finaliza el proceso
 * 2)Si no la contiene, enviar la solicitud a FileSystem para obtener el valor
 * solicitado y almacenarlo
 * 3)Una vez obtenido, se debe solicitar una nueva pagina libre para almacenar
 * el mismo. En caso de no disponer de una pagina libre, se debe ejecutar el
 * algoritmo de reemplazo y, en caso de no poder efectuarlo por estar la memoria
 * full, ejecutar el Journal de la memoria
 */

insert(char* nombre_tabla, int key, char* value);


#endif /* API_POOL_H_ */
