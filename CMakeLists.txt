cmake_minimum_required(VERSION 3.16)
project(ecee-2160-lab-reports)

# If we are cross-compiling for the DE1-SoC, configure CMake to use the
# pre-C++11 ABI.
# See report for lab0 for a brief discussion on why this setting is necessary.
if (CMAKE_CXX_COMPILER MATCHES "arm-linux-gnueabihf-g[\+][\+]$")
    add_compile_options(-D_GLIBCXX_USE_CXX11_ABI=0)
endif()

# Enable warnings; stop compilation if any warnings are produced.
add_compile_options(-Wall -Wextra -pedantic -Werror -Wconversion)

# Use C++17 for lab targets
set(CMAKE_CXX_STANDARD 17)

# Lab reports
add_subdirectory(common)
add_subdirectory(lab0)
add_subdirectory(lab1)
add_subdirectory(lab2)
add_subdirectory(lab3)
add_subdirectory(lab4)

# Use C++14 for homework targets
set(CMAKE_CXX_STANDARD 14)

# Homework assignments
add_subdirectory(hw1)
