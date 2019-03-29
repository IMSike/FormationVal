#ifndef MY_PRINTF_H_INCLUDED
#define MY_PRINTF_H_INCLUDED

#define NB_OPTIONS_PRINTF 2

#include <stdarg.h>

void 	my_putchar(const char caractere);
int	convert_base(int nb, int base);
int 	printf_option(const char *str, va_list args, ...);
int 	my_printf(const char *str, ...);
void 	my_putstr(char *str);

void 	binaire(int* inc, int* retour, va_list args);
void	entier(int* inc, int* retour, va_list args);

typedef void (*ptrFonction)(int*, int*, va_list);

typedef struct map
{
	char key;
	ptrFonction fonc;
}		map_fct;

void init(map_fct *ma_map);
int call(map_fct *ma_map, char c, int* inc, int* retour, va_list args);


#endif