#include "my_printf.h"
#include <stdio.h>

int main (int argc, char *argv[])
{
	int test;
	test=123;
	my_printf("hello world\n%d", test);
}