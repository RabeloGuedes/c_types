#ifndef BIG_INT_H
# define BIG_INT_H

# ifdef CHUNK_BASE 
#  undef CHUNK_BASE
#  define CHUNK_BASE 1000000000u 
# else
#  define CHUNK_BASE 1000000000u 
# endif

# ifdef CHUNK_DIGITS 
#  undef CHUNK_DIGITS
#  define CHUNK_DIGITS 9 
# else
#  define CHUNK_DIGITS 9 
# endif

# include <types/utils.h>

typedef struct bigint bigint;

typedef struct bitint_methods {
    bigint  *(*new)(char *);
    void    (*init)(bigint *);
}   bitint_funcs;


bitint_funcs    *BigInt(void);

#endif
