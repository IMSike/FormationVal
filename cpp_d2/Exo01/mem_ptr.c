#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mem_ptr.h"

void add_str ( const char* str1 , const char* str2 , char** res )
{
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if (len1 || len2) {
		*res = malloc(len1 + len2 + 1);
		memset(*res, 0, len1 + len2 + 1);
		strcpy(*res, str1);
		strcat(*res, str2);
	}
	else {
		*res = NULL;
	}
}

void add_str_struct ( str_op_t* str_op )
{
	add_str(str_op->str1, str_op->str2, &str_op->res);
}

static void test_add_str ( void )
{
	char * str1 = " Hey , " ;
	char * str2 = " it works ! " ;
	char * res ;

	add_str ( str1 , str2 , & res ) ;
	printf ( " %s \n " , res ) ;
}

static void test_add_str_struct ( void )
{
	char * str1 = " Hey , " ;
	char * str2 = " it works ! " ;
	str_op_t str_op ;

	str_op.str1 = str1 ;
	str_op.str2 = str2 ;
	add_str_struct(& str_op ) ;
	printf ( " %s \n " , str_op.res ) ;
}
