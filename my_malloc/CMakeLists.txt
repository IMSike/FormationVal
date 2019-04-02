cmake_minimum_required(VERSION 3.0)

project(my_malloc)

set(	SRCS
	tests/main.c
	src/my_malloc.c
	src/blocks_management.c)

set(	HEADERS
	headers/my_malloc.h
	headers/blocks_management.h)

set(	GCC_COVERAGE_COMPILE_FLAGS "-W -Wextra -Wall")

#add_library( myLibMalloc STATIC ${SRCS} ${HEADERS})
add_executable(my_malloc ${SRCS} ${HEADERS})
#target_link_libraries(my_malloc myLibMalloc)
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

#target_include_directories(myLibMalloc PUBLIC headers)
target_include_directories(my_malloc PUBLIC headers)