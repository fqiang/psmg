################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/model/AmplModel.cpp \
../src/model/CompositeSet.cpp \
../src/model/IDNode.cpp \
../src/model/IndexingSet.cpp \
../src/model/ListNode.cpp \
../src/model/ListSet.cpp \
../src/model/ModelComp.cpp \
../src/model/OpNode.cpp \
../src/model/SetNode.cpp \
../src/model/SimpleSet.cpp \
../src/model/SyntaxNode.cpp \
../src/model/SyntaxNodeIDREF.cpp \
../src/model/SyntaxNodeIx.cpp \
../src/model/ValueNode.cpp 

OBJS += \
./src/model/AmplModel.o \
./src/model/CompositeSet.o \
./src/model/IDNode.o \
./src/model/IndexingSet.o \
./src/model/ListNode.o \
./src/model/ListSet.o \
./src/model/ModelComp.o \
./src/model/OpNode.o \
./src/model/SetNode.o \
./src/model/SimpleSet.o \
./src/model/SyntaxNode.o \
./src/model/SyntaxNodeIDREF.o \
./src/model/SyntaxNodeIx.o \
./src/model/ValueNode.o 

CPP_DEPS += \
./src/model/AmplModel.d \
./src/model/CompositeSet.d \
./src/model/IDNode.d \
./src/model/IndexingSet.d \
./src/model/ListNode.d \
./src/model/ListSet.d \
./src/model/ModelComp.d \
./src/model/OpNode.d \
./src/model/SetNode.d \
./src/model/SimpleSet.d \
./src/model/SyntaxNode.d \
./src/model/SyntaxNodeIDREF.d \
./src/model/SyntaxNodeIx.d \
./src/model/ValueNode.d 


# Each subdirectory must supply rules for building sources it contributes
src/model/%.o: ../src/model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DDEBUG -I/usr/include/mpi -I/home/s0965328/workspace/autodiff/autodiff_library/include -I/home/s0965328/workspace/oops/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


