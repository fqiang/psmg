################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/xml/subdir.mk
-include src/util/subdir.mk
-include src/st_model/subdir.mk
-include src/sml/subdir.mk
-include src/parser/subdir.mk
-include src/oops/subdir.mk
-include src/model/subdir.mk
-include src/metric/subdir.mk
-include src/interface/subdir.mk
-include src/context/subdir.mk
-include src/autodiff/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: psmg-oops

# Tool invocations
psmg-oops: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	mpic++ -L/home/s0965328/workspace/oops/oops_Argonne_2014/lib/parallel_gnu -L/home/s0965328/workspace/oops/oops_Argonne_2014/lib/metis-5/build/Linux-x86_64/libmetis -o "psmg-oops" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) psmg-oops
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
