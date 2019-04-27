#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include "crawler.h"

const char* flags_list[NB_FLAGS]=  {"-h", "--help", "-n", "-i"}; //si rajout de flag : modifier NB_FLAGS dans flag_handling.h

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
	int number_of_threads_by_child = 0;
	int number_of_childs_can_be_active = 0;

	for(int inc = 1; inc < argc; inc++)
	{
		switch (check_is_flag(argv[inc]))
		{
		case -1 :	//flag non reconu
			printf("\nERROR : FLAG ISN'T RECOGNIZED IN handling_flag() IN flag_handling.c\n");
			break;
		case 0 	:	//flag -h
		case 1	:	//flag --help
			print_help();
			return;
			break;
		case 2	:	//flag -n
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
		case 3	:	//flag -i
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
		}
	}
	handling_inputs(number_of_threads_by_child, number_of_childs_can_be_active);
}