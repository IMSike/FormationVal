#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_printf.h"

void	my_putchar(char caractere)
{
	write (1, &caractere, 1);
}

void	my_putstr(char *str)
{
	write(1,str, strlen(str));
}

int	convert_base(int nb, int base)
{
	const int BUFF_SIZE = 1024;
	char buff[BUFF_SIZE];
    	char lettre[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    	int cmp, temp, retour;

   	cmp = temp = retour = 0;

    	if(nb < 0)
    	    ++retour, my_putchar('-');

  	do
  	{
	        temp = nb % base;
	        temp = (temp < 0) ? -temp : temp;
	        buff[cmp] = lettre[temp];
	        ++cmp;
	        nb /= base;

	}while(nb != 0);
	for(cmp = cmp - 1; cmp >= 0; cmp--)
	{
	        my_putchar(buff[cmp]);
	        ++retour;
	}
	return retour;
}

void 	init(map_fct *ma_map)
{
	ptrFonction mes_fonctions[NB_OPTIONS_PRINTF] = {&binaire, &entier, &entierOctal, &entierNonSigne, &caractere, &entierHexa, &chaineDeCaracteres, &caracterePourcent};
	char mes_clefs[NB_OPTIONS_PRINTF] = {'b', 'd', 'o', 'u', 'c', 'X', 's', '%'};
	for(int inc = 0; inc < NB_OPTIONS_PRINTF; ++inc)
	{
		(ma_map + inc)->key = mes_clefs[inc];
		(ma_map + inc)->fonc = mes_fonctions[inc];
	}
}

int 	call(map_fct *ma_map, char c, int* inc, int* retour, va_list args)
{
	for(int i = 0; i < NB_OPTIONS_PRINTF; ++i)
	{
		if((ma_map + i)->key == c)
			(ma_map + i)->fonc(inc, retour, args);
	}
}

void 	binaire(int* inc, int* retour, va_list args)
{
	int entier = va_arg(args, int);
	retour += convert_base(entier, 2);
	++inc;
}

void 	entier(int* inc, int* retour, va_list args)
{
	int entier = va_arg(args, int);
	retour += convert_base(entier, 10);
	++inc;
}

void 	entierOctal(int* inc, int* retour, va_list args)
{
	int entier = va_arg(args, int);
	my_putchar('0');
	retour += convert_base(entier, 8) + 1;
	++inc;
}

void 	entierNonSigne(int* inc, int* retour, va_list args)
{
	unsigned int entier = va_arg(args, unsigned int);
	retour += convert_base(entier, 10);
	++inc;
}

void 	caractere(int* inc, int* retour, va_list args)
{
	char c = (unsigned char) va_arg(args, int);
	my_putchar(c);
	++inc, ++retour;
}

void 	entierHexa(int* inc, int* retour, va_list args)
{
	int entier = va_arg(args, int);
	my_putchar('0');
	my_putchar('x');
	retour += convert_base(entier, 16) + 2;
	++inc;
}

void 	chaineDeCaracteres(int* inc, int* retour, va_list args)
{
	char *s = va_arg(args, char *);
	for(; *s !=  '\0'; s++)
	{
		my_putchar(*s);
		++retour;
	}
	++inc;
}

void 	caracterePourcent(int* inc, int* retour, va_list args)
{
	my_putchar('%');
	++retour;
}

int 	printf_option(const char *str, va_list args, ...)
{
	int i, retour;
	i = retour = 0;
	map_fct *m = malloc(NB_OPTIONS_PRINTF * sizeof(map_fct));
	init(m);

	for(i = 0; i < strlen(str); ++i)
	{
		if(str[i]=='%' && i+1 < strlen(str))
		{
			++i;
			call(m,str[i],&i, &retour, args);
			//my_putchar(str[i]);
		}
		else
		{
			my_putchar(str[i]);
		        ++retour;
		}

	}
	free(m);
	return retour;
}

int    my_printf(const char *str, ...)
{
    	int res;
    	va_list ap;

    	va_start(ap, str);

    	res = printf_option(str, ap);

    	va_end(ap);

    	return res;
}