#include <types/string.h>

struct string {
  char  *s;
  ui64  len;
  ui64  capacity;
};

/// @brief This function aims to initialize a new string by using the pointer to char passed as parameter.
/// @param s 
/// @return string (i.e: 'new_string("hello")-> string(hello)')
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

/// @brief Takes a pointer to a pointer to a string and deallocates the internal string,
/// set the memory to zero and the pointer to pointer to string to NULL. This allows to
/// avoid segmentation faults due to read after free or double free. It can still segfaults
/// if deferencing after calling this function.
/// @param str 
void  dealloc_string(string **str)
{
  if (!str || !*str)
    return ;
  free((*str)->s);
  memoryset(*str, 0, sizeof(string));
  free(*str);
  *str = NULL;
}

/// @brief Reads the length of the string.
/// @param str 
/// @return unsigned long long. (i.e: 'get_string_len(string("hello"))-> 5')
ui64  get_string_len(const string *str)
{
  if (!str)
    return (0);
  return (str->len);
}

/// @brief It writes the given string into the file descriptor given as argument.
/// @param fd 
/// @param str 
void  print_string(int fd, const string *str)
{
  if (!str || !str->s)
  {
    write(fd, "NULL", 4);
    return ;
  }
  write(fd, str->s, str->len);
}

/// @brief This function concatenated a string to another.
/// @param str 
/// @param to_append 
/// @attention i.e: 'append_str_to_string(string("hello "), string("world"))-> "hello world"'
void    append_str_to_string(string *str, string *to_append)
{
  char  *ptr;
  ui64  total_len;

  total_len = str->len + to_append->len;
  if (!str || !to_append)
    return ;
  if (!to_append->s || to_append->len == 0 || total_len < str->len)
    return;
  if (str->capacity <= total_len)
  {
    ptr = realloc(str->s, total_len + 1);
    if (!ptr)
      return;
    str->s = ptr;
    str->capacity = total_len;
  }
  memorycopy(str->s + str->len, to_append->s, to_append->len);
  str->len = total_len;
  str->s[total_len] = '\0';
}

/// @brief This function concatenated a pointer to char to a string.
/// @param str 
/// @param to_append 
/// @attention i.e: 'append_str_to_string(string("hello "), "world")-> "hello world"'
void    append_pchar_to_string(string *str, const char *to_append)
{
  ui64  add_len;
  ui64  total_len;
  char  *ptr;

  if (!str || !to_append)
    return ;
  add_len = stringlen((char *)to_append);
  if (!add_len)
    return ;
  total_len = str->len + add_len;
  if (total_len < str->len)
    return ;
  if (str->capacity <= total_len)
  {
    ptr = realloc(str->s, total_len + 1);
    if (!ptr)
      return;
    str->s = ptr;
    str->capacity = total_len;
  }
  memorycopy(str->s + str->len, (void *)to_append, add_len);
  str->len = total_len;
  str->s[total_len] = '\0';
}

/// @brief This function concatenated a single character to a string.
/// @param str 
/// @param to_append 
/// @attention i.e: 'append_str_to_string(string("hello"), !)-> "hello!"'
void  append_char_to_string(string *str, char c)
{
  char  *ptr;
  void  *src;

  if (!str || str->len + 1 < str->len)
    return ;
  if (str->capacity <= str->len + 1)
  {
    ptr = realloc(str->s, str->len + 1);
    if (!ptr)
      return ;
    str->s = ptr;
    str->capacity = str->len + 1;
  }
  src = &c;
  memorycopy(str->s + str->len, src, 1);
  str->len += 1;
  str->s[str->len] = '\0';
}

/// @brief This function concatenated an integer to a string.
/// @param str 
/// @param to_append 
/// @attention i.e: 'append_str_to_string(string("hello "), 1337)-> "hello 1337"'
void  append_int_to_string(string *str, int n)
{
  char  *ptr;
  char  *num;
  ui64  add_len;
  ui64  total_len;

  if (!str)
    return ;
  num = int_to_ascii(n);
  if (!num)
    return ;
  add_len = stringlen(num);
  total_len = str->len + add_len;
  if (!add_len || total_len < str->len)
  {
    free(num);
    return ;
  }
  if (str->capacity <= total_len)
  {
    ptr = realloc(str->s, total_len + 1);
    if (!ptr)
    {
      free(num);
      return ;
    }
    str->s = ptr;
    str->capacity = total_len;
  }
  memorycopy(str->s + str->len, (void *)num, add_len);
  free(num);
  str->len = total_len;
  str->s[total_len] = '\0';
}

/// @brief This function concatenated a long long to a string.
/// @param str 
/// @param to_append 
/// @attention i.e: 'append_str_to_string(string("hello "), 4294967296)-> "hello 4294967296"'
void  append_llong_to_string(string *str, long long l)
{
  char  *ptr;
  char  *num;
  ui64  add_len;
  ui64  total_len;

  if (!str)
    return ;
  num = llong_to_ascii(l);
  if (!num)
    return ;
  add_len = stringlen(num);
  total_len = str->len + add_len;
  if (!add_len || total_len < str->len)
  {
    free(num);
    return ;
  }
  if (str->capacity <= total_len)
  {
    ptr = realloc(str->s, total_len + 1);
    if (!ptr)
    {
      free(num);
      return ;
    }
    str->s = ptr;
    str->capacity = total_len;
  }
  memorycopy(str->s + str->len, (void *)num, add_len);
  free(num);
  str->len = total_len;
  str->s[total_len] = '\0';
}

/// @brief Appends the given value argument into the string.
/// @param str 
/// @param type 
/// @param value
void  append_to_string(string *str, append_type type, void *value)
{
  if (!str || !value)
    return ;
  switch (type)
  {
    case TYPE_STRING:
      append_str_to_string(str, (string *)value);
      break ;
    case TYPE_PCHAR:
      append_pchar_to_string(str, *(const char **)value);
      break ;
    case TYPE_CHAR:
      append_char_to_string(str, *(char *)value);
      break ;
    case TYPE_INT:
      append_int_to_string(str, *(int *)value);
      break ;
    case TYPE_LLONG:
      append_llong_to_string(str, *(long long *)value);
      break ;
    default:
        break ;
  }
}

/// @brief Compares the string content with a pointer to char.
/// @param str 
/// @param cmp 
/// @return int (1 if equal, 0 if not equal)
int  equals_string(const string *str, const char *cmp)
{
  ui64  cmp_len;
  ui64  i;

  if (!str || !cmp)
    return (!str && !cmp);
  if (!str->s)
    return (0);
  cmp_len = stringlen((char *)cmp);
  if (str->len != cmp_len)
    return (0);
  i = 0;
  while (i < str->len)
  {
    if (str->s[i] != cmp[i])
      return (0);
    i++;
  }
  return (1);
}

/// @brief Creates a exactly deep copy of the given string.
/// @param str 
/// @return string (i.e: 'copy_string(string("hello"))-> string(hello)')
string  *copy_string(string *str)
{
  string  *ptr;

  if (!str || !str->s)
    return (NULL);
  ptr = calloc(1, sizeof(string));
  if (!ptr)
    return (NULL);
  ptr->capacity = str->capacity;
  ptr->len = str->len;
  ptr->s = calloc(ptr->capacity, sizeof(char));
  if (!ptr->s)
  {
    free(ptr);
    return (NULL);
  }
  memorycopy(ptr->s, str->s, str->len);
  return (ptr);
}

/// @brief Converts all upper case character to lower case.
/// @param str 
void  lower_string(string *str)
{
  char  *ptr;
  ui64  len;

  if (!str || !str->s)
    return ;
  ptr = str->s;
  len = str->len;
  while (len--)
  {
    if (*ptr >= 'A' && *ptr <= 'Z')
      *ptr = *ptr + 'a' - 'A';
    ptr++;
  }  
}

/// @brief Converts all lower case character to upper case.
/// @param str 
void  upper_string(string *str)
{
  char  *ptr;
  ui64  len;

  if (!str || !str->s)
    return ;
  ptr = str->s;
  len = str->len;
  while (len--)
  {
    if (*ptr >= 'a' && *ptr <= 'z')
      *ptr = *ptr + 'A' - 'a';
    ptr++;
  }  
}

/// @brief This function returns a struct with all functions that
/// can be used with the string type.
/// @param  
/// @return str_funcs
str_funcs   *String(void)
{
  static  str_funcs string_functions;

  string_functions.len = &get_string_len;
  string_functions.write = &print_string;
  string_functions.del = &dealloc_string;
  string_functions.new = &new_string;
  string_functions.append = &append_to_string;
  string_functions.clone = &copy_string;
  string_functions.to_lower = &lower_string;
  string_functions.to_upper = &upper_string;
  return (&string_functions);
}
