#ifndef MY_PRINTF_H_INCLUDED
#define MY_PRINTF_H_INCLUDED


void 	my_putchar(const char caractere);
int	convert_base(int nb, int base);
int 	printf_option(const char *str, va_list args, ...);
int 	my_printf(const char *str, ...);
void 	my_putstr(char *str);

#endif