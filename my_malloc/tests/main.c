#include "my_malloc.h"
#include "blocks_management.h"

int main(int argc, char const *argv[])
{
	print_alloc();
	char *test= mon_malloc(2*sizeof(char));
	print_alloc();
	mon_free(test);
	print_alloc();
}