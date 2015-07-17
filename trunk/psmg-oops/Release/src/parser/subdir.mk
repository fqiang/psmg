################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/parser/data.tab.cpp \
../src/parser/data.yy.cpp \
../src/parser/sml.tab.cpp \
../src/parser/sml.yy.cpp 

OBJS += \
./src/parser/data.tab.o \
./src/parser/data.yy.o \
./src/parser/sml.tab.o \
./src/parser/sml.yy.o 

CPP_DEPS += \
./src/parser/data.tab.d \
./src/parser/data.yy.d \
./src/parser/sml.tab.d \
./src/parser/sml.yy.d 


# Each subdirectory must supply rules for building sources it contributes
src/parser/%.o: ../src/parser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DRELEASE=1 -DDEBUG=0 -I/home/s0965328/workspace/oops/oops_agr/oops/include -I/home/s0965328/usr/local/include -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


