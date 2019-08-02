################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Mensajeria/mensajes.c \
../Mensajeria/recibir.c \
../Mensajeria/request.c \
../Mensajeria/requestCreate.c \
../Mensajeria/requestDescribe.c \
../Mensajeria/requestDrop.c \
../Mensajeria/requestInsert.c \
../Mensajeria/requestJournal.c \
../Mensajeria/requestSelect.c \
../Mensajeria/t_dato.c 

OBJS += \
./Mensajeria/mensajes.o \
./Mensajeria/recibir.o \
./Mensajeria/request.o \
./Mensajeria/requestCreate.o \
./Mensajeria/requestDescribe.o \
./Mensajeria/requestDrop.o \
./Mensajeria/requestInsert.o \
./Mensajeria/requestJournal.o \
./Mensajeria/requestSelect.o \
./Mensajeria/t_dato.o 

C_DEPS += \
./Mensajeria/mensajes.d \
./Mensajeria/recibir.d \
./Mensajeria/request.d \
./Mensajeria/requestCreate.d \
./Mensajeria/requestDescribe.d \
./Mensajeria/requestDrop.d \
./Mensajeria/requestInsert.d \
./Mensajeria/requestJournal.d \
./Mensajeria/requestSelect.d \
./Mensajeria/t_dato.d 


# Each subdirectory must supply rules for building sources it contributes
Mensajeria/%.o: ../Mensajeria/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


