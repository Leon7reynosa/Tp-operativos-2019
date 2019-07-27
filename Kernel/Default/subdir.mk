################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../API_Kernel.c \
../consistencia.c \
../consola.c \
../gossiping.c \
../kernel.c \
../metadata.c \
../metricas.c \
../parser.c \
../planificador.c \
../request_memoria.c \
../semaforos.c 

OBJS += \
./API_Kernel.o \
./consistencia.o \
./consola.o \
./gossiping.o \
./kernel.o \
./metadata.o \
./metricas.o \
./parser.o \
./planificador.o \
./request_memoria.o \
./semaforos.o 

C_DEPS += \
./API_Kernel.d \
./consistencia.d \
./consola.d \
./gossiping.d \
./kernel.d \
./metadata.d \
./metricas.d \
./parser.d \
./planificador.d \
./request_memoria.d \
./semaforos.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


