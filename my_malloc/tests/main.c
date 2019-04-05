#include "my_malloc.h"
#include "blocks_management.h"


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
}