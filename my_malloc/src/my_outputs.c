#include <string.h>
#include <unistd.h>
#include "my_outputs.h"

void	my_putchar(char c)
{
	write(1, &c, 1);
}

void	my_putstr(char *str)
{
	write(1, str, strlen(str));
}

void	my_putnbr(int nb)
{
	int   div;

	div = 1;
	if (nb < 0)
	{
		my_putchar('-');
		nb *= -1;
	}
	while ((nb / div) >= 10)
	div *= 10;
	while (div > 0)
	{
		my_putchar((nb / div) % 10 + '0');
		div /= 10;
	}
}

void	my_putptr(unsigned long nb)
{
	if (!(nb))
		my_putstr("(null)");
	else
	{
		my_putchar('0');
		my_putchar('x');
		my_put_hex_nbr((unsigned int)nb);
	}
}

int	my_put_hex_nbr(unsigned int nb)
{
  	unsigned int	i;

  	i = my_put_neg(nb);
  	if (i > 15)
    		my_put_hex_nbr(i / 16);
  	my_put_hex((i % 16) + '0');
  	return (0);
}

void	my_put_hex(char c)
{
  	if (c > 9 + '0' && c < 16 + '0')
    	c = c - '0' - 10 + 'a';
  	write(1, &c, 1);
}

int	my_put_neg(int nb)
{
  	if (nb < 0)
    	{
      		my_putchar('-');
      		nb = nb * (-1);
    	}
  	return (nb);
}