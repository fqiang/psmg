################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sml/Config.cpp \
../src/sml/Sml.cpp 

OBJS += \
./src/sml/Config.o \
./src/sml/Sml.o 

CPP_DEPS += \
./src/sml/Config.d \
./src/sml/Sml.d 


# Each subdirectory must supply rules for building sources it contributes
src/sml/%.o: ../src/sml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Intel Intel(R) 64 C++ Compiler '
	/home/s0965328/usr/local/bin/mpic++ -g -O3 -w2 -opt-prefetch=3 -ipo -nolib-inline -I/home/s0965328/workspace/oops/oops_agr/oops/include -I/home/s0965328/usr/local/include -DRELEASE=1 -DDEBUG=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


