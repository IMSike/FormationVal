#ifndef MY_PRINTF_H_INCLUDED
#define MY_PRINTF_H_INCLUDED

#define NB_OPTIONS_PRINTF 8

#include <stdarg.h>

void 	my_putchar(const char character);
int 	printf_option(const char *str, va_list args, ...);
int 	my_printf(const char *str, ...);
void 	my_putstr(char *str);

typedef void (*ptrFonction)(int*, int*, va_list);

typedef struct map
{
	char key;
	ptrFonction fonc;
}		map_fct;

#endif