################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/context/BlockCons.cpp \
../src/context/BlockDep.cpp \
../src/context/BlockHV.cpp \
../src/context/BlockLP.cpp \
../src/context/BlockObj.cpp \
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
./src/context/BlockCons.o \
./src/context/BlockDep.o \
./src/context/BlockHV.o \
./src/context/BlockLP.o \
./src/context/BlockObj.o \
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
./src/context/BlockCons.d \
./src/context/BlockDep.d \
./src/context/BlockHV.d \
./src/context/BlockLP.d \
./src/context/BlockObj.d \
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
	mpic++ -I/home/s0965328/workspace/oops/oops_agr/oops/include -I/usr/include/mpi -I/home/s0965328/workspace/pugixml/src -I/home/s0965328/workspace/autodiff/autodiff_library/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


