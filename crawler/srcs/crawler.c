#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>
#include "crawler.h"
#include "mutex.h"

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
	if(chaine != NULL)
		for(int i = 0; chaine[i] != '\0'; i++)
			if(chaine[i] == '\n')
				chaine[i] = '\0';
}

//prend la chaine command et la décompose suivant le contenu. renvoi le type de filtre à utiliser (0 = pas de filtre, 1 = filtre -o, 2 = filtre -r)
int read_user_command(char* command, char* action, char* file, char* filter)
{
	int type_of_filter = 0;
	char* p = NULL;
	if(command != NULL)
	{
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
	}
	remove_returnLine(action);
	remove_returnLine(file);
	remove_returnLine(filter);
	return type_of_filter;
}

void create_complete_path(char* complete_path, char* part_1, char* part_2)
{
	*complete_path = '\0';
	strcat(complete_path, part_1);
	strcat(complete_path, "/");
	strcat(complete_path, part_2);
}

void create_thread(t_com_s *arg, int type_of_filter, char* path, char* filter, int *size_of_message, pthread_t *tid)
{
	printf("TEST_create_thread\n");
	arg->type_of_filter = type_of_filter;
	arg->path = malloc(sizeof(char)*PATH_SIZE_MAX);
	strcpy(arg->path, path);
	arg->filter = malloc(sizeof(char)*FILTER_SIZE_MAX);
	strcpy(arg->filter, filter);
	arg->size_of_what_to_print = size_of_message;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(tid, &attr, getWhatToPrintInFile, arg);
	printf("test_fin-create_thread\n");
}


void handling_print_directory(int type_of_filter, char* path, char* filter, int number_of_threads_by_child, int number_of_childs_can_be_active)
{
	int fd;
	fd_set *set = malloc(sizeof(fd_set)*100);
	struct timeval tv;
	FD_ZERO(set);
	FD_SET(fd, set);
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	char* my_fifo = PATH_OF_COMMUNICATION_FILE_FOR_NAMED_PIPE;
	char* name_of_pipe = malloc(sizeof(char)*PATH_SIZE_MAX);

	t_com_s args[number_of_threads_by_child];
	pthread_t tids[number_of_threads_by_child];

	struct dirent *lecture;
	DIR *rep;
	rep = opendir(path);
	char* complete_path = NULL;
	complete_path = malloc(sizeof(char*)*PATH_SIZE_MAX);

	char* message;
	message = malloc(sizeof(char)*SIZE_OF_CONTENT_FILE_MAX*NUMBER_OF_FILE_MAX);
	int size_of_message;

	int inc_of_active_processus = 0;
	pid_t pid[number_of_childs_can_be_active];

	lecture = readdir(rep);
	while(lecture)
	{
		printf("TEST_while(lecture)________________\n");
		while(inc_of_active_processus < number_of_childs_can_be_active && lecture) //tant qu'on est en dessous de la limite de process
		{
			printf("TEST_while_process-------------------------------------------------\n");
			pid[inc_of_active_processus] = create_process();
			if(pid[inc_of_active_processus] == -1)
				printf("ERROR : FAILLURE WHEN CREATE A PROCESS in handling_print_directory() in crawler.c\n");
			else if(pid[inc_of_active_processus] == 0) // processus fils
			{
				//int retval;

				int inc_of_active_thread = 0;
				sprintf(name_of_pipe, "%s%d", my_fifo, inc_of_active_processus);
				printf("+%s++\n", name_of_pipe);
				mkfifo(name_of_pipe, 0666);
				while(inc_of_active_thread < number_of_threads_by_child && lecture) //tant qu'on est en dessous de la limite de threads
				{
					printf("TEST_while_thread~~~~~~~~~~~~~~~~~~~~~~~\n");
					int size_of_message;
					create_complete_path(complete_path, path, lecture->d_name);
					if(isDirectory(complete_path) == 0)
					{
						printf("TEST_thread_created\n");
						create_thread(&args[inc_of_active_thread], type_of_filter, complete_path, filter, &size_of_message, &tids[inc_of_active_thread]);
						inc_of_active_thread++;
					}
					lecture = readdir(rep);
				}
				printf("TEST\n");
				//on condense les info et on envoi au processus Père
				//FD_ZERO(set);
				//FD_SET(fd, set);
				/*retval = select(fd+1, NULL, set, NULL ,&tv);
				printf("\n\n%s\n\n", retval);
				if (retval == -1)
					printf("ERROR : select() in handling_print_directory() in crawler.c\n");
				else if (retval)
				{
					printf("test\n");
				        fd = open(name_of_pipe, O_WRONLY);
				}
				else
					printf("ERROR : CAN'T WRITE IN FD in handling_print_directory() in crawler.c\n");
*/
				fd = open(name_of_pipe, O_WRONLY);
				printf("TEST2\n");
				for(int inc = 0; inc < inc_of_active_thread; inc++)
				{
					//printf("|%d|%d|\n", inc, inc_of_active_thread);
					pthread_join(tids[inc], NULL);
					//printf("--%s--\n", args[inc].what_to_print);
					lock();
					printf("TEST_for_thread\n");
					printf("--%s!\n", args[inc].what_to_print);
					write(fd, args[inc].what_to_print, (*args[inc].size_of_what_to_print));
					unlock();
				}
				close(fd);
				printf("TEST_fin_thread\n");
				exit(0);
			}
			else // processus père
			{
				printf("TEST_processusPere\n");
				inc_of_active_processus++;
				for(int inc = 0; inc < number_of_threads_by_child; inc++)
				{
					lecture = readdir(rep);
					if(!lecture)
						break;
				}
				//wait(NULL);
			}
		}
		//if(inc_of_active_processus == number_of_childs_can_be_active && lecture)
		//{
			
		for(int inc = 0; inc < inc_of_active_processus; inc++)
		{
			printf("TEST_for : %d|%d\n", inc, inc_of_active_processus);
			wait(NULL);
			printf("TEST_for2\n");
		}
		for(int inc = 0; inc < inc_of_active_processus; inc++)
		{
			sprintf(name_of_pipe, "%s%d", my_fifo, inc);
			printf("+%s+\n", name_of_pipe);
			fd = open(name_of_pipe, O_RDONLY);

			lock();
			printf("TEST!\n");

			int retval;
			retval = select(fd+1, set, NULL, NULL ,&tv);
			if (retval == -1)
				printf("ERROR : select() in handling_print_directory() in crawler.c\n");
			else if (retval)
			        size_of_message = read(fd, message, SIZE_OF_CONTENT_FILE_MAX);
			else
				printf("ERROR : CAN'T READ FD in handling_print_directory() in crawler.c\n");

			
			printf("TEST!\n");
			unlock();
			close(fd);
			printf("TEST!\n");
			write(1, message, size_of_message);
			printf("TESt_FIN2\n");

			inc_of_active_processus = 0;
			lecture = readdir(rep);
		}
		//}
		/*else
			for(int inc = 0; inc < inc_of_active_processus; inc++)
			{
				printf("TES_for\n");
				wait(NULL);
				printf("TES_for2\n");
			}*/
		//récupération des infos et vidage de process
	}
	free(message);
	free(complete_path);
	free(name_of_pipe);
}







/*
void handling_print_directory(int type_of_filter, char* path, char* filter, int number_of_threads_by_child, int number_of_childs_can_be_active)
{
	t_com_s args[number_of_threads_by_child];
	int inc_of_active_thread = 0;
	pthread_t tids[number_of_threads_by_child];

	//int inc_of_active_processus = 0;
	//pid_t pid[number_of_childs_can_be_active];

	struct dirent *lecture;
	DIR *rep;
	rep = opendir(path);
	char* complete_path = NULL;
	complete_path = malloc(sizeof(char*)*PATH_SIZE_MAX);

	lecture = readdir(rep);
	while(lecture)
	{
		int size_of_message;
		*complete_path = '\0';
		strcat(complete_path, path);
		strcat(complete_path, "/");
		strcat(complete_path, lecture->d_name);
		if(isDirectory(complete_path) == 0) //si on tombe sur autre chose qu'un dossier (fichier), on s'en occupe
		{
			args[inc_of_active_thread].type_of_filter = type_of_filter;
			args[inc_of_active_thread].path = malloc(sizeof(char)*PATH_SIZE_MAX);
			strcpy(args[inc_of_active_thread].path, complete_path);
			args[inc_of_active_thread].filter = malloc(sizeof(char)*FILTER_SIZE_MAX);
			strcpy(args[inc_of_active_thread].filter, filter);
			args[inc_of_active_thread].size_of_what_to_print = &size_of_message;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&tids[inc_of_active_thread], &attr, getWhatToPrintInFile, &args[inc_of_active_thread]);
			inc_of_active_thread++;
		}
		if(inc_of_active_thread == number_of_threads_by_child)
		{
			for(int inc = 0; inc < inc_of_active_thread; inc++)
			{
				pthread_join(tids[inc], NULL);
				printf("\nfile name : %s\nSTART\n", args[inc].path);
				printf("%s\n", args[inc].what_to_print);
				printf("END\n");
			}
			inc_of_active_thread = 0;
		}
		lecture = readdir(rep);
	}
	for(int inc = 0; inc < inc_of_active_thread; inc++)
	{
		pthread_join(tids[inc], NULL);
		printf("\nfile name : %s\nSTART\n", args[inc].path);
		printf("%s\n", args[inc].what_to_print);
		printf("END\n");
	}
	free(complete_path);
	closedir(rep);
}
*/


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
			int fd;
			char* my_fifo = PATH_OF_COMMUNICATION_FILE_FOR_NAMED_PIPE;
			mkfifo(my_fifo, 0666);
			int size_of_message;
			char* message;
			message = malloc(sizeof(char)*SIZE_OF_CONTENT_FILE_MAX);
			pid_t pid = create_process();
			if(pid == -1)
				printf("ERROR : FAILLURE WHEN CREATE A PROCESS in print_something() in crawler.c\n");
			else if(pid == 0) // processus fils
			{
				fd = open(my_fifo, O_WRONLY);
				message = getWhatToPrint(type_of_filter, path, filter, &size_of_message);
				write(fd, message, size_of_message);
				close(fd);
				exit(0);
			}
			else // processus père
			{
				fd = open(my_fifo, O_RDONLY);
				wait(NULL);
				size_of_message = read(fd, message, SIZE_OF_CONTENT_FILE_MAX);
				close(fd);
				printf("\nfile name : %s\nSTART\n", path);
				write(1, message, size_of_message);
				printf("\nEND\n");
			}

			/*
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
			*/
		}
		printf("\n");
	}
	else
		printf("ERROR : PATH NAME ISN'T RECOGNIZED AS FILE OR DIRECTORY in print_something() in crawler.c\n");
}

void handling_inputs(int number_of_threads_by_child, int number_of_childs_can_be_active)
{
	char *action = NULL;
	char *file = NULL;
	char *filter = NULL;
	char *input = malloc(sizeof(char)*PATH_SIZE_MAX);
	//char input[PATH_SIZE_MAX];
	int type_of_filter = 0; // 0 = pas de filtre, 1 = filtre -o, 2 = filtre -r
	int have_to_quit = 0;

	action = malloc(sizeof(char*)*(PATH_SIZE_MAX/10));
	file = malloc(sizeof(char*)*(PATH_SIZE_MAX/2));
	filter = malloc(sizeof(char*)*(PATH_SIZE_MAX/10));

	if(number_of_childs_can_be_active == 0)
		number_of_childs_can_be_active = 99999;
	if(number_of_threads_by_child == 0)
		number_of_threads_by_child = 99999;

	while(have_to_quit == 0)
	{
		printf("PLEASE ENTER A COMMAND ('help' to know more, 'quit' to exit) >> \n");
		read(0, input, PATH_SIZE_MAX);
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
	free(input);
}