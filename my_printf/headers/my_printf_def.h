#ifndef MY_PRINTF_DEF_H_INCLUDED
#define MY_PRINTF_DEF_H_INCLUDED

#define NB_OPTIONS_PRINTF 8

typedef void (*ptrFonctionOnOptionsOfMyPrintf)(int*, int*, va_list);
typedef struct mapForSelectAPrintfOption
{
	char key;
	ptrFonctionOnOptionsOfMyPrintf fonc;
}		map_fct;

#endif