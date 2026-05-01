#!/usr/bin/env bash
set -euo pipefail

# only reconfigure if cache doesnt exist
if [ ! -f build/fw/CMakeCache.txt ]; then
  # -S: CMakeLists.txt source directory
  # -B: build directory housing generated files: Makefile, .o, .elf, .bin
  
  # generates Makefile with appropriate flags for the STM32F446RE board
  cmake -S . -B build/fw -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
fi

# run the Makefile to generate .elf and .bin file (target) for flashing
cmake --build build/fw
