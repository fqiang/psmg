################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/st_model/StochCtx.cpp \
../src/st_model/StochModel.cpp 

OBJS += \
./src/st_model/StochCtx.o \
./src/st_model/StochModel.o 

CPP_DEPS += \
./src/st_model/StochCtx.d \
./src/st_model/StochModel.d 


# Each subdirectory must supply rules for building sources it contributes
src/st_model/%.o: ../src/st_model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DRELEASE=1 -DDEBUG=0 -I/home/s0965328/workspace/oops/oops_agr/oops/include -I/home/s0965328/usr/local/include -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


