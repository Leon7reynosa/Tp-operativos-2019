#include "generadorConfig.h"


int main (void){

	generadorConfig();
}

void generadorConfig(){


	int opcion;

    //FILESYSTEM
    int puerto_escucha, retardo, tamanio_value, tiempo_dump;
    char* punto_montaje = string_new();
    //METADATA FILESYSTEM
    int block_size, blocks;
    char* magic_number = string_new();
    //MEMORIA
    int puerto_memoria, puerto_fileSystem;
    char* ip_seeds = string_new();
    char* puerto_seeds = string_new();
    int retardo_mem, retardo_fs, tam_mem, retardo_journal, retardo_gossiping, mem_num;
    //KERNEL
    char* ip_memoria = string_new();
    int quantum, multiprocesamiento, metadata_refresh, sleep_ejecucion, numero_memoria;


    printf("##### MENU #####\n");
    printf("Crear config para:\n");
    printf("1) FileSystem\n");
    printf("2) Metadata FileSystem\n");
    printf("3) Memoria\n");
    printf("4) Kernel\n");
    printf("5) EXIT\n");

    printf("Ingrese un numero = ");
    scanf("%i", &opcion);

while(opcion != 5){
    switch(opcion){
        case 1:
            printf("\nIngrese parametros [PUERTO ESCUCHA] [PUNTO_MONTAJE] [RETARDO] [TAMANIO VALUE] [TIEMPO DUMP]\n");
            scanf("%i %s %i %i %i", &puerto_escucha, punto_montaje, &retardo, &tamanio_value, &tiempo_dump);
            crear_config_fileSystem(puerto_escucha, punto_montaje, retardo, tamanio_value, tiempo_dump);
            break;
        case 2:
            printf("Ingrese parametros [BLOCK SIZE] [BLOCKS] [MAGIC NUMBER]\n");
            scanf("%i %i %s", &block_size, &blocks, magic_number);
            crear_config_metadata_fileSystem(block_size, blocks, magic_number);
            break;
        case 3:
            printf("Ingrese parametros [[IP SEEDS]] [PUERTO MEMORIA] [PUERTO FS] [[PUERTO SEEDS]] [RETARDO MEM] [RETARDO FS] [TAM MEM] [RETARDO JOURNAL] [RETARDO GOSSIPING] [MEMORY NUMBER]\n");
            scanf("%s %i %i %s %i %i %i %i %i %i", ip_seeds, &puerto_memoria, &puerto_fileSystem, puerto_seeds, &retardo_mem, &retardo_fs, &tam_mem, &retardo_journal, &retardo_gossiping, &mem_num);
            crear_config_memoria(puerto_memoria, puerto_fileSystem, ip_seeds, puerto_seeds, retardo_mem, retardo_fs, tam_mem, retardo_journal, retardo_gossiping, mem_num);
            break;
        case 4:
            printf("Ingrese parametros [IP MEMORIA] [PUERTO MEMORIA] [NUMERO MEMORIA] [QUANTUM] [MULTIPROCESAMIENTO] [METADATA REFRESH] [SLEEP EJECUCION] [PUNTO_MONTAJE]\n");
            scanf("%s %i %i %i %i %i", ip_memoria, &puerto_memoria, &numero_memoria, &quantum, &multiprocesamiento, &metadata_refresh, &sleep_ejecucion, punto_montaje);
            crear_config_kernel(ip_memoria, puerto_memoria, numero_memoria, quantum, multiprocesamiento, metadata_refresh, sleep_ejecucion, punto_montaje);
            break;
        case 5:
            exit(1);
        default:
            printf("No es opcion valida, elija otra !!!\n");
            break;
    }

    printf("##### MENU #####\n");
    printf("Crear config para:\n");
    printf("1) FileSystem\n");
    printf("2) Metadata FileSystem\n");
    printf("3) Memoria\n");
    printf("4) Kernel\n");
    printf("5) EXIT\n");

    printf("Ingrese un numero =");
    scanf("%i", &opcion);
    
}

}

void crear_config_fileSystem(int puerto_escucha, char* punto_montaje, int retardo, int tamanio_value, int tiempo_dump){
	char* string_puerto, *string_retardo, *string_tamanio_value, *string_tiempo_dump;

	string_puerto = string_itoa(puerto_escucha);
	string_retardo = string_itoa(retardo);
	string_tamanio_value = string_itoa(tamanio_value);
	string_tiempo_dump = string_itoa(tiempo_dump);

	//TP_OPERATIVOS/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/LFS/

	FILE* file = fopen("/home/utnso/Escritorio/fileSystem.config", "w");
    fclose(file);

    t_config* config_file_system = config_create("/home/utnso/Escritorio/fileSystem.config");
    config_set_value(config_file_system, "PUERTO_ESCUCHA", string_puerto);
    config_set_value(config_file_system, "PUNTO_MONTAJE", punto_montaje);
    config_set_value(config_file_system, "RETARDO", string_retardo);
    config_set_value(config_file_system, "TAMANIO_VALUE", string_tamanio_value);
    config_set_value(config_file_system, "TIEMPO_DUMP", string_tiempo_dump);

    config_save(config_file_system);
    config_destroy(config_file_system);

}

void crear_config_metadata_fileSystem(int block_size, int blocks, char* magic_number){
    char* string_block_size = string_new();
    char* string_blocks = string_new();
    
    //TP_OPERATIVOS/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/LFS/Metadata/

    FILE* file = fopen("/home/utnso/Escritorio/metadata.bin", "w");
    fclose(file);

    string_block_size = string_itoa(block_size);
    string_blocks = string_itoa(blocks);

    //TP_OPERATIVOS/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/LFS/Metadata/

    t_config* config_metadata_file_system = config_create("/home/utnso/Escritorio/metadata.bin");
    config_set_value(config_metadata_file_system, "BLOCK_SIZE", string_block_size);
    config_set_value(config_metadata_file_system, "BLOCKS", string_blocks);
    config_set_value(config_metadata_file_system, "MAGIC_NUMBER", magic_number);

    config_save(config_metadata_file_system);
    config_destroy(config_metadata_file_system);

}

void crear_config_memoria(int puerto_memoria, int puerto_fileSystem, char* ip_seeds, char* puerto_seeds, int retardo_mem, int retardo_fs, int tam_mem, int retardo_journal, int retardo_gossiping, int mem_num){
    char* string_puerto_memoria = string_new();
    char* string_puerto_fileSystem = string_new();
    char* string_retardo_mem = string_new();
    char* string_retardo_fs = string_new();
    char* string_tam_mem = string_new();
    char* string_retardo_journal = string_new();
    char* string_retardo_gossiping = string_new();
    char* string_mem_num = string_new();


    //TP_OPERATIVOS/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/Pool Memory/
    FILE* file = fopen("/home/utnso/Escritorio/pool.config", "w");
    fclose(file);

    string_puerto_memoria = string_itoa(puerto_memoria);
    string_puerto_fileSystem = string_itoa(puerto_fileSystem);
    string_retardo_mem = string_itoa(retardo_mem);
    string_retardo_fs = string_itoa(retardo_fs);
    string_tam_mem = string_itoa(tam_mem);
    string_retardo_journal = string_itoa(retardo_journal);
    string_retardo_gossiping = string_itoa(retardo_gossiping);
    string_mem_num = string_itoa(mem_num);

    t_config* config_pool = config_create("/home/utnso/Escritorio/pool.config");
    config_set_value(config_pool, "PUERTO_MEMORIA", string_puerto_memoria);
    config_set_value(config_pool, "PUERTO_FILESYSTEM", string_puerto_fileSystem);
    config_set_value(config_pool, "IP_SEEDS", ip_seeds);
    config_set_value(config_pool, "PUERTO_SEEDS", puerto_seeds);
    config_set_value(config_pool, "RETARDO_MEM", string_retardo_mem);
    config_set_value(config_pool, "RETARDO_FS", string_retardo_fs);
    config_set_value(config_pool, "TAM_MEM", string_tam_mem);
    config_set_value(config_pool, "RETARDO_JOURNAL", string_retardo_journal);
    config_set_value(config_pool, "RETARDO_GOSSIPING", string_retardo_gossiping);
    config_set_value(config_pool, "MEM_NUM", string_mem_num);

    config_save(config_pool);
    config_destroy(config_pool);
}


void crear_config_kernel(char* ip_memoria, int puerto_memoria, int numero_memoria, int quantum, int multiprocesamiento, int metadata_refresh, int sleep_ejecucion, char* punto_montaje){
    char* string_puerto_memoria = string_new();
    char* string_quantum = string_new();
    char* string_multiprocesamiento = string_new();
    char* string_metadata_refresh = string_new();
    char* string_sleep_ejecucion = string_new();
    char* string_numero_memoria = string_new();
    
    // TP_OPERATIVOS/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/Kernel/

    FILE* file = fopen("/home/utnso/Escritorio/kernel.config", "w");
    fclose(file);

    string_puerto_memoria = string_itoa(puerto_memoria);
    string_quantum = string_itoa(quantum);
    string_multiprocesamiento = string_itoa(multiprocesamiento);
    string_metadata_refresh = string_itoa(metadata_refresh);
    string_sleep_ejecucion = string_itoa(sleep_ejecucion);
    string_numero_memoria = string_itoa(numero_memoria);

    // TP_OPERATIVOS/tp-2019-1c-Te-Lo-Testeo-Asi-Nom-s/Kernel/

    t_config* config_kernel = config_create("/home/utnso/Escritorio/kernel.config");
    config_set_value(config_kernel, "IP_MEMORIA", ip_memoria);
    config_set_value(config_kernel, "PUERTO_MEMORIA", string_puerto_memoria);
    config_set_value(config_kernel, "NUMERO_MEMORIA", string_numero_memoria);
    config_set_value(config_kernel, "QUANTUM", string_quantum);
    config_set_value(config_kernel, "MULTIPROCESAMIENTO", string_multiprocesamiento);
    config_set_value(config_kernel, "METADATA_REFRESH", string_metadata_refresh);
    config_set_value(config_kernel, "SLEEP_EJECUCION", string_sleep_ejecucion);
    config_set_value(config_kernel, "PUNTO_MONTAJE", punto_montaje);


    config_save(config_kernel);
    config_destroy(config_kernel);
    

}

