#ifndef _FUNC_PTR_H_
#define _FUNC_PTR_H_

#include "func_ptr_enum.h"

void print_normal ( const char * str );
void print_reverse ( const char * str );
void print_upper ( const char * str );
void print_42 ( const char * str );
void do_action(action_t action, char *str);


typedef void (*ptrFonctionOnOptions)(const char*) ;
typedef struct mapForSelectAPrintfOption
{
	action_t key;
	ptrFonctionOnOptions fonc;
}		map_fct;

void init(map_fct* m);
void call(map_fct *ma_map, action_t action, const char* str);

#endif