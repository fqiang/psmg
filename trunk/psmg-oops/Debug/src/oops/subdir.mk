################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/oops/psmg-oops.cpp 

OBJS += \
./src/oops/psmg-oops.o 

CPP_DEPS += \
./src/oops/psmg-oops.d 


# Each subdirectory must supply rules for building sources it contributes
src/oops/%.o: ../src/oops/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DDEBUG -I/usr/include/mpi -I/home/s0965328/workspace/autodiff/autodiff_library/include -I/home/s0965328/workspace/oops/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


