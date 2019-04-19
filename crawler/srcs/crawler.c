#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "crawler.h"

int stdin_is_empty()
{
	char str[256];
	fd_set fds;
        struct timeval timeout = {0, 0};
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        if (select(0 + 1, &fds, 0, 0, &timeout) <= 0)
            	return 0;
        else 
	        return 1;
}


handling_inputs(int number_of_threads_by_child, int number_of_childs_can_be_active)
{
	char input[PATH_SIZE_MAX];
	int have_to_quit = 0;
	while(have_to_quit == 0)
	{
		printf(">");
		fscanf(stdin, "%s", input);
		if(strcmp(input, "help") == 0) //affichage de l'aide
			print_help();
		printf("---%d\n", stdin_is_empty() );
		if(strcmp(input, "read") == 0) //cas de lecture de fichier ou repertoire
		{
			char file[PATH_SIZE_MAX];
			char filter_o[PATH_SIZE_MAX];
			char filter_r[PATH_SIZE_MAX];

			fscanf(stdin, "%s", file);
			if(isFile(file) == 1) //on verifie que c'est bien un dossier ou repertoire
			{
				fscanf(stdin, "%s", input); //on cherche des filtres
				if(strcmp(input, "-r") == 0) //flag -r
					fscanf(stdin, "%s", filter_r);
				else if(strcmp(input, "-o") == 0) //flag -o
				{
					fscanf(stdin, "%s", filter_o);

				}
				else
					printf("ERROR : flag '%s' ISN'T RCOGNIZED in handling_inputs() in crawler.c\n", input);
			}
			else
				printf("ERROR : PATH NAME ISN'T RECOGNIZED in handling_inputs() in crawler.c\n");
		}
		have_to_quit = (strcmp(input, "quit\0") == 0)? 1 : 0;
	}
}