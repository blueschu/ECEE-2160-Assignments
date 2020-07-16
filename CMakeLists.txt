cmake_minimum_required(VERSION 3.16)
project(ecee-2160-lab-reports)

# Use C++17 for lab targets
set(CMAKE_CXX_STANDARD 17)

# Lab reports
add_subdirectory(lab0)
add_subdirectory(lab1)
add_subdirectory(lab2)

# Use C++14 for homework targets
set(CMAKE_CXX_STANDARD 14)

# Homework assignments
add_subdirectory(hw1)
