cmake_minimum_required(VERSION 3.0)

project(my_crawler)

set(	SRCS
	tests/main.c
	srcs/crawler.c
	srcs/files_handling.c
	srcs/flag_handling.c)

set(	HEADERS
	headers/crawler.h
	headers/files_handling.h
	headers/flag_handling.h)

set(	GCC_COVERAGE_COMPILE_FLAGS "-W -Wextra -Wall")

add_library( myCrawler SHARED ${SRCS} ${HEADERS})
add_executable(my_crawler tests/main.c)
target_link_libraries(my_crawler myCrawler)
target_link_libraries(my_crawler pthread)
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

target_include_directories(myCrawler PUBLIC headers)
target_include_directories(my_crawler PUBLIC headers)