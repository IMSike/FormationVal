#include "my_malloc.h"
#include "blocks_management.h"

/*
int main(int argc, char const *argv[])
{
	print_alloc();
	char *test= malloc(20000000*sizeof(char));
	char *test2= malloc(2*sizeof(char));
	char *test3= malloc(2*sizeof(char));
	free(test);
	char *test4= malloc(2*sizeof(char));
	print_alloc();
	free(test2);
	free(test3);
	free(test4);
	print_alloc();
	//int teste = 123;
	//printf("test%d", teste);
}*/

static int no_alloc_strlen(const char *str)
{
	int i;
	for (i = 0; str && str[i]; i++);
	return (i);
}

static void no_alloc_putstr(const char *str)
{
	write(1, str, no_alloc_strlen(str));
}

int main(void)
{
	no_alloc_putstr("light malloc test : \n");
	for (int i = 0; i != 1000; i++){
		malloc(1);
	}
	no_alloc_putstr("medium malloc test : \n");
	for (int i = 0; i != 1000; i++){
		malloc(500);
	}
	no_alloc_putstr("heavy malloc test : \n");
	for (int i = 0; i != 1000; i++){
		malloc(1000000);
	}
	return (1);
}