#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "crawler.h"


/*
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
*/

int main(int argc, char *argv[])
{
    handling_flag(argc, argv);



/*
int fd;
char* my_fifo = PATH_OF_COMMUNICATION_FILE_FOR_NAMED_PIPE;
mkfifo(my_fifo, 0666);
pid_t pid = create_process();
if(pid == -1)
    printf("ERROR : FAILLURE WHEN CREATE A PROCESS in main\n");
else if(pid == 0) // processus fils
{
    fd = open(my_fifo, O_WRONLY);
    char str[8] = "Bonjour\n";
    char *str_ptr = &str;
    write(fd, str_ptr, strlen(str_ptr)+1);
    close(fd);
}
else
{
    fd = open(my_fifo, O_RDONLY);
    char str2[8];
    char *str_ptr2 = &str2;
    read(fd, str_ptr2, sizeof(str_ptr2));
    printf(": %s :\n", str_ptr2);
    close(fd);
}





*/







	return (0);
}