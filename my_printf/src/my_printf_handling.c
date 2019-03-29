#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_printf.h"
#include "my_printf_options.h"
#include "my_printf_handling.h"

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
	ptrFonctionOnOptionsOfMyPrintf mes_fonctions[NB_OPTIONS_PRINTF] = {&binary, &integer, &octalInteger, &unsignedInteger, &character, &hexaInteger, &string, &percentCharacter};
	char mes_clefs[NB_OPTIONS_PRINTF] = {'b', 'd', 'o', 'u', 'c', 'X', 's', '%'};
	for(int inc = 0; inc < NB_OPTIONS_PRINTF; ++inc)
	{
		(ma_map + inc)->key = mes_clefs[inc];
		(ma_map + inc)->fonc = mes_fonctions[inc];
	}
}

void 	call(map_fct *ma_map, char c, int* inc, int* retour, va_list args)
{
	for(int i = 0; i < NB_OPTIONS_PRINTF; ++i)
	{
		if((ma_map + i)->key == c)
			(ma_map + i)->fonc(inc, retour, args);
	}
}