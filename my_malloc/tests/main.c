#include "my_malloc.h"
#include "blocks_management.h"

int main(int argc, char const *argv[])
{
	print_alloc();
	char *test= malloc(2*sizeof(char));
	print_alloc();
	free(test);
	print_alloc();
}