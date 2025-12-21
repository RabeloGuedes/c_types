#ifndef TYPES_STRING_H
# define TYPES_STRING_H

# include <types/utils.h>

typedef struct string string;

string  *new_string(char *s);
ui64    get_string_len(const string *str);
void    print_string(int fd, const string *str);
void    dealloc_string(string **str);
#endif
