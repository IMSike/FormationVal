#ifndef _FILES_HANDLING_H_
#define _FILES_HANDLING_H_

/* constantes */
#define FILE_LINE_SIZE_MAX 1000

/* Prototypes */
void print_file(char* file_path);
int isFile(const char* name);
void print_file_contain(char* file_path, char* str);
void print_line_of_file_contain(char* file_path, char* str);

#endif