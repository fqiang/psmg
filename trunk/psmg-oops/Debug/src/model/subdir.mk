################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/model/AmplModel.cpp \
../src/model/ConsComp.cpp \
../src/model/ModelComp.cpp \
../src/model/ObjComp.cpp \
../src/model/ParamComp.cpp \
../src/model/SetComp.cpp \
../src/model/SyntaxNode.cpp \
../src/model/SyntaxNodeID.cpp \
../src/model/SyntaxNodeIDREF.cpp \
../src/model/SyntaxNodeIDREFM.cpp \
../src/model/SyntaxNodeString.cpp \
../src/model/SyntaxNodeSumExp.cpp \
../src/model/SyntaxNodeValue.cpp \
../src/model/VarComp.cpp 

OBJS += \
./src/model/AmplModel.o \
./src/model/ConsComp.o \
./src/model/ModelComp.o \
./src/model/ObjComp.o \
./src/model/ParamComp.o \
./src/model/SetComp.o \
./src/model/SyntaxNode.o \
./src/model/SyntaxNodeID.o \
./src/model/SyntaxNodeIDREF.o \
./src/model/SyntaxNodeIDREFM.o \
./src/model/SyntaxNodeString.o \
./src/model/SyntaxNodeSumExp.o \
./src/model/SyntaxNodeValue.o \
./src/model/VarComp.o 

CPP_DEPS += \
./src/model/AmplModel.d \
./src/model/ConsComp.d \
./src/model/ModelComp.d \
./src/model/ObjComp.d \
./src/model/ParamComp.d \
./src/model/SetComp.d \
./src/model/SyntaxNode.d \
./src/model/SyntaxNodeID.d \
./src/model/SyntaxNodeIDREF.d \
./src/model/SyntaxNodeIDREFM.d \
./src/model/SyntaxNodeString.d \
./src/model/SyntaxNodeSumExp.d \
./src/model/SyntaxNodeValue.d \
./src/model/VarComp.d 


# Each subdirectory must supply rules for building sources it contributes
src/model/%.o: ../src/model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DDEBUG=0 -DRELEASE=1 -I/home/s0965328/workspace/oops/oops_agr/oops/include -I/usr/include/mpi -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


