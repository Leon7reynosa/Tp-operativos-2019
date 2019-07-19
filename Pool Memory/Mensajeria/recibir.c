/*
 * recibir.c
 *
 *  Created on: 19 abr. 2019
 *      Author: utnso
 */

#include"recibir.h"


request recibir_request(int conexion){

	//ACA EN VEZ DE QUE SEA CON MEMORIA_DTO NO SERIA MEJOR USAR SEEDS?

	cod_operacion* cod_op = malloc(sizeof(cod_operacion));
	void* tipo_request;
	int pene = 0;
	pene = recv(conexion, cod_op,sizeof(cod_operacion),MSG_WAITALL);

	if(pene == -1){
			perror("Fallo al recibir el codigo de operacion.");
		}

	if(pene == 0){
		printf("Se re desconecto el cliente pa!\n\n");
		return crear_request(DESCONEXION, NULL);
	}

	printf("codigo_op : %d\n" , *cod_op);
	//printf("bytes: %d\n",pene);

	switch(*cod_op){

		case SELECT:

			tipo_request = decodificar_select(conexion);
			break;

		case INSERT:

			tipo_request = decodificar_insert(conexion);
			break;

		case CREATE:

			printf("Decodifico el create\n");
			tipo_request = decodificar_create(conexion);
			break;

		case DESCRIBE:

			tipo_request = decodificar_describe(conexion);
			break;

		case DROP:

			tipo_request = decodificar_drop(conexion);

			break;

		case JOURNAL:

			break;

		case GOSSIP:

			tipo_request = decodificar_gossiping(conexion);

			break;

		default:

			break;

	}

	printf("Creo la request\n");
	request request = crear_request(*cod_op, tipo_request);

	free(cod_op);
	printf("Request creada\n");
	return request;

}


////////////////////7 FUNCIONES VIEJAS /////////////////////////7

void* recibir_buffer(int* size, int conexion){

	void* buffer;

	if(recv(conexion,size,sizeof(int),0) == -1){
		perror("Fallo al recibir el tamanio.");
	}

	buffer = malloc(*size);

	if(recv(conexion,buffer,*size,0) == -1){
		perror("Fallo al recibir el mensaje");
	}

	return buffer;

}
/*
cod_op determinar_operacion(char* buffer){

	int size = strlen(buffer);

	char* aux = malloc(size+1);
	memcpy(aux,buffer,size);
	aux[size + 1] = '\0';
	memcpy(aux,buffer,size);

	int i;

	for(i = 0; i<size; i++){
		aux [i] = toupper(aux[i]);
	}

	switch(strcmp(aux,"EXIT")){

		case 0:
			free(aux);
			return DESCONEXION;
			break;
		default:
			free(aux);
			return MENSAJE;
	}

}

void desconectar_cliente(int conexion){

	close(conexion);
	printf("Cliente %d Desconectado\n", conexion);

}


void recibir_mensaje(int conexion){

	int size;
	char* buffer;

	buffer = recibir_buffer(&size,conexion);

	buffer[size] = '\0';

	switch(determinar_operacion(buffer)){

		case MENSAJE:
			printf("[Cliente %d] Mensaje : %s \n",conexion,buffer);
			break;
		case DESCONEXION:
			desconectar_cliente(conexion);
			break;
		default:
			printf("No deberia haber entrado aca por ahora\n\n");
			exit(1);
	}

	free(buffer);

}
*/
//////////////////////////////////////////////////////////////////////////////////////////


t_dato* recibir_dato_LFS(int conexion){

	t_dato* dato_recibido = malloc(sizeof(t_dato));

	dato_recibido->value = malloc(sizeof(t_stream));

	int bytes = recv(conexion,&(dato_recibido->timestamp),sizeof(time_t), 0);

	if(bytes == -1){
		perror("NO RECIBIO EL TIMESTAMP;");
	}

	printf("Timestamp: %i\n", dato_recibido->timestamp);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bytes = recv(conexion,&(dato_recibido->key),sizeof(u_int16_t), 0);

	if(bytes == -1){
			perror("NO RECIBIO LA KEY;");
	}

	printf("Key: %i\n", dato_recibido->key);

	///////////////////////////////////////////////////////////////////////////////////
	bytes = recv(conexion, &(dato_recibido->value->size),sizeof(int), 0);

	if(bytes == -1){
		perror("NO RECIBIO EL TAMANIO DEL VALUE;");
	}

	printf("Size: %i\n", dato_recibido->value->size);

	//////////////////////////////////////////////////////////////////////////////

	dato_recibido->value->buffer = malloc(dato_recibido->value->size);

	bytes = recv(conexion, dato_recibido->value->buffer, dato_recibido->value->size, 0);

	if(bytes == -1){
		perror("NO RECIBIO EL VALUE;");
	}

	///////////////////////////////////////////////////////////////////////////////////

	return dato_recibido;

}

t_list* recibir_describe(int conexion){

	int numero_tablas, error_recv;

	int size;
	char* tabla_recibida;
	char* consistencia_recibida;
	int particiones_recibidas, compactacion_recibida;

	error_recv = recv(conexion, &numero_tablas, sizeof(int), MSG_WAITALL);

	printf("LLEGO LA RESPUESTA\n");

	printf("Cant tablas: %i\n", numero_tablas);

	t_list* datos_metadata = list_create();

	if(error_recv == -1){
		perror("NO SE RECIBIO LA CANTIDAD DE TABLAS DESCRIBE");
	}

	printf("Recibo el/los datos metadata\n");
	for(int i = 0; i < numero_tablas; i++){

		Metadata metadata_recibida;

		error_recv = recv(conexion, &size, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL SIZE DE LA TABLA");
		}

		printf("size tabla: %i\n", size);

		tabla_recibida = malloc(size);

		error_recv = recv(conexion, tabla_recibida, size, 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO LA TABLA");
		}

		printf("tabla: %s\n", tabla_recibida);

		error_recv = recv(conexion, &size, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL SIZE DE LA CONSISTENCIA");
		}

		printf("size consistencia: %i\n", size);

		consistencia_recibida = malloc(size);

		error_recv = recv(conexion, consistencia_recibida, size, 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO LA CONSISTENCIA");
		}

		printf("consistencia: %s\n", consistencia_recibida);

		error_recv = recv(conexion, &particiones_recibidas, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL NUMERO DE PARTICIONES");
		}

		printf("particiones: %i\n", particiones_recibidas);

		error_recv = recv(conexion, &compactacion_recibida, sizeof(int), 0);

		if(error_recv == -1){
			perror("NO SE RECIBIO EL TIEMPO DE COMPACTACION");
		}

		printf("compactacion: %i\n", compactacion_recibida);

		printf("Creo la estructura metadata con los datos anteriores\n");
		metadata_recibida = crear_metadata(tabla_recibida, consistencia_recibida, particiones_recibidas, compactacion_recibida);

		printf("Lo agrego a la lista\n");
		list_add(datos_metadata, metadata_recibida);

		free(tabla_recibida);
		free(consistencia_recibida);

	}

	return datos_metadata;

}

tabla_gossip_dto recibir_datos_gossip(socket_seed){

	int cantidad_memorias;
	int bytes_recv;


	tabla_gossip_dto dato_recibido;

	bytes_recv = recv(socket_seed, &cantidad_memorias, sizeof(int), 0);

	if(bytes_recv <= 0){
		perror("Recibir cantidad de memorias");
	}

	dato_recibido = crear_dto_gossip(cantidad_memorias);

	for(int i = 0; i < cantidad_memorias; i++){

		int nro;
		int size_ip;
		char* ip;
		int puerto;

		bytes_recv = recv(socket_seed, &nro, sizeof(int), 0);

		if(bytes_recv <= 0){
			perror("Recibir numero de memorias");
		}

		bytes_recv = recv(socket_seed, &size_ip, sizeof(int), 0);

		if(bytes_recv <= 0){
			perror("Recibir numero de memorias");
		}

		ip = malloc(size_ip);

		bytes_recv = recv(socket_seed, ip, size_ip, 0);

		if(bytes_recv <= 0){
			perror("Recibir IP");
		}

		bytes_recv = recv(socket_seed, &puerto, sizeof(int), 0);

		if(bytes_recv <= 0){
			perror("Recibir puerto");
		}


		memoria_dto memoria_ = crear_memoria_dto(nro, ip, puerto);

		agregar_memoria_gossip_dto(dato_recibido, memoria_);

	}


	return dato_recibido;

}

