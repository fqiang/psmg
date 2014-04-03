################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/context/Block.cpp \
../src/context/BlockCons.cpp \
../src/context/BlockHV.cpp \
../src/context/BlockObj.cpp \
../src/context/Con.cpp \
../src/context/ConSingle.cpp \
../src/context/ExpandedModel.cpp \
../src/context/IndexSet.cpp \
../src/context/ModelContext.cpp \
../src/context/PValue.cpp \
../src/context/PValueSym.cpp \
../src/context/PValueValue.cpp \
../src/context/Param.cpp \
../src/context/ParamMult.cpp \
../src/context/ParamSingle.cpp \
../src/context/Set.cpp \
../src/context/SetOrdered.cpp \
../src/context/SetSimple.cpp \
../src/context/Var.cpp \
../src/context/VarSingle.cpp 

OBJS += \
./src/context/Block.o \
./src/context/BlockCons.o \
./src/context/BlockHV.o \
./src/context/BlockObj.o \
./src/context/Con.o \
./src/context/ConSingle.o \
./src/context/ExpandedModel.o \
./src/context/IndexSet.o \
./src/context/ModelContext.o \
./src/context/PValue.o \
./src/context/PValueSym.o \
./src/context/PValueValue.o \
./src/context/Param.o \
./src/context/ParamMult.o \
./src/context/ParamSingle.o \
./src/context/Set.o \
./src/context/SetOrdered.o \
./src/context/SetSimple.o \
./src/context/Var.o \
./src/context/VarSingle.o 

CPP_DEPS += \
./src/context/Block.d \
./src/context/BlockCons.d \
./src/context/BlockHV.d \
./src/context/BlockObj.d \
./src/context/Con.d \
./src/context/ConSingle.d \
./src/context/ExpandedModel.d \
./src/context/IndexSet.d \
./src/context/ModelContext.d \
./src/context/PValue.d \
./src/context/PValueSym.d \
./src/context/PValueValue.d \
./src/context/Param.d \
./src/context/ParamMult.d \
./src/context/ParamSingle.d \
./src/context/Set.d \
./src/context/SetOrdered.d \
./src/context/SetSimple.d \
./src/context/Var.d \
./src/context/VarSingle.d 


# Each subdirectory must supply rules for building sources it contributes
src/context/%.o: ../src/context/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DDEBUG -I/usr/include/mpi -I/home/s0965328/workspace/autodiff/autodiff_library/include -I/home/s0965328/workspace/oops/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


