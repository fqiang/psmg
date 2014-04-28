################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/metric/Stat.cpp \
../src/metric/TimeStamp.cpp 

OBJS += \
./src/metric/Stat.o \
./src/metric/TimeStamp.o 

CPP_DEPS += \
./src/metric/Stat.d \
./src/metric/TimeStamp.d 


# Each subdirectory must supply rules for building sources it contributes
src/metric/%.o: ../src/metric/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -I/home/s0965328/workspace/oops/oops_agr/oops/include -I/usr/include/mpi -I/home/s0965328/workspace/pugixml/src -I/home/s0965328/workspace/autodiff/autodiff_library/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


