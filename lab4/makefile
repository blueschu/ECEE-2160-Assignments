# Basic makefile for lab 4.
# Reference: https://www.gnu.org/software/make/manual/make.html

# Use cross-compiler for armhf
CXX = arm-linux-gnueabihf-g++

# Rational for flags is discussed in top-level CMakeLists.txt and report for lab 0.
CPPFLAGS = -std=c++17 -Wall -Wextra -pedantic -Werror -Wconversion -D_GLIBCXX_USE_CXX11_ABI=0

lab4-arm : de1soc_register_io.o lab4.cpp de1soc_config.h led_array.h \
			seven_segment_display.h switch_array.h wrapping_counter.h register_io.h
		${CXX} ${CPPFLAGS} lab4.cpp de1soc_device.o -o lab4-arm

de1soc_register_io.o : de1soc_register_io.cpp de1soc_register_io.h register_io.h \
			de1soc_config.h posix_api.h
		${CXX} ${CPPFLAGS} -c de1soc_register_io.cpp -o de1soc_device.o


# Only two object file are created since most utilties for this lab have been templated.

.PHONY: clean
clean :
		rm de1soc_device.o lab4-arm
