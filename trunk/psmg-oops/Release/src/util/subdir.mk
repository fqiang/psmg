################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/util/DummyVarGen.cpp \
../src/util/util.cpp 

OBJS += \
./src/util/DummyVarGen.o \
./src/util/util.o 

CPP_DEPS += \
./src/util/DummyVarGen.d \
./src/util/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/util/%.o: ../src/util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DRELEASE=1 -DDEBUG=0 -I/home/s0965328/workspace/oops/oops_agr/oops/include -I/home/s0965328/usr/local/include -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


