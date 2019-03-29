#include "../headers/my_printf.h"
#include <stdio.h>

int main (int argc, char *argv[])
{
	int test;
	test=2546;
	char test2[3] = "bjr";
	my_printf("%b\n", test);
	my_printf("%d\n", test);
	my_printf("%o\n", test);
	my_printf("%u\n", test);
	my_printf("%c\n", test);
	my_printf("%X\n", test);
	my_printf("%s\n", test2);
	my_printf("%%\n", test);
}