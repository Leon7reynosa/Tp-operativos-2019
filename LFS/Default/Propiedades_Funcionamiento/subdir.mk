################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Propiedades_Funcionamiento/Lissandra.c \
../Propiedades_Funcionamiento/bloque.c \
../Propiedades_Funcionamiento/compactador.c \
../Propiedades_Funcionamiento/comunity_func.c \
../Propiedades_Funcionamiento/conexiones.c \
../Propiedades_Funcionamiento/configuracion.c \
../Propiedades_Funcionamiento/consola.c \
../Propiedades_Funcionamiento/file_system_aux.c \
../Propiedades_Funcionamiento/metadata.c \
../Propiedades_Funcionamiento/parser.c \
../Propiedades_Funcionamiento/particion_contenido.c \
../Propiedades_Funcionamiento/requests.c 

OBJS += \
./Propiedades_Funcionamiento/Lissandra.o \
./Propiedades_Funcionamiento/bloque.o \
./Propiedades_Funcionamiento/compactador.o \
./Propiedades_Funcionamiento/comunity_func.o \
./Propiedades_Funcionamiento/conexiones.o \
./Propiedades_Funcionamiento/configuracion.o \
./Propiedades_Funcionamiento/consola.o \
./Propiedades_Funcionamiento/file_system_aux.o \
./Propiedades_Funcionamiento/metadata.o \
./Propiedades_Funcionamiento/parser.o \
./Propiedades_Funcionamiento/particion_contenido.o \
./Propiedades_Funcionamiento/requests.o 

C_DEPS += \
./Propiedades_Funcionamiento/Lissandra.d \
./Propiedades_Funcionamiento/bloque.d \
./Propiedades_Funcionamiento/compactador.d \
./Propiedades_Funcionamiento/comunity_func.d \
./Propiedades_Funcionamiento/conexiones.d \
./Propiedades_Funcionamiento/configuracion.d \
./Propiedades_Funcionamiento/consola.d \
./Propiedades_Funcionamiento/file_system_aux.d \
./Propiedades_Funcionamiento/metadata.d \
./Propiedades_Funcionamiento/parser.d \
./Propiedades_Funcionamiento/particion_contenido.d \
./Propiedades_Funcionamiento/requests.d 


# Each subdirectory must supply rules for building sources it contributes
Propiedades_Funcionamiento/%.o: ../Propiedades_Funcionamiento/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


