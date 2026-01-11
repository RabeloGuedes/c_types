#ifndef TYPES_STRING_H
# define TYPES_STRING_H

# include <types/utils.h>

typedef struct string string;

typedef enum {
    TYPE_STRING,
    TYPE_PCHAR,
    TYPE_CHAR,
    TYPE_INT,
    TYPE_LLONG
}   append_type;

// Tagged union - bundles type and value together (type-safe)
typedef struct {
    append_type type;
    union {
        int         as_int;
        char        as_char;
        long long   as_llong;
        const char  *as_pchar;
        string      *as_str;
    };
}   typed_value;

// Helper macros - user can ONLY create valid type+value combinations
# define VAL_INT(n)    ((typed_value){TYPE_INT,    {.as_int = (n)}})
# define VAL_CHAR(c)   ((typed_value){TYPE_CHAR,   {.as_char = (c)}})
# define VAL_LLONG(l)  ((typed_value){TYPE_LLONG,  {.as_llong = (l)}})
# define VAL_PCHAR(s)  ((typed_value){TYPE_PCHAR,  {.as_pchar = (s)}})
# define VAL_STR(s)    ((typed_value){TYPE_STRING, {.as_str = (s)}})

typedef struct string_metohods 
{
    string  *(*new)(char *);
    ui64    (*len)(const string *);
    void    (*write)(int, const string *);
    void    (*del)(string **);
    void    (*append)(string *, typed_value);
    string  *(*clone)(string *);
    void    (*to_lower)(string *);
    void    (*to_upper)(string *);
    void    (*swap_case)(string *);
    int     (*index_of)(const string *, typed_value);
    int     (*last_index_of)(const string *, typed_value);
    int     (*is_null)(const string *);
    int     (*is_alpha)(const string *);
    int     (*is_alnum)(const string *);
    int     (*is_ascii)(const string *);
    int     (*is_digit)(const string *);
    int     (*is_decimal)(const string *);
    int     (*is_lower)(const string *);
    int     (*is_upper)(const string *);
    int     (*is_printable)(const string *);
    int     (*is_space)(const string *);
    int     (*is_title)(const string *);
    int     (*is_empty)(const string *);
    int     (*count)(const string *, typed_value);
    int     (*change)(string *, char *);
    int     (*compare)(const string *, typed_value);
    void    (*capitalize)(string *);
    string  *(*substring)(const string *, ui64, ui64);
    void    (*trim)(string *);
}   str_funcs;


string      *new_string(char *s);
int         equals_string(const string *, const char *);
str_funcs   *String(void);

#endif
