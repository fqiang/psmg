################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sml/GlobalVariables.cpp \
../src/sml/Sml.cpp 

OBJS += \
./src/sml/GlobalVariables.o \
./src/sml/Sml.o 

CPP_DEPS += \
./src/sml/GlobalVariables.d \
./src/sml/Sml.d 


# Each subdirectory must supply rules for building sources it contributes
src/sml/%.o: ../src/sml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DDEBUG -I/usr/include/mpi -I/home/s0965328/workspace/oops/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

