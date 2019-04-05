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
/*
void 	my_putptr(void *ptr)
{
	unsigned char t[sizeof ptr];
        size_t i;

        memcpy(t, &ptr, sizeof ptr);
        for (i = 0; i < sizeof ptr; ++i)
        {
        	//my_putstr("%02x");
        	my_putchar(t[i]);
                //printf("%02x", t[i]);
        }
}
*/
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
int		my_put_hex_nbr(unsigned int nb)
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