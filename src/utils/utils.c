#include <types/utils.h>
# include <stdio.h>

/// @brief Copies the bytes from the source to the destination.
/// @param dst 
/// @param src 
/// @param bytes 
void  memorycopy(void *dst, void *src, ui64 bytes)
{
  unsigned char *d;
  unsigned char *s;

  d = dst;
  s = src;
  if (!dst || !src || !bytes)
    return ;
  if (dst > src)
  {
    while (bytes--)
      d[bytes] = s[bytes];
  }
  else
  {
    while (bytes--)
      *d++ = *s++;
  }
}

/// @brief Caculates the number of characters from a pointer to char.
/// @param s 
/// @return unsigned long long. (i.e: 'stringlen("hello")-> 5')
ui64  stringlen(char *s)
{
  ui64  i;

  i = 0;
  while (s && s[i])
    i++;
  return (i);
}

/// @brief Sets the n bytes from the ptr address onwards to the second given argument.
/// @param ptr 
/// @param c 
/// @param bytes 
void  memoryset(void *ptr, int c, ui64 bytes)
{
  unsigned char *p;

  p = ptr;
  if (!ptr || !bytes)
    return ;
  while (bytes--)
    *p++ = c;
}

/// @brief Converts an integer to a pointer to char (ascii).
/// @param n 
/// @return pointer to char. (i.e: 'int_to_ascii(78)-> "78"')
char  *int_to_ascii(int n)
{
  char  *ptr;
  ui64  len;
  int   num;

  num = n;
  len = 0;
  if (n <= 0)
    len++;
  while (num)
  {
    len++;
    num /= 10;
  }
  ptr = calloc(len + 1, sizeof(char));
  if (!ptr)
    return (NULL);
  ptr[len--] = '\0';
  if (!n)
    ptr[0] = '0';
  if (n < 0)
    ptr[0] = '-';
  while (n)
  {
    ptr[len--] = ((n % 10) * ((n >> 31) | 1)) + 48;
    n /= 10;
  }
  return (ptr);
}

/// @brief Converts an long long to a pointer to char (ascii).
/// @param n 
/// @return pointer to char. (i.e: 'llong_to_ascii(4294967296)-> "4294967296"')
char  *llong_to_ascii(long long n)
{
  char        *ptr;
  ui64        len;
  long long   num;
  
  num = n;
  len = 0;
  if (n <= 0)
    len++;
  while (num)
  {
    len++;
    num /= 10;
  }
  ptr = calloc(len + 1, sizeof(char));
  if (!ptr)
    return (NULL);
  ptr[len--] = '\0';
  if (!n)
    ptr[0] = '0';
  if (n < 0)
    ptr[0] = '-';
  while (n)
  {
    ptr[len--] = ((n % 10) * ((n >> 31) | 1)) + 48;
    n /= 10;
  }
  return (ptr);
}