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

			tipo_request = decodificar_create(conexion);
			break;

		case GOSSIP:

			tipo_request = decodificar_gossiping(conexion);

			break;

		default:

			break;

	}

	request request = crear_request(*cod_op, tipo_request);

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

t_dato* recibir_request_LFS(int conexion ){

	t_dato* dato_recibido = malloc(sizeof(t_dato));

	dato_recibido->value = malloc(sizeof(t_stream));

	int bytes = recv(conexion,&(dato_recibido->key),sizeof(int),MSG_WAITALL);

	printf("RECIBI %d bytes \n",bytes);
	printf("KEY %d\n",dato_recibido->key);

	if(bytes == -1){
		perror("NO RECIBIO LA KEY;");
	}

	///////////////////////////////////////////////////////////////////////////////////
	bytes = recv(conexion, &(dato_recibido->value->size),sizeof(int),MSG_WAITALL);

	printf("RECIBI %d bytes \n",bytes);
	printf("SIZE %d\n",dato_recibido->value->size);

	if(bytes == -1){
			perror("NO RECIBIO EL TAMANIO DEL VALUE;");
	}

	//////////////////////////////////////////////////////////////////////////////

	dato_recibido->value->buffer = malloc(dato_recibido->value->size);

	bytes = recv(conexion, dato_recibido->value->buffer, dato_recibido->value->size,MSG_WAITALL);

	printf("RECIBI %d bytes \n",bytes);

	char* value;

	value = dato_recibido->value->buffer;

	printf("VALUE %s\n",value);

	if(bytes == -1){
			perror("NO RECIBIO EL VALUE;");
	}

	///////////////////////////////////////////////////////////////////////////////////

	bytes = recv(conexion,&(dato_recibido->timestamp),sizeof(int),MSG_WAITALL);

	printf("RECIBI %d bytes \n",bytes);
	printf("TIMESTAMP %d\n\n",dato_recibido->timestamp);

	if(bytes == -1){
			perror("NO RECIBIO EL TAMANIO DEL VALUE;");
	}

	return dato_recibido ;

}



