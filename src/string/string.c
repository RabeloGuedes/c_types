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
/// @param val typed_value containing type and value
void  append_to_string(string *str, typed_value val)
{
  if (!str)
    return ;
  switch (val.type)
  {
    case TYPE_STRING:
      append_str_to_string(str, val.as_str);
      break ;
    case TYPE_PCHAR:
      append_pchar_to_string(str, val.as_pchar);
      break ;
    case TYPE_CHAR:
      append_char_to_string(str, val.as_char);
      break ;
    case TYPE_INT:
      append_int_to_string(str, val.as_int);
      break ;
    case TYPE_LLONG:
      append_llong_to_string(str, val.as_llong);
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

/// @brief This function searches for a string inside another one and returns
/// the position where it starts in the searched string, if the search_oder
/// is negative it searches the last index of the given element.
/// @param str 
/// @param to_find 
/// @return 
int find_string(char *str, char *to_find, int search_order)
{
  int i;
  int j;
  int k;

  if (!str || !to_find)
    return (-1);
  if (search_order >= 0)
  {
  i = 0;
    while (str[i])
    {
      j = 0;
      while (str[i + j] && to_find[j] && str[i + j] == to_find[j])
      {
        if (!to_find[j + 1])
          return (i);
        j++;
      }
      i++;
    }
    return (-1);
  }
  else
  {
    i = stringlen(str) - 1;
    while (i > 0)
    {
      j = stringlen(to_find) - 1;
      k = 0;
      while (i + k >= 0 && j >= 0 && str[i + k] == to_find[j])
      {
        if (!j)
          return (i + k);
        j--;
        k--;
      }
      i--;
    }
    return (-1);
  }
}

char  *convert_types_to_pchar(typed_value val)
{
  char  *ptr;

  switch (val.type)
  {
    case TYPE_STRING:
      if (!val.as_str)
        return (NULL);
      ptr = calloc(val.as_str->capacity, sizeof(char));
      if (!ptr)
        return (NULL);
      memorycopy(ptr, val.as_str->s, val.as_str->capacity);
      break ;
    case TYPE_PCHAR:
      if (!val.as_pchar)
        return (NULL);
      ptr = calloc(stringlen((char *)val.as_pchar) + 1, sizeof(char));
      if (!ptr)
        return (NULL);
      memorycopy(ptr, (void *)val.as_pchar, stringlen((char *)val.as_pchar));
      break ;
    case TYPE_CHAR:
      ptr = calloc(2, sizeof(char));
      if (!ptr)
        return (NULL);
      *ptr = val.as_char;
      break ;
    case TYPE_INT:
      ptr = int_to_ascii(val.as_int);
      if (!ptr)
        return (NULL);
      break ;
    case TYPE_LLONG:
      ptr = llong_to_ascii(val.as_llong);
      if (!ptr)
        return (NULL);
      break ;
    default:
      return (NULL);
  }
  return (ptr);
}

/// @brief Returns the index of the first match of the given value argument.
/// @param str 
/// @param val typed_value containing type and value
/// @return integer
int  index_of_element(const string *str, typed_value val)
{
  char  *ptr;
  int   index;

  if (!str || !str->s)
    return (-1);
  ptr = convert_types_to_pchar(val);
  if (!ptr)
    return (-1);
  index = find_string(str->s, ptr, 1);
  free(ptr);
  return (index);
}

/// @brief Returns the index of the last match of the given value argument.
/// @param str 
/// @param val typed_value containing type and value
/// @return integer
int  last_index_of_element(const string *str, typed_value val)
{
  char  *ptr;
  int   index;

  if (!str || !str->s)
    return (-1);
  ptr = convert_types_to_pchar(val);
  if (!ptr)
    return (-1);
  index = find_string(str->s, ptr, -1);
  free(ptr);
  return (index);
}

/// @brief Verifies if the string or the internal pointer to char is NULL.
/// @param str 
/// @return 1 or 0
int is_string_null(const string *str)
{
  return (!str || !str->s);
}

/// @brief Verifies if all characters in the string are alphabetic.
/// @param str 
/// @return 1 or 0
int is_string_alpha(const string *str)
{
  char  *ptr;
  ui64  len;

  if (!str || !str->s)
    return (0);
  len = str->len;
  ptr = str->s;
  while (len--)
  {
    if ((*ptr < 'A' || *ptr > 'Z') && (*ptr < 'a' || *ptr > 'z'))
      return (0);
    ptr++;
  }
  return (1);
}

/// @brief Verifies if all characters in the string are alphanumeric.
/// @param str 
/// @return 1 or 0
int is_string_alnum(const string *str)
{
  char  *ptr;
  ui64  len;

  if (!str || !str->s)
    return (0);
  len = str->len;
  ptr = str->s;
  while (len--)
  {
    if ((*ptr < 'A' || *ptr > 'Z') && (*ptr < 'a' || *ptr > 'z') && (*ptr < '0' || *ptr > '9'))
      return (0);
    ptr++;
  }
  return (1);
}

/// @brief Verifies if all characters in the string are ASCII (0-127).
/// @param str 
/// @return 1 or 0
int is_string_ascii(const string *str)
{
  char  *ptr;
  ui64  len;

  if (!str || !str->s)
    return (0);
  len = str->len;
  ptr = str->s;
  while (len--)
  {
    if ((unsigned char)*ptr > 127)
      return (0);
    ptr++;
  }
  return (1);
}

/// @brief Verifies if the all characters are digits.
/// @param str 
/// @return 1 or 0
int is_string_digit(const string *str)
{
  char  *ptr;
  ui64  len;

  len = str->len;
  if (!str || !str->s)
    return (0);
  ptr = str->s;
  while (len--)
  {
    if (*ptr < '0' || *ptr > '9')
      return (0);
    ptr++;
  }
  return (1);
}

/// @brief Verifies if the string is decimal.
/// @param str 
/// @return 1 or 0
int is_string_decimal(const string *str)
{
  char  *ptr;
  ui64  len;
  char  dot_found;

  dot_found = 0;
  len = str->len;
  if (!str || !str->s)
    return (0);
  ptr = str->s;
  while (len--)
  {
    if (((*ptr < '0' || *ptr > '9') && *ptr != '.') || (dot_found && *ptr == '.'))
      return (0);
    if (!dot_found && *ptr == '.')
      dot_found = 1;
    ptr++;
  }
  return (1);
}

/// @brief Verifies if the string is composed only of lower case characters.
/// @param str 
/// @return 1 or 0
int is_string_lower(const string *str)
{
  char  *ptr;
  ui64  len;

  if (!str || !str->s)
    return (0);
  len = str->len;
  ptr = str->s;
  while (len--)
  {
    if (*ptr < 'a' || *ptr > 'z')
      return (0);
    ptr++;
  }
  return (1);
}

/// @brief Verifies if the string is composed only of upper case characters.
/// @param str 
/// @return 1 or 0
int is_string_upper(const string *str)
{
  char  *ptr;
  ui64  len;

  if (!str || !str->s)
    return (0);
  len = str->len;
  ptr = str->s;
  while (len--)
  {
    if (*ptr < 'A' || *ptr > 'Z')
      return (0);
    ptr++;
  }
  return (1);
}

/// @brief Verifies if the string is composed only of printable characters.
/// @param str 
/// @return 1 or 0
int is_string_printable(const string *str)
{
  char  *ptr;
  ui64  len;

  if (!str || !str->s)
    return (0);
  len = str->len;
  ptr = str->s;
  while (len--)
  {
    if ((*ptr < '\t' || *ptr > '\r') && (*ptr < ' ' || *ptr > '~'))
      return (0);
    ptr++;
  }
  return (1);
}

/// @brief Verifies if the string is composed only of white spaces characters.
/// @param str 
/// @return 1 or 0
int is_string_whitespaces(const string *str)
{
  char  *ptr;
  ui64  len;

  if (!str || !str->s)
    return (0);
  len = str->len;
  ptr = str->s;
  while (len--)
  {
    if ((*ptr < '\t' || *ptr > '\r') && *ptr != ' ')
      return (0);
    ptr++;
  }
  return (1);
}

/// @brief Verifies if the string is titled.
/// @param str 
/// @return 1 or 0
int is_string_title(const string *str)
{
  char  *ptr;
  ui64  len;
  char  was_separator;

  was_separator = 1;
  if (!str || !str->s)
    return (0);
  len = str->len;
  ptr = str->s;
  while (len--)
  {
    if (!was_separator && (*ptr >= 'A' && *ptr <= 'Z'))
      return (0);
    if ((*ptr >= '0' && *ptr <= '9')
      || (*ptr >= ',' && *ptr <= '/')
      || (*ptr >= '\t' && *ptr <= '\r')
      || *ptr == ' ')
      was_separator = 1;
    else
      was_separator = 0;
    ptr++;
  }
  return (1);
}

/// @brief Verifies if the string is empty.
/// @param str 
/// @return 1 or 0
int is_string_empty(const string *str)
{
  if (!str || !str->s)
    return (0);
  return (!str->len);
}

/// @brief This function counts how many times the 
/// @param str 
/// @param t 
/// @return integer
int  count_in_string(const string *str, typed_value t)
{
  char  *ptr;
  char  *s;
  int   count;
  int   pos;

  count = 0;
  pos = 0;
  if (!str || !str->s)
    return (0);
  ptr = convert_types_to_pchar(t);
  if (!ptr)
    return (-1);
  s = str->s;
  while (pos != -1)
  {
    pos = find_string(s, ptr, 1);
    if (pos != -1)
    {
      s += pos + 1;
      count++;
    }
  }
  free(ptr);
  return (count);
}

/// @brief This function swaps the case of the characters in the string.
/// @param str 
void  swap_string_case(string *str)
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
      *ptr = *ptr - ('a' - 'A');
    else if (*ptr >= 'A' && *ptr <= 'Z')
      *ptr = *ptr + ('a' - 'A');
    ptr++;
  }
}

/// @brief This function replaces the current string by the given char pointer.
/// @param str 
/// @param s 
/// @return 1 on success, 0 on failure
int  change_string(string *str, char *s)
{
  char  *ptr;
  ui64  len;

  if (!str)
    return (0);
  if (!s)
  {
    free(str->s);
    str->s = NULL;
    str->capacity = 0;
    str->len = 0;
    return (1);
  }
  len = stringlen(s);
  ptr = calloc(len + 1, sizeof(char));
  if (!ptr)
    return (0);
  memorycopy(ptr, s, len);
  free(str->s);
  str->s = ptr;
  str->len = len;
  str->capacity = str->len + 1;
  return (1);
}

/// @brief This function compares a string with the equivalent
/// string version of the given argument.
/// @param str 
/// @param to_compare 
/// @return integer
int compare_strings(const string *str, typed_value to_compare)
{
  char  *s;
  char  *ptr;
  ui64  len_s;
  ui64  len_t;

  if (!str || !str->s)
    return (0);
  ptr = convert_types_to_pchar(to_compare);
  if (!ptr)
    return (-1);
  s = str->s;
  len_s = str->len;
  len_t = stringlen(ptr);
  while (len_s-- && len_t--)
  {
    if (s[len_s] != ptr[len_t])
    {
      free(ptr);
      return (0);
    }
  }
  free(ptr);
  return (1);
}

/// @brief This function performs a transformation in the string
/// by capitalize the string.
/// @param str 
void  capitalize_string(string *str)
{
  char  *ptr;
  ui64  len;
  char  was_separator;

  was_separator = 1;
  if (!str || !str->s)
    return ;
  len = str->len;
  ptr = str->s;
  while (len--)
  {
    if (!was_separator && *ptr >= 'A' && *ptr <= 'Z')
      *ptr += ('a' - 'A');
    else if (was_separator && *ptr >= 'a' && *ptr <= 'z')
      *ptr -= 'a' - 'A';
    if ((*ptr >= '0' && *ptr <= '9')
      || (*ptr >= ',' && *ptr <= '/')
      || (*ptr >= '\t' && *ptr <= '\r')
      || *ptr == ' ')
      was_separator = 1;
    else
      was_separator = 0;
    ptr++;
  }
}

/// @brief This function creates a subtring of the given string.
/// @param str 
/// @param start 
/// @param end 
/// @return string
string  *substring(const string *str, ui64 start, ui64 end)
{
  string  *p;
  char    *ptr;
  char    *s;
  ui64    len;
  ui64    i;

  if (!str || !str->s || start >= end)
    return (NULL);
  if (end > str->len)
    end = str->len;
  len = end - start;
  p = new_string("");
  if (!p)
    return (NULL);
  ptr = calloc(len + 1, sizeof(char));
  if (!ptr)
  {
    dealloc_string(&p);
    return (NULL);
  }
  s = str->s;
  i = 0;
  while (i < len)
  {
    ptr[i] = s[start + i];
    i++;
  }
  free(p->s);
  p->s = ptr;
  p->len = len;
  p->capacity = len + 1;
  return (p);
}

/// @brief This function trims the string.
/// @param str 
void  trim_string(string *str)
{
  char  *ptr;
  ui64  start;
  ui64  end;
  ui64  new_len;
  ui64  i;

  if (!str || !str->s)
    return ;
  start = 0;
  end = str->len;
  ptr = str->s;
  while (start < end && ((ptr[start] >= '\t' && ptr[start] <= '\r') || ptr[start] == ' '))
    start++;
  if (start == end)
  {
    str->s[0] = '\0';
    str->len = 0;
    return ;
  }
  while (end > start && ((ptr[end - 1] >= '\t' && ptr[end - 1] <= '\r') || ptr[end - 1] == ' '))
    end--;
  new_len = end - start;
  ptr = calloc(new_len + 1, sizeof(char));
  if (!ptr)
    return ;
  i = 0;
  while (i < new_len)
  {
    ptr[i] = str->s[start + i];
    i++;
  }
  free(str->s);
  str->s = ptr;
  str->len = new_len;
  str->capacity = new_len + 1;
}

/// @brief This function set the string internal variables to zero.
/// @param str 
void  init_string(string *str)
{
  if (!str)
    return ;
  str->s = NULL;
  str->capacity = 0;
  str->len = 0;
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
  string_functions.index_of = &index_of_element;
  string_functions.last_index_of = &last_index_of_element;
  string_functions.is_null = &is_string_null;
  string_functions.is_alpha = &is_string_alpha;
  string_functions.is_alnum = &is_string_alnum;
  string_functions.is_ascii = &is_string_ascii;
  string_functions.is_digit = &is_string_digit;
  string_functions.is_decimal = &is_string_decimal;
  string_functions.is_lower = &is_string_lower;
  string_functions.is_upper = &is_string_upper;
  string_functions.is_printable = &is_string_printable;
  string_functions.is_space = &is_string_whitespaces;
  string_functions.is_title = &is_string_title;
  string_functions.is_empty = &is_string_empty;
  string_functions.count = &count_in_string;
  string_functions.swap_case = &swap_string_case;
  string_functions.change = &change_string;
  string_functions.compare = &compare_strings;
  string_functions.capitalize = &capitalize_string;
  string_functions.substring = &substring;
  string_functions.trim = &trim_string;
  string_functions.init = &init_string;
  return (&string_functions);
}
