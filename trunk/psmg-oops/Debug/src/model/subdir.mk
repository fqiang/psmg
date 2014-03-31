################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/model/AmplModel.cpp \
../src/model/ConsComp.cpp \
../src/model/DumCompEntry.cpp \
../src/model/ModelComp.cpp \
../src/model/ObjComp.cpp \
../src/model/ParamComp.cpp \
../src/model/SetComp.cpp \
../src/model/SyntaxNode.cpp \
../src/model/SyntaxNodeID.cpp \
../src/model/SyntaxNodeIDREF.cpp \
../src/model/SyntaxNodeIDREFM.cpp \
../src/model/SyntaxNodeOP.cpp \
../src/model/SyntaxNodeValue.cpp \
../src/model/VarComp.cpp 

OBJS += \
./src/model/AmplModel.o \
./src/model/ConsComp.o \
./src/model/DumCompEntry.o \
./src/model/ModelComp.o \
./src/model/ObjComp.o \
./src/model/ParamComp.o \
./src/model/SetComp.o \
./src/model/SyntaxNode.o \
./src/model/SyntaxNodeID.o \
./src/model/SyntaxNodeIDREF.o \
./src/model/SyntaxNodeIDREFM.o \
./src/model/SyntaxNodeOP.o \
./src/model/SyntaxNodeValue.o \
./src/model/VarComp.o 

CPP_DEPS += \
./src/model/AmplModel.d \
./src/model/ConsComp.d \
./src/model/DumCompEntry.d \
./src/model/ModelComp.d \
./src/model/ObjComp.d \
./src/model/ParamComp.d \
./src/model/SetComp.d \
./src/model/SyntaxNode.d \
./src/model/SyntaxNodeID.d \
./src/model/SyntaxNodeIDREF.d \
./src/model/SyntaxNodeIDREFM.d \
./src/model/SyntaxNodeOP.d \
./src/model/SyntaxNodeValue.d \
./src/model/VarComp.d 


# Each subdirectory must supply rules for building sources it contributes
src/model/%.o: ../src/model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DDEBUG -I/usr/include/mpi -I/home/s0965328/workspace/autodiff/autodiff_library/include -I/home/s0965328/workspace/oops/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


