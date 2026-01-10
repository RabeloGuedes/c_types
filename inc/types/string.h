#ifndef TYPES_STRING_H
# define TYPES_STRING_H

# include <types/utils.h>
typedef enum {
    TYPE_STRING,
    TYPE_PCHAR,
    TYPE_CHAR,
    TYPE_INT,
    TYPE_LLONG
}   append_type;

typedef struct string string;
typedef struct string_metohods 
{
    string  *(*new)(char *);
    ui64    (*len)(const string *);
    void    (*write)(int, const string *);
    void    (*del)(string **);
    void    (*append)(string *, append_type, void *);
    string  *(*clone)(string *);
    void    (*to_lower)(string *);
    void    (*to_upper)(string *);
    int     (*index_of)(const string *, append_type, void *);
    int     (*last_index_of)(const string *, append_type, void *);
}   str_funcs;


string      *new_string(char *s);
int         equals_string(const string *, const char *);
str_funcs   *String(void);

#endif
