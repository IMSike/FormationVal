cmake_minimum_required(VERSION 3.0)

project(my_malloc)

set(	SRCS
	tests/main.c
	src/my_malloc.c
	src/blocks_management.c
	src/my_outputs.c)

set(	HEADERS
	headers/my_malloc.h
	headers/blocks_management.h
	headers/my_outputs.h)

set(	GCC_COVERAGE_COMPILE_FLAGS "-W -Wextra -Wall")

add_library( myMalloc SHARED ${SRCS} ${HEADERS})
add_executable(my_malloc tests/main.c)
target_link_libraries(my_malloc myMalloc)
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

target_include_directories(myMalloc PUBLIC headers)
target_include_directories(my_malloc PUBLIC headers)