################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../API_Pool.c \
../dato.c \
../memoria.c \
../pagina.c \
../pool.c \
../segmento.c \
../test1.c 

OBJS += \
./API_Pool.o \
./dato.o \
./memoria.o \
./pagina.o \
./pool.o \
./segmento.o \
./test1.o 

C_DEPS += \
./API_Pool.d \
./dato.d \
./memoria.d \
./pagina.d \
./pool.d \
./segmento.d \
./test1.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


