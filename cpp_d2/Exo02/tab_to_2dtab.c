#include <stdlib.h>
#include <stdio.h>

void tab_to_2dtab ( const int* tab , int length , int width , int*** res )
{
	*res = malloc(length * sizeof(int*));
	for(int inc = 0; inc < length; inc++)
	{
		(*res)[inc] = malloc(width * sizeof(int*));
		for(int inc2 = 0; inc2 < width; inc2++)
			(*res)[inc][inc2] = tab[inc*width+inc2];
	}
}