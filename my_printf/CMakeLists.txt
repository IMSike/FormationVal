cmake_minimum_required(VERSION 3.0)
project(my_printf)
set(	SRCS
	src/main.c
	src/my_printf.c)
set(	HEADERS
	headers/my_printf.h)
add_executable(my_printf ${SRCS} ${HEADERS})
target_include_directories(my_printf PUBLIC)