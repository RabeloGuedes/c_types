#ifndef TYPES_UTILS_H
# define TYPES_UTILS_H

# include <stdlib.h>
# include <unistd.h>

typedef unsigned long long ui64;

void  memorycopy(void *dst, void *src, ui64 bytes);
ui64  stringlen(char *s);
void  memoryset(void *ptr, int c, ui64 bytes);
char  *int_to_ascii(int n);
char  *llong_to_ascii(long long n);

#endif
