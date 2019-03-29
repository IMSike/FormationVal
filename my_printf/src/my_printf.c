#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/my_printf.h"
#include "../headers/my_printf_options.h"
#include "../headers/my_printf_handling.h"

void	my_putchar(char character)
{
	write (1, &character, 1);
}

void	my_putstr(char *str)
{
	write(1,str, strlen(str));
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