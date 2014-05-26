################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/oops/OOPSBlock.cpp \
../src/oops/sml-oops_lp_qp.cpp \
../src/oops/sml-oops_nlp.cpp 

OBJS += \
./src/oops/OOPSBlock.o \
./src/oops/sml-oops_lp_qp.o \
./src/oops/sml-oops_nlp.o 

CPP_DEPS += \
./src/oops/OOPSBlock.d \
./src/oops/sml-oops_lp_qp.d \
./src/oops/sml-oops_nlp.d 


# Each subdirectory must supply rules for building sources it contributes
src/oops/%.o: ../src/oops/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Intel Intel(R) 64 C++ Compiler '
	/home/s0965328/usr/local/bin/mpic++ -g -O3 -w2 -opt-prefetch=3 -ipo -nolib-inline -I/home/s0965328/workspace/oops/oops_agr/oops/include -I/home/s0965328/usr/local/include -DRELEASE=1 -DDEBUG=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


