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
	mpic++ -DDEBUG=0 -DRELEASE=1 -I/home/s0965328/workspace/oops/oops_agr/oops/include -I/usr/include/mpi -O3 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


