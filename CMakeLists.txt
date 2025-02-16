# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.18)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(tm1638 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# turn on all compiler warnings 
add_compile_options(-Wall -Wextra)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
  examples/MODEL1/main.cpp
  #examples/MODEL2/main.cpp
  #examples/MODEL3/main.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

add_library(pico_tm1638 INTERFACE)

target_sources(pico_tm1638 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/tm1638plus.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/tm1638plus_model2.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/tm1638plus_common.cpp
)

target_include_directories(pico_tm1638 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(${PROJECT_NAME} pico_stdlib pico_tm1638 )


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

