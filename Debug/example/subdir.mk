################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../example/example.o 

CPP_SRCS += \
../example/example.cpp 

OBJS += \
./example/example.o 

CPP_DEPS += \
./example/example.d 


# Each subdirectory must supply rules for building sources it contributes
example/%.o: ../example/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/usr/local/dev/cpp/logstream" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


