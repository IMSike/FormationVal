#include <dirent.h>
#include <ctype.h>
#include "crawler.h"

const char* flags_list[NB_FLAGS]=  {"-h", "--help", "-n", "-i", "-o", "-r"}; //si rajout de flag : modifier NB_FLAGS dans flag_handling.h

void print_help()
{
	printf("Bienvenue dans l'aide\n" );
}

//renvoie l'indice du flag dans le tableau flag_list. Si le flag n'est pas reconnu, renvoie -1
int check_is_flag(char* str)
{
	if(str != NULL)
	{
		int inc = 0;
		while(inc < NB_FLAGS)
		{
			if(strcmp(str, flags_list[inc]) == 0 )
				return inc;
			inc++;
		}
	}
	return -1;
}

//renvoi 1 si la chaine passée en parametre est numérique, 0 sinon
int isNumeric(char* str)
{
	int ok = 1; //1 si la chaine est numérique 0 sinon
	if(str != NULL)
	{
		int inc = 0;
		while(str[inc] != '\0' && ok > 0)
		{
			printf("---%d\n", isdigit(str[inc]));
			ok = isdigit(str[inc++]);
		}
	}
	return (ok > 1)? 1 : 0;
}

void handling_flag(int argc, char *argv[])
{
	char* directories[NB_FILE_OR_DIRECTORY_CAN_BE_SEND_IN_PARAMETERS];
	int nb_directories = 0;
	char* files[NB_FILE_OR_DIRECTORY_CAN_BE_SEND_IN_PARAMETERS];
	int nb_files = 0;
	int number_of_threads_by_child;
	int number_of_childs_can_be_active;
	char* filter_o = NULL;
	char* filter_r = NULL;

	for(int inc = 1; inc < argc; inc++)
	{
		switch (check_is_flag(argv[inc]))
		{
		case -1 : //flag inconnu, on test si c'est un fichier ou dossier
			if(isFile(argv[inc]) == 1)
			{
				if(isDirectory(argv[inc]))
					directories[nb_directories++] = argv[inc];
				else
					files[nb_files++] = argv[inc];
			}
			else // Si c'est ni un dossier ni un fichier
				printf("\nERROR : FLAG ISN'T RECOGNIZED IN handling_flag() IN flag_handling.c\n");
			break;
		case 0 	://flag -h
		case 1	://flag --help
			print_help();
			return;
			break;
		case 2	://flag -n
			if((inc + 1) < argc) //on verifi qu'il y est bien quelquechose derrière le flag
			{
				if(isNumeric(argv[++inc])) // on verifi que ce quelquechose soit numérique
					number_of_threads_by_child = atoi(argv[inc]);
				else
					printf("ERROR : FLAG -n NEED A NUMBER (use -h or --help for more informations)\n");
			}
			else
				printf("ERROR : FLAG -n NEED A VALUE (use -h or --help for more informations\n");
			break;
		case 3	://flag -i
			if((inc + 1) < argc) //on verifi qu'il y est bien quelquechose derrière le flag
			{
				if(isNumeric(argv[++inc])) // on verifi que ce quelquechose soit numérique
					number_of_childs_can_be_active = atoi(argv[inc]);
				else
					printf("ERROR : FLAG -i NEED A NUMBER (use -h or --help for more informations)\n");
			}
			else
				printf("ERROR : FLAG -i NEED A VALUE (use -h or --help for more informations\n");
			break;
		case 4	://flag -o
			if((inc + 1) < argc) //on verifi qu'il y est bien quelquechose derrière le flag
				filter_o = argv[inc++];
			break;
		case 5 	://flag -r
			if((inc + 1) < argc) //on verifi qu'il y est bien quelquechose derrière le flag
				filter_r = argv[inc++];
			break;
		}
	}
	handling_brain(directories, nb_directories, files, nb_files, number_of_threads_by_child, number_of_childs_can_be_active, filter_o, filter_r);
}

void handling_brain(char* directories[], int nb_directories, char* files[], int nb_files, int number_of_threads_by_child, int number_of_childs_can_be_active, char* filter_o, char* filter_r)
{
	if(filter_r != NULL)
	{
		for(int inc = 0; inc < nb_files; inc++)
		{
			print_line_of_file_contain(files[inc], filter_r);
		}
		if(nb_directories > 0)
		{
			struct dirent *lecture;
    			DIR *rep;
			for(int inc = 0; inc < nb_directories; inc++)
			{
				rep = opendir(directories[inc]);
				while ((lecture = readdir(rep)))
					print_line_of_file_contain(lecture->d_name, filter_r);
				closedir(rep);
			}
		}
	}
	else if(filter_o != NULL)
	{
		for(int inc = 0; inc < nb_files; inc++)
		{
			print_file_contain(files[inc], filter_o);
		}
		if(nb_directories > 0)
		{
			struct dirent *lecture;
    			DIR *rep;
			for(int inc = 0; inc < nb_directories; inc++)
			{
				rep = opendir(directories[inc]);
				while ((lecture = readdir(rep)))
					print_file_contain(lecture->d_name, filter_r);
				closedir(rep);
			}
		}
	}
	else
	{
		for(int inc = 0; inc < nb_files; inc++)
		{
			print_file(files[inc]);
		}
		if(nb_directories > 0)
		{
			struct dirent *lecture;
    			DIR *rep;
    			char* path;
			for(int inc = 0; inc < nb_directories; inc++)
			{
				path=NULL;
				path=malloc(PATH_SIZE_MAX);
				printf("||||||%s\n", directories[inc] );
				strcat(path, directories[inc]);
				strcat(path, "/");
				printf("||||||%s\n", path);
				rep = opendir(directories[inc]);
				
				while ((lecture = readdir(rep)))
				{
					char* temp = NULL;
					temp=malloc(PATH_SIZE_MAX);
					strcat(temp, path);


					printf("TEST\n");
					*temp = *path;
					strcat(temp, lecture->d_name);
					print_file(temp);
					printf("----%s\n", path);
					printf("%s\n", temp);


					free(temp);
				}
				closedir(rep);
				free(path);
			}
			
		}
	}
}