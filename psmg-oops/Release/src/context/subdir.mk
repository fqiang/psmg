################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/context/Block.cpp \
../src/context/CBlock.cpp \
../src/context/EMBlock.cpp \
../src/context/ExpandedModel.cpp \
../src/context/IndexSet.cpp \
../src/context/ModelContext.cpp \
../src/context/Param.cpp \
../src/context/Set.cpp \
../src/context/Var.cpp 

OBJS += \
./src/context/Block.o \
./src/context/CBlock.o \
./src/context/EMBlock.o \
./src/context/ExpandedModel.o \
./src/context/IndexSet.o \
./src/context/ModelContext.o \
./src/context/Param.o \
./src/context/Set.o \
./src/context/Var.o 

CPP_DEPS += \
./src/context/Block.d \
./src/context/CBlock.d \
./src/context/EMBlock.d \
./src/context/ExpandedModel.d \
./src/context/IndexSet.d \
./src/context/ModelContext.d \
./src/context/Param.d \
./src/context/Set.d \
./src/context/Var.d 


# Each subdirectory must supply rules for building sources it contributes
src/context/%.o: ../src/context/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -I/usr/include/mpi -I/home/s0965328/workspace/autodiff/autodiff_library/include -I/home/s0965328/workspace/oops/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


