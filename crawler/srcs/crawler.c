#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>
#include "crawler.h"

pid_t create_process(void)
{
	/* On crée une nouvelle valeur de type pid_t */
	pid_t pid;
	/* On fork() tant que l'erreur est EAGAIN */
	do
	{
		pid = fork();
	} while ((pid == -1) && (errno == EAGAIN));
	/* On retourne le PID du processus ainsi créé */
	return pid;
}

void remove_returnLine(char* chaine)
{
	for(int i = 0; chaine[i] != '\0'; i++)
		if(chaine[i] == '\n')
			chaine[i] = '\0';
}

//prend la chaine command et la décompose suivant le contenu. renvoi le type de filtre à utiliser (0 = pas de filtre, 1 = filtre -o, 2 = filtre -r)
int read_user_command(char* command, char* action, char* file, char* filter)
{
	int type_of_filter = 0;
	char* p = NULL;

	p = strtok(command, " ");
	if(p != NULL)
	{
		strcpy(action, p);
		p = strtok(NULL, " ");
		if(p != NULL)
		{
			strcpy(file, p);
			p = strtok(NULL, " ");
			if(p != NULL)
			{
				remove_returnLine(p);
				if(strcmp(p, "-r") == 0)
				{
					p = strtok(NULL, " ");
					if(p != NULL)
					{
						strcpy(filter, p);
						type_of_filter = 2;
					}
					else
						printf("ERROR : YOU USED -r OPTION WITHOUT CONTENT in read_user_command() in crawler.c\n");
				}
				else if(strcmp(p, "-o") == 0)
				{
					p = strtok(NULL, " ");
					if(p != NULL)
					{
						strcpy(filter, p);
						type_of_filter = 1;
						p = strtok(NULL, " ");
						if(p != NULL)
						{
							remove_returnLine(p);
							if(strcmp(p, "-r") == 0)
							{
								p = strtok(NULL, " ");
								if(p != NULL)
								{
									strcpy(filter, p);
									type_of_filter = 2;
								}
								else
									printf("ERROR : YOU USED -r OPTION WITHOUT CONTENT in read_user_command() in crawler.c\n");
							}
							else
								printf("ERROR : OPTION NOT RECOGNIZED in read_user_command() in crawler.c\n");
						}
					}
					else
						printf("ERROR : YOU USED -o OPTION WITHOUT CONTENT in read_user_command() in crawler.c\n");
				}
				else
					printf("ERROR : OPTION NOT RECOGNIZED in read_user_command() in crawler.c\n");
			}
		}
	}
	else
		printf("ERROR : NO USER COMMAND IN ENTRY in read_user_command() in crawler.c\n");

	remove_returnLine(action);
	remove_returnLine(file);
	remove_returnLine(filter);
	return type_of_filter;
}




void handling_print_directory(int type_of_filter, char* path, char* filter, int number_of_threads_by_child, int number_of_childs_can_be_active)
{
	t_com_s args[number_of_threads_by_child];
	int inc_of_active_thread = 0;
	pthread_t tids[number_of_threads_by_child];

	struct dirent *lecture;
	DIR *rep;
	rep = opendir(path);
	char* complete_path = NULL;
	complete_path = malloc(sizeof(char*)*PATH_SIZE_MAX);
	while((lecture = readdir(rep)))
	{
		/*char* message;
		message = malloc(sizeof(char)*SIZE_OF_CONTENT_FILE_MAX);*/
		int size_of_message;

		*complete_path = '\0';
		strcat(complete_path, path);
		strcat(complete_path, "/");
		strcat(complete_path, lecture->d_name);
		if(isDirectory(complete_path)) //si on tombe sur un dossier à l'intérieur du dossier
		{
			//TODO
		}
		else
		{
			printf("=====>> %s\n", complete_path);
			args[inc_of_active_thread].type_of_filter = type_of_filter;
			args[inc_of_active_thread].path = complete_path;
			args[inc_of_active_thread].filter = filter;
			args[inc_of_active_thread].size_of_what_to_print = &size_of_message;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&tids[inc_of_active_thread], &attr, getWhatToPrintInFile, &args[inc_of_active_thread]);
			//printf("||%s||\n", args[inc_of_active_thread].what_to_print);
			inc_of_active_thread++;
			printf("TEST : %d thread actif(%d) / %s\n", inc_of_active_thread, tids[inc_of_active_thread-1], args[inc_of_active_thread - 1].path );

			/*message = getWhatToPrint(type_of_filter, complete_path, filter, &size_of_message);
			if(message != "\0")
			{
				printf("\nfile name : %s\nSTART\n", lecture->d_name);
				write(1, message, size_of_message);
				printf("\nEND\n");
			}*/
		}
		if(inc_of_active_thread == number_of_threads_by_child)
		{
			for(int inc = 0; inc < inc_of_active_thread; inc++)
			{
				pthread_join(tids[inc], NULL);
				printf("\nfile name : %s\nSTART\n", args[inc].path);
				printf("%s\n", args[inc].what_to_print);
				printf("\nEND\n");
			}
			inc_of_active_thread = 0;
		}
	}
	for(int inc = 0; inc < inc_of_active_thread; inc++)
	{
		pthread_join(tids[inc], NULL);
		printf("TEST2 : %d sur %d (%d)\n", inc+1, inc_of_active_thread, tids[inc]);
		printf("\nfile name : %s\nSTART\n", args[inc].path);
		printf("~%s~\n", args[inc].what_to_print);
		printf("\nEND\n");
	}
	free(complete_path);
	closedir(rep);
}

void print_something(int type_of_filter, char* path, char* filter, int number_of_threads_by_child, int number_of_childs_can_be_active)
{
	if(isFile(path) == 1) //on verifie que path est bien un dossier ou repertoire
	{
		if(isDirectory(path) == 1) // si c'est un dossier
		{
			handling_print_directory(type_of_filter, path, filter, number_of_threads_by_child, number_of_childs_can_be_active);
		}
		else // si c'est un fichier
		{
			int fd[2];
			char* message;
			message = malloc(sizeof(char)*SIZE_OF_CONTENT_FILE_MAX);
			int size_of_message;
			if(pipe(fd) < 0)
				printf("ERROR : CAN'T CREATE PIPE in print_something() in crawler.c\n");
			pid_t pid = create_process();
			if(pid == -1)
				printf("ERROR : FAILLURE WHEN CREATE A PROCESS in print_something() in crawler.c\n");
			else if(pid == 0) // processus fils
			{
				close(fd[0]);
				message = getWhatToPrint(type_of_filter, path, filter, &size_of_message);
				write(fd[1], message, size_of_message);
				close(fd[1]);
				exit(0);
			}
			else // processus père
			{
				close(fd[1]);
				size_of_message = read(fd[0], message, SIZE_OF_CONTENT_FILE_MAX);
				printf("\nfile name : %s\nSTART\n", path);
				write(1, message, size_of_message);
				printf("\nEND\n");
				wait(NULL);
			}
			free(message);
		}
		printf("\n");
	}
	else
		printf("ERROR : PATH NAME ISN'T RECOGNIZED AS FILE OR DIRECTORY in print_something() in crawler.c\n");
}

void handling_inputs(int number_of_threads_by_child, int number_of_childs_can_be_active)
{
	char* action = NULL;
	char* file = NULL;
	char* filter = NULL;
	char input[PATH_SIZE_MAX];
	int type_of_filter = 0; // 0 = pas de filtre, 1 = filtre -o, 2 = filtre -r
	int have_to_quit = 0;

	action = malloc(sizeof(char*)*(PATH_SIZE_MAX/10));
	file = malloc(sizeof(char*)*(PATH_SIZE_MAX/2));
	filter = malloc(sizeof(char*)*(PATH_SIZE_MAX/10));

	while(have_to_quit == 0)
	{
		printf("PLEASE ENTER A COMMAND ('help' to know more, 'quit' to exit) >> \n");
		read(0, &input, PATH_SIZE_MAX);
		type_of_filter = read_user_command(input, action, file, filter);

		if(strcmp(action, "help") == 0) //affichage de l'aide
			print_help();

		if(strcmp(action, "read") == 0) //cas de lecture de fichier ou repertoire
			print_something(type_of_filter, file, filter, number_of_threads_by_child, number_of_childs_can_be_active);
		have_to_quit = (strcmp(action, "quit\0") == 0)? 1 : 0;
	}
	free(action);
	free(file);
	free(filter);
}