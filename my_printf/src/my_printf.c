#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_printf.h"
#include "my_printf_options.h"
#include "my_printf_handling.h"

void	my_putchar(char character)
{
	write (1, &character, 1);
}

void	my_putstr(char *str)
{
	write(1,str, strlen(str));
}

int 	printf_option(const char *str, va_list args, ...)
{
	int i, returnValue;
	i = returnValue = 0;
	map_fct *m = malloc(NB_OPTIONS_PRINTF * sizeof(map_fct));
	init(m);

	for(i = 0; i < (int)strlen(str); ++i)
	{
		if(str[i]=='%' && i+1 < (int)strlen(str))
		{
			++i;
			call(m,str[i],&i, &returnValue, args);
		}
		else
		{
			my_putchar(str[i]);
		        ++returnValue;
		}

	}
	free(m);
	return returnValue;
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