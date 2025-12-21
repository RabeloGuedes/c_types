#include <types/utils.h>

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

ui64  stringlen(char *s)
{
  ui64  i;

  i = 0;
  while (s && s[i])
    i++;
  return (i);
}

void  memoryset(void *ptr, int c, ui64 bytes)
{
  unsigned char *p;

  p = ptr;
  if (!ptr || !bytes)
    return ;
  while (bytes--)
    *p++ = c;
}
