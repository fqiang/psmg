################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/oops/OOPSBlock.cpp \
../src/oops/sml-oops_lp.cpp \
../src/oops/sml-oops_nlp.cpp 

OBJS += \
./src/oops/OOPSBlock.o \
./src/oops/sml-oops_lp.o \
./src/oops/sml-oops_nlp.o 

CPP_DEPS += \
./src/oops/OOPSBlock.d \
./src/oops/sml-oops_lp.d \
./src/oops/sml-oops_nlp.d 


# Each subdirectory must supply rules for building sources it contributes
src/oops/%.o: ../src/oops/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	mpic++ -I/home/s0965328/workspace/oops/oops_agr/oops/include -I/usr/include/mpi -I/home/s0965328/workspace/pugixml/src -I/home/s0965328/workspace/autodiff/autodiff_library/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


