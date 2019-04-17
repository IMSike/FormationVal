#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "func_ptr.h"

#define NB_OPTIONS_ENUM 4

void print_normal ( const char* str )
{
	write(1,str, strlen(str));
	write(1,"\n", 1);
}

void print_reverse ( const char* str )
{
	char* tmp = strdup(str);
	int len = strlen(tmp);
	if(tmp)
		for(int inc = 0; inc < (len/2); inc++)
		{
			tmp[inc] = str[len - inc - 1];
			tmp[len - inc - 1] = str[inc];
		}
	print_normal(tmp);
}

void print_upper ( const char* str )
{
	char* tmp = strdup(str);
	for (int i = 0 ; tmp[i] != '\0' ; i++)
	{
		tmp[i] = toupper(tmp[i]);
	}
	print_normal(tmp);
}


void print_42 ( const char* str )
{
	const char *test = str;
	const char *test2 = test;
	test = test2;
	print_normal("42");
}

void do_action(action_t action, char *str)
{
	map_fct* m = malloc(NB_OPTIONS_ENUM * sizeof(map_fct));
	init(m);
	call(m, action, str);
}


void 	init(map_fct *ma_map)
{
	ptrFonctionOnOptions my_fonctions[NB_OPTIONS_ENUM+1] = {&print_normal, &print_reverse, &print_upper, &print_42};
	action_t my_keys[NB_OPTIONS_ENUM+1] = {PRINT_NORMAL, PRINT_REVERSE, PRINT_UPPER, PRINT_42};
	for(int inc = 0; inc < NB_OPTIONS_ENUM; ++inc)
	{
		(ma_map + inc)->key = my_keys[inc];
		(ma_map + inc)->fonc = my_fonctions[inc];
	}
}

void 	call(map_fct *ma_map, action_t action, const char* str)
{
	for(int i = 0; i < NB_OPTIONS_ENUM; ++i)
	{
		if((ma_map + i)->key == action)
			(ma_map + i)->fonc(str);
	}
}