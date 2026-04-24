set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)   # same binary dispatches based on file ext: .c vs .s
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc) 
set(CMAKE_C_FLAGS_INIT "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
