#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "crawler.h"


int main(int argc, char *argv[])
{
	handling_flag(argc, argv);
	return (0);

/*

	 int i = 0;
    int j = 0;
    while (1){
        fd_set fds;
        struct timeval timeout = {0, 0};

        FD_ZERO(&fds);
        FD_SET(0, &fds);
        if (select(0 + 1, &fds, 0, 0, &timeout) <= 0){
            if (j < 20)
            {
                printf("0\n");
                j++;
            }
        } else {
            if (i < 20){
                printf("1\n");
                i++;
            }
        }
    }
    return (0);*/
}