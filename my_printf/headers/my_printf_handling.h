#ifndef MY_PRINTF_HANDLING_H_INCLUDED
#define MY_PRINTF_HANDLING_H_INCLUDED

#include <stdarg.h>
#include "my_printf_def.h"

int	convert_base(int nb, int base);
void 	init(map_fct *ma_map);
void 	call(map_fct *ma_map, char c, int* inc, int* retour, va_list args);


#endif