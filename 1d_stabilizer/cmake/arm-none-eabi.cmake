set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)   # same binary dispatches based on file ext: .c vs .s
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc) 

# CPU FLAGS:
#   - mcpu: cortex-m4           : target Cortex-M4 instruction set (STM32-F446 family uses M4F with FPU)

#   - mfpu: fpv4-sp-d16         : use FPv4 single precision FPU with 16 registers (M4F HW FPU)
#                                   - fpv4: ARM FPU architecture version (4)
#                                     - sp: single precision (supports 32-bit floats), 64-bit falls back to SW emulation
#                                     - dp: double-precision ops not avialable. dp-ops fall back to SW emulation
#                                     - 32 sp-registers available 
#                                     - d16: specifies register file size (32 sp-registers or 16 dp-registers)

#                                   - sp-float operations take 1-3 cycles on FPU 
#                                   - sw emulated dp-float operations are ~50 slower (and use CPU int-registers)

#   - mfloat-abi=hard           : pass float args in fpu registers (required matching libc)
#                                   - libc: std c library (implements all std functions: stdio, stdlib, string, etc.) 

#                                   - thumb/v7e-m/fpv4-sp/hard/ corresponds to Cortex-M4F, FPv4-SP, hard ABI 
#                                     - linker uses Cortex-M4 instructions (Thumb-2)
#                                     - linker expects fpu instructions to work 
#                                     - linker passes foats into FPU registers (hard ABI)

#                                     - without matching flags, linker grabs soft-variant
#                                       - code passes `float arg` into FPU registers 
#                                       - libc's printf function reads the float arg from int registers (garbage)

#   - mthumb                    : emit Thumb-2 instructions (Cortex-M only supports these)


set(CPU_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb")
set(CMAKE_C_FLAGS_INIT          "${CPU_FLAGS}")
set(CMAKE_ASM_FLAGS_INIT        "${CPU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${CPU_FLAGS}")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
