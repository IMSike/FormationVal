#ifndef MY_PRINTF_OPTIONS_H_INCLUDED
#define MY_PRINTF_OPTIONS_H_INCLUDED

void 	binary(int* inc, int* returnValue, va_list args);
void	integer(int* inc, int* returnValue, va_list args);
void	octalInteger(int* inc, int* returnValue, va_list args);
void	unsignedInteger(int* inc, int* returnValue, va_list args);
void	character(int* inc, int* returnValue, va_list args);
void	hexaInteger(int* inc, int* returnValue, va_list args);
void	string(int* inc, int* returnValue, va_list args);
void	percentCharacter(int* inc, int* returnValue, va_list args);

int 	printf_option(const char *str, va_list args, ...);

#endif