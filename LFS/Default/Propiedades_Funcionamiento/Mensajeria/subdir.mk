################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Propiedades_Funcionamiento/Mensajeria/mensajes.c \
../Propiedades_Funcionamiento/Mensajeria/recibir.c \
../Propiedades_Funcionamiento/Mensajeria/request.c \
../Propiedades_Funcionamiento/Mensajeria/requestCreate.c \
../Propiedades_Funcionamiento/Mensajeria/requestDescribe.c \
../Propiedades_Funcionamiento/Mensajeria/requestDrop.c \
../Propiedades_Funcionamiento/Mensajeria/requestInsert.c \
../Propiedades_Funcionamiento/Mensajeria/requestSelect.c 

OBJS += \
./Propiedades_Funcionamiento/Mensajeria/mensajes.o \
./Propiedades_Funcionamiento/Mensajeria/recibir.o \
./Propiedades_Funcionamiento/Mensajeria/request.o \
./Propiedades_Funcionamiento/Mensajeria/requestCreate.o \
./Propiedades_Funcionamiento/Mensajeria/requestDescribe.o \
./Propiedades_Funcionamiento/Mensajeria/requestDrop.o \
./Propiedades_Funcionamiento/Mensajeria/requestInsert.o \
./Propiedades_Funcionamiento/Mensajeria/requestSelect.o 

C_DEPS += \
./Propiedades_Funcionamiento/Mensajeria/mensajes.d \
./Propiedades_Funcionamiento/Mensajeria/recibir.d \
./Propiedades_Funcionamiento/Mensajeria/request.d \
./Propiedades_Funcionamiento/Mensajeria/requestCreate.d \
./Propiedades_Funcionamiento/Mensajeria/requestDescribe.d \
./Propiedades_Funcionamiento/Mensajeria/requestDrop.d \
./Propiedades_Funcionamiento/Mensajeria/requestInsert.d \
./Propiedades_Funcionamiento/Mensajeria/requestSelect.d 


# Each subdirectory must supply rules for building sources it contributes
Propiedades_Funcionamiento/Mensajeria/%.o: ../Propiedades_Funcionamiento/Mensajeria/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


