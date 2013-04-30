################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/context/ExpandedModel2.cpp \
../src/context/ModelContext.cpp \
../src/context/Param.cpp \
../src/context/Set.cpp \
../src/context/SetValue.cpp \
../src/context/Var.cpp \
../src/context/VarValue.cpp 

OBJS += \
./src/context/ExpandedModel2.o \
./src/context/ModelContext.o \
./src/context/Param.o \
./src/context/Set.o \
./src/context/SetValue.o \
./src/context/Var.o \
./src/context/VarValue.o 

CPP_DEPS += \
./src/context/ExpandedModel2.d \
./src/context/ModelContext.d \
./src/context/Param.d \
./src/context/Set.d \
./src/context/SetValue.d \
./src/context/Var.d \
./src/context/VarValue.d 


# Each subdirectory must supply rules for building sources it contributes
src/context/%.o: ../src/context/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DDEBUG -I/usr/include/mpi -I/home/s0965328/workspace/oops/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


