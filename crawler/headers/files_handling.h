#ifndef _FILES_HANDLING_H_
#define _FILES_HANDLING_H_

/* constantes */
#define FILE_LINE_SIZE_MAX 1000
#define FILE_NUMBER_OF_LINE_MAX 1000

/* Prototypes */
void print_file(char* file_path);
int isFile(const char* name);
int isDirectory(const char* directory_path);
void print_file_contain(char* file_path, char* str);
void print_line_of_file_contain(char* file_path, char* str);
int nb_occ(char* chaine,char car);
void* getWhatToPrintInFile(void* arg);
char *getWhatToPrint(int type_of_filter, char* path, char* filter, int* size_of_what_to_print);

char *fileToString( char *filePath, int *bufferLength );
int getCharFilesNumbers ( char *filePath );
char  *allocateCharPtr  ( int wordSize );

#endif