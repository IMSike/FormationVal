#ifndef _CRAWLER_H_
#define _CRAWLER_H_

#include <stdio.h>
#include <string.h>
#include "files_handling.h"
#include "flag_handling.h"
#include "sockets.h"
#include "mutex.h"

#define SIZE_OF_CONTENT_FILE_MAX 3000
#define FILTER_SIZE_MAX 300
#define NUMBER_OF_FILE_MAX 100
#define PATH_OF_COMMUNICATION_FILE_FOR_NAMED_PIPE "communication_file"

/* Prototypes */
void handling_inputs(int number_of_threads_by_child,int number_of_childs_can_be_active);
void remove_returnLine(char* chaine);
int read_user_command(char* command, char* action, char* file, char* filter);
void handling_print_directory(int type_of_filter, char* path, char* filter, int number_of_threads_by_child, int number_of_childs_can_be_active);
void print_something(int type_of_filter, char* path, char* filter, int number_of_threads_by_child, int number_of_childs_can_be_active);
pid_t create_process(void);

/* structures */
typedef struct thread_communication_struct
{
	int type_of_filter;
	char* path;
	char* filter;
	int* size_of_what_to_print;
	char* what_to_print;
}	t_com_s;

#endif