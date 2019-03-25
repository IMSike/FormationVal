#include <stdarg.h>
#include <unistd.h>
#include <string.h>

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


int 	printf_option(const char *str, va_list args, ...)
{
	int i, retour;
	i = retour = 0;

	for(i = 0; str[i] !=  '\0'; ++i)
	{
	        switch(str[i])
	        {
	        case '%':
	        	++i;
	            	switch(str[i])
	            	{
	            	case 'b':
	            	{
	                	int entier = va_arg(args, int);
	                	retour += convert_base(entier, 2);
	                	++i;
	            	}
	                break;

	        	case 'o':
	            	{
		                int entier = va_arg(args, int);
		                my_putchar('0');
		                retour += convert_base(entier, 8) + 1;
		                ++i;
	            	}
	                break;

	            	case 'i':
	            	case 'd':
	            	{
		                int entier = va_arg(args, int);
		                retour += convert_base(entier, 10);
		                ++i;
	            	}
	                break;

		        case 'u':
		        {
		                unsigned int entier = va_arg(args, unsigned int);
		                retour += convert_base(entier, 10);
		                ++i;
	            	}
	                break;

	            	case 'X':
	            	{
		                int entier = va_arg(args, int);
		                my_putchar('0');
		                my_putchar('x');
		                retour += convert_base(entier, 16) + 2;
		                ++i;
	            	}
	                break;

	            	case 'c':
	            	{
		                char c = (unsigned char) va_arg(args, int);
		                my_putchar(c);
		                ++i, ++retour;
	            	}
	                break;

	           	case 's':
	            	{
		                char *s = va_arg(args, char *);
		                for(; *s !=  '\0'; s++)
		                {
		                	my_putchar(*s);
		                    	++retour;
		                }

		                ++i;
	            	}
	                break;

	            	case '%':
		                my_putchar('%');
		                ++retour;
	                break;

	            	default:
		                my_putchar('%');
		                ++retour;
	                break;
	            	}
	        default:
		        my_putchar(str[i]);
		        ++retour;
	        break;
	        }
	}
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