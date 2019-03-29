cmake_minimum_required(VERSION 3.0)

project(my_printf)

set(	SRCS
	src/main.c
	src/my_printf.c
	src/my_printf_options.c
	src/my_printf_handling.c)

set(	HEADERS
	headers/my_printf.h
	headers/my_printf_options.h
	headers/my_printf_handling.h)

set(	GCC_COVERAGE_COMPILE_FLAGS "-W -Wextra -Wall")

add_executable(my_printf ${SRCS} ${HEADERS})
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
target_include_directories(my_printf PUBLIC headers)