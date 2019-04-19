#ifndef _FLAG_HANDLING_H_
#define _FLAG_HANDLING_H_

#define NB_FLAGS 6
const char* flags_list[NB_FLAGS];
#define NB_FILE_OR_DIRECTORY_CAN_BE_SEND_IN_PARAMETERS 10
#define PATH_SIZE_MAX 500

/* prototypes */
void handling_flag(int argc, char *argv[]);
void print_help();
int isNumeric(char* str);


#endif