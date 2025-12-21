#include <types/string.h>

struct string {
  char  *s;
  ui64  len;
  ui64  capacity;
};

string  *new_string(char *s)
{
  string  *str;

  str = calloc(1, sizeof(string));
  if (!str)
    return (NULL);

  str->len = stringlen(s);
  str->capacity = str->len;
  str->s = calloc(1, str->len + 1);

  if (!str->s)
  {
    free(str);
    return (NULL);
  }
  memorycopy(str->s, s, str->len);
  return (str);
}

void  dealloc_string(string **str)
{
  if (!str || !*str)
    return ;
  free((*str)->s);
  memoryset(*str, 0, sizeof(string));
  free(*str);
  str = NULL;
}

ui64  get_string_len(const string *str)
{
  return (str->len);
}

void  print_string(int fd, const string *str)
{
  if (!str->s)
  {
    write(fd, "NULL", 4);
    return ;
  }
  write(fd, str->s, str->len);
}
