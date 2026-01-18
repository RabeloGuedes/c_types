#include <types/bigint.h>

struct bigint {
    ui32   *limbs;
    ui64    capacity;
    ui64    len;
    char    sign;
};

/// @brief This function expands the bigint.
/// @param b
void  expand_bigint_capacity(bigint *b)
{
  ui32  *limbs;

  if (!b)
    return ;
  limbs = calloc(b->capacity + 2, sizeof(ui32)); 
  if (!limbs)
    return ;
  memorycopy(limbs, b->limbs, b->capacity * sizeof(ui32));
  free(b->limbs);
  b->limbs = limbs;
  b->capacity += 2;
}

/// @brief This function performs the multiplication on the bigint.
/// @param b n
static void mul_uint_to_bigint(bigint *b, ui32 n)
{
  ui64  carry;
  ui32  i;
  ui64  prod;

  if (!b)
    return ;
  carry = 0;
  i = 0;
  while (i < b->len)
  {
    prod = (ui64)b->limbs[i] * n + carry;
    b->limbs[i] = (ui32)prod;
    carry = prod >> 32;
  }
  if (carry)
  {
    expand_bigint_capacity(b);
    b->limbs[b->len++] = (ui32)carry;
  }
}

/// @brief This function performs the addition on the bigint.
/// @param b n
static void add_uint_to_bigint(bigint *b, ui32 n)
{
  ui64  sum;
  ui64  carry;
  ui64  len;

  if (!b)
    return ;
  sum = (ui64)b->limbs[0] + n;
  carry = sum >> 32;
  len = 1;
  while (carry && len < b->capacity)
  {
    sum = (ui64)b->limbs[len] + carry;
    b->limbs[len] = (ui32)sum;
    carry = sum >> 32;
    len++;
  }

  if (carry)
  {
    expand_bigint_capacity(b);
    if (b->capacity > len)
      b->limbs[b->len++] = (ui32)carry;
  }
}

/// @brief This function instantiate a bigint from a string.
/// @param s
/// @return bigint
bigint  *new_bigint(char *s)
{
    ui64    len;
    ui64    pos;
    bigint  *b;
    ui64    acc;
    char    buff[CHUNK_DIGITS + 1];
    ui32    first_chunk;
    ui32    chunk;

    len = stringlen(s);
    pos = 0;
    acc = 0;
    b = calloc(1, sizeof(bigint));
    if (!b)
        return (NULL);
    if (!s)
    {
        b->limbs = calloc(1, sizeof(ui32));
        if (!b->limbs)
        {
            free(b);
            return (NULL);
        }
        b->capacity = 1;
        b->len = 1;
        return (b);
    }
    while (iswhitespace(*s))
        s++;
    if (*s == '-' || *s == '+')
        b->sign = *s++;
    if (!is_pchar_digit(s))
    {
      b->limbs = calloc(1, sizeof(ui32));
      if (!b->limbs)
      {
        free(b);
        return (NULL);
      }
      b->capacity = 1;
      b->len = 1;
      return (b);
    }
    first_chunk = len % CHUNK_DIGITS;    
    if (!first_chunk)
      first_chunk = CHUNK_DIGITS;
    while (pos < len)
    {
      acc = (pos == 0) ? first_chunk : CHUNK_DIGITS;
      
      memorycopy(buff, s + pos, acc);
      buff[acc] = '\0';

      chunk = (ui32)stringtoui64(buff);
      bigint_mul_small(b, CHUNK_BASE);
      bigint_add_small(b, chunk);

      pos += acc;
    }
    return (b);
}

/// @brief This function creates a new bigint from a long long.
/// @param l 
/// @return 
bigint  *new_bigint_llong(long long l)
{
    bigint  *b;

    b = calloc(1, sizeof(bigint));
    if (!b)
        return ;
    b->limbs = calloc(2, sizeof(ui32));
    if (!b->limbs)
    {
        free(b);
        return (NULL);
    }
    b->limbs[0] = l & 0xFFFFFFFFu;
    b->limbs[1] = l >> 32;
    b->len = b->limbs[1] ? 2 : 1;
    b->capacity = 2;
    b->sign = l < 0 ? -1 : 1;
    return (b);
}

/// @brief This function initialize the bigint by setting the internal variables to zero.
/// @param b 
void    init_bigint(bigint *b)
{
    if (!b)
        return ;
    if (b->limbs)
        free(b);
    b->limbs = NULL;
    b->capacity = 0;
    b->len = 0;
    b->sign = 1;
}

/// @brief This function returns the central and main
/// struct of pointer to functions for the bigint type.
/// @param  
/// @return 
bitint_funcs    *BigInt(void)
{
    static  bitint_funcs big_funcs;

    big_funcs.new = &new_bigint;
}
