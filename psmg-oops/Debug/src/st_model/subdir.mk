################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/st_model/StageNodeNode.cpp \
../src/st_model/StochModel.cpp \
../src/st_model/StochModelComp.cpp 

OBJS += \
./src/st_model/StageNodeNode.o \
./src/st_model/StochModel.o \
./src/st_model/StochModelComp.o 

CPP_DEPS += \
./src/st_model/StageNodeNode.d \
./src/st_model/StochModel.d \
./src/st_model/StochModelComp.d 


# Each subdirectory must supply rules for building sources it contributes
src/st_model/%.o: ../src/st_model/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -DDEBUG -I/usr/include/mpi -I/home/s0965328/workspace/autodiff/autodiff_library/include -I/home/s0965328/workspace/oops/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


