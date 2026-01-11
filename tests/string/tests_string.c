#include <types/string.h>
#include "../test_framework.h"
#include <fcntl.h>

// ============================================================================
// Test Functions for String()->new
// ============================================================================

void test_new_basic(void)
{
    string *s = String()->new("hello");
    ASSERT_NOT_NULL(s);
    ASSERT_EQ(String()->len(s), 5);
    ASSERT(equals_string(s, "hello"));
    String()->del(&s);
}

void test_new_empty(void)
{
    string *s = String()->new("");
    ASSERT_NOT_NULL(s);
    ASSERT_EQ(String()->len(s), 0);
    String()->del(&s);
}

void test_new_long_string(void)
{
    char long_str[1024];
    memset(long_str, 'a', 1023);
    long_str[1023] = '\0';
    
    string *s = String()->new(long_str);
    ASSERT_NOT_NULL(s);
    ASSERT_EQ(String()->len(s), 1023);
    ASSERT(equals_string(s, long_str));
    String()->del(&s);
}

void test_new_null(void)
{
    string *s = String()->new(NULL);
    // Should handle NULL gracefully (either return NULL or empty string)
    if (s != NULL)
        String()->del(&s);
}

// ============================================================================
// Test Functions for String()->len
// ============================================================================

void test_len_basic(void)
{
    string *s = String()->new("hello world");
    ASSERT_EQ(String()->len(s), 11);
    String()->del(&s);
}

void test_len_empty(void)
{
    string *s = String()->new("");
    ASSERT_EQ(String()->len(s), 0);
    String()->del(&s);
}

void test_len_null(void)
{
    // This should not crash
    String()->len(NULL);
}

// ============================================================================
// Test Functions for String()->del
// ============================================================================

void test_del_basic(void)
{
    string *s = String()->new("test");
    ASSERT_NOT_NULL(s);
    String()->del(&s);
    // After deletion, pointer should be handled
}

void test_del_null_ptr(void)
{
    String()->del(NULL);
}

void test_del_ptr_to_null(void)
{
    string *s = NULL;
    String()->del(&s);
}

void test_del_double_free(void)
{
    string *s = String()->new("test");
    String()->del(&s);
    // Second delete should not crash
    String()->del(&s);
}

// ============================================================================
// Test Functions for String()->write
// ============================================================================

void test_write_basic(void)
{
    string *s = String()->new("hello");
    // Write to /dev/null to avoid output during tests
    int fd = open("/dev/null", O_WRONLY);
    if (fd >= 0)
    {
        String()->write(fd, s);
        close(fd);
    }
    String()->del(&s);
}

void test_write_null(void)
{
    int fd = open("/dev/null", O_WRONLY);
    if (fd >= 0)
    {
        String()->write(fd, NULL);
        close(fd);
    }
}

void test_write_invalid_fd(void)
{
    string *s = String()->new("test");
    String()->write(-1, s);
    String()->del(&s);
}

// ============================================================================
// Test Functions for String()->append
// ============================================================================

void test_append_pchar(void)
{
    string *s = String()->new("hello");
    const char *suffix = " world";
    String()->append(s, VAL_PCHAR(suffix));
    ASSERT_EQ(String()->len(s), 11);
    ASSERT(equals_string(s, "hello world"));
    String()->del(&s);
}

void test_append_string(void)
{
    string *s1 = String()->new("hello");
    string *s2 = String()->new(" world");
    String()->append(s1, VAL_STR(s2));
    ASSERT_EQ(String()->len(s1), 11);
    ASSERT(equals_string(s1, "hello world"));
    String()->del(&s1);
    String()->del(&s2);
}

void test_append_char(void)
{
    string *s = String()->new("hell");
    char c = 'o';
    String()->append(s, VAL_CHAR(c));
    ASSERT_EQ(String()->len(s), 5);
    ASSERT(equals_string(s, "hello"));
    String()->del(&s);
}

void test_append_int(void)
{
    string *s = String()->new("num: ");
    int n = 42;
    String()->append(s, VAL_INT(n));
    ASSERT_EQ(String()->len(s), 7);
    ASSERT(equals_string(s, "num: 42"));
    String()->del(&s);
}

void test_append_int_negative(void)
{
    string *s = String()->new("num: ");
    int n = -42;
    String()->append(s, VAL_INT(n));
    ASSERT_EQ(String()->len(s), 8);
    ASSERT(equals_string(s, "num: -42"));
    String()->del(&s);
}

void test_append_llong(void)
{
    string *s = String()->new("big: ");
    long long n = 9223372036854775807LL;
    String()->append(s, VAL_LLONG(n));
    ASSERT_NOT_NULL(s);
    ASSERT(equals_string(s, "big: 9223372036854775807"));
    String()->del(&s);
}

void test_append_llong_negative(void)
{
    string *s = String()->new("neg: ");
    long long n = -9223372036854775807LL;
    String()->append(s, VAL_LLONG(n));
    ASSERT_NOT_NULL(s);
    ASSERT(equals_string(s, "neg: -9223372036854775807"));
    String()->del(&s);
}

void test_append_null_string(void)
{
    const char *suffix = " world";
    String()->append(NULL, VAL_PCHAR(suffix));
}

void test_append_null_value(void)
{
    string *s = String()->new("hello");
    String()->append(s, VAL_PCHAR(NULL));
    String()->del(&s);
}

void test_append_empty_pchar(void)
{
    string *s = String()->new("hello");
    const char *empty = "";
    String()->append(s, VAL_PCHAR(empty));
    ASSERT_EQ(String()->len(s), 5);
    ASSERT(equals_string(s, "hello"));
    String()->del(&s);
}

void test_append_multiple(void)
{
    string *s = String()->new("");
    const char *hello = "hello";
    const char *space = " ";
    const char *world = "world";
    
    String()->append(s, VAL_PCHAR(hello));
    String()->append(s, VAL_PCHAR(space));
    String()->append(s, VAL_PCHAR(world));
    
    ASSERT_EQ(String()->len(s), 11);
    ASSERT(equals_string(s, "hello world"));
    String()->del(&s);
}

void test_append_int_zero(void)
{
    string *s = String()->new("zero: ");
    int n = 0;
    String()->append(s, VAL_INT(n));
    ASSERT_EQ(String()->len(s), 7);
    ASSERT(equals_string(s, "zero: 0"));
    String()->del(&s);
}

// ============================================================================
// Test Functions for String() struct
// ============================================================================

void test_string_struct_not_null(void)
{
    str_funcs *funcs = String();
    ASSERT_NOT_NULL(funcs);
}

void test_string_struct_functions_not_null(void)
{
    str_funcs *funcs = String();
    ASSERT_NOT_NULL(funcs->new);
    ASSERT_NOT_NULL(funcs->len);
    ASSERT_NOT_NULL(funcs->write);
    ASSERT_NOT_NULL(funcs->del);
    ASSERT_NOT_NULL(funcs->append);
    ASSERT_NOT_NULL(funcs->clone);
    ASSERT_NOT_NULL(funcs->to_lower);
    ASSERT_NOT_NULL(funcs->to_upper);
    ASSERT_NOT_NULL(funcs->index_of);
    ASSERT_NOT_NULL(funcs->last_index_of);
}

void test_string_struct_singleton(void)
{
    str_funcs *funcs1 = String();
    str_funcs *funcs2 = String();
    ASSERT_EQ(funcs1, funcs2);  // Should return same static struct
}

// ============================================================================
// Test Functions for String()->clone
// ============================================================================

void test_clone_basic(void)
{
    string *s = String()->new("hello world");
    string *clone = String()->clone(s);
    ASSERT_NOT_NULL(clone);
    ASSERT_EQ(String()->len(clone), String()->len(s));
    ASSERT_NE(clone, s);  // Should be different pointers
    ASSERT(equals_string(clone, "hello world"));
    ASSERT(equals_string(s, "hello world"));  // Both have same content
    String()->del(&s);
    String()->del(&clone);
}

void test_clone_empty(void)
{
    string *s = String()->new("");
    string *clone = String()->clone(s);
    ASSERT_NOT_NULL(clone);
    ASSERT_EQ(String()->len(clone), 0);
    String()->del(&s);
    String()->del(&clone);
}

void test_clone_null(void)
{
    string *clone = String()->clone(NULL);
    ASSERT_NULL(clone);
}

void test_clone_independence(void)
{
    string *s = String()->new("original");
    string *clone = String()->clone(s);
    
    // Modify original
    const char *suffix = " modified";
    String()->append(s, VAL_PCHAR(suffix));
    
    // Clone should remain unchanged
    ASSERT_EQ(String()->len(clone), 8);
    ASSERT(equals_string(clone, "original"));
    ASSERT_EQ(String()->len(s), 17);
    ASSERT(equals_string(s, "original modified"));
    
    String()->del(&s);
    String()->del(&clone);
}

// ============================================================================
// Test Functions for String()->to_lower
// ============================================================================

void test_to_lower_basic(void)
{
    string *s = String()->new("HELLO WORLD");
    String()->to_lower(s);
    ASSERT_EQ(String()->len(s), 11);
    ASSERT(equals_string(s, "hello world"));
    String()->del(&s);
}

void test_to_lower_mixed(void)
{
    string *s = String()->new("HeLLo WoRLd");
    String()->to_lower(s);
    ASSERT_EQ(String()->len(s), 11);
    ASSERT(equals_string(s, "hello world"));
    String()->del(&s);
}

void test_to_lower_already_lower(void)
{
    string *s = String()->new("hello world");
    String()->to_lower(s);
    ASSERT_EQ(String()->len(s), 11);
    ASSERT(equals_string(s, "hello world"));
    String()->del(&s);
}

void test_to_lower_with_numbers(void)
{
    string *s = String()->new("HELLO123WORLD");
    String()->to_lower(s);
    ASSERT_EQ(String()->len(s), 13);
    ASSERT(equals_string(s, "hello123world"));
    String()->del(&s);
}

void test_to_lower_empty(void)
{
    string *s = String()->new("");
    String()->to_lower(s);
    ASSERT_EQ(String()->len(s), 0);
    String()->del(&s);
}

void test_to_lower_null(void)
{
    String()->to_lower(NULL);
}

// ============================================================================
// Test Functions for String()->to_upper
// ============================================================================

void test_to_upper_basic(void)
{
    string *s = String()->new("hello world");
    String()->to_upper(s);
    ASSERT_EQ(String()->len(s), 11);
    ASSERT(equals_string(s, "HELLO WORLD"));
    String()->del(&s);
}

void test_to_upper_mixed(void)
{
    string *s = String()->new("HeLLo WoRLd");
    String()->to_upper(s);
    ASSERT_EQ(String()->len(s), 11);
    ASSERT(equals_string(s, "HELLO WORLD"));
    String()->del(&s);
}

void test_to_upper_already_upper(void)
{
    string *s = String()->new("HELLO WORLD");
    String()->to_upper(s);
    ASSERT_EQ(String()->len(s), 11);
    ASSERT(equals_string(s, "HELLO WORLD"));
    String()->del(&s);
}

void test_to_upper_with_numbers(void)
{
    string *s = String()->new("hello123world");
    String()->to_upper(s);
    ASSERT_EQ(String()->len(s), 13);
    ASSERT(equals_string(s, "HELLO123WORLD"));
    String()->del(&s);
}

void test_to_upper_empty(void)
{
    string *s = String()->new("");
    String()->to_upper(s);
    ASSERT_EQ(String()->len(s), 0);
    String()->del(&s);
}

void test_to_upper_null(void)
{
    String()->to_upper(NULL);
}

// ============================================================================
// Test Functions for String()->index_of
// ============================================================================

void test_index_of_pchar_found(void)
{
    string *s = String()->new("hello world");
    const char *needle = "world";
    int idx = String()->index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, 6);
    String()->del(&s);
}

void test_index_of_pchar_not_found(void)
{
    string *s = String()->new("hello world");
    const char *needle = "foo";
    int idx = String()->index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_index_of_pchar_at_start(void)
{
    string *s = String()->new("hello world");
    const char *needle = "hello";
    int idx = String()->index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, 0);
    String()->del(&s);
}

void test_index_of_pchar_at_end(void)
{
    string *s = String()->new("hello world");
    const char *needle = "ld";
    int idx = String()->index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, 9);
    String()->del(&s);
}

void test_index_of_char_found(void)
{
    string *s = String()->new("hello world");
    char c = 'w';
    int idx = String()->index_of(s, VAL_CHAR(c));
    ASSERT_EQ(idx, 6);
    String()->del(&s);
}

void test_index_of_char_not_found(void)
{
    string *s = String()->new("hello world");
    char c = 'z';
    int idx = String()->index_of(s, VAL_CHAR(c));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_index_of_char_first(void)
{
    string *s = String()->new("hello world");
    char c = 'h';
    int idx = String()->index_of(s, VAL_CHAR(c));
    ASSERT_EQ(idx, 0);
    String()->del(&s);
}

void test_index_of_int_found(void)
{
    string *s = String()->new("value is 42 here");
    int n = 42;
    int idx = String()->index_of(s, VAL_INT(n));
    ASSERT_EQ(idx, 9);
    String()->del(&s);
}

void test_index_of_int_not_found(void)
{
    string *s = String()->new("value is 42 here");
    int n = 99;
    int idx = String()->index_of(s, VAL_INT(n));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_index_of_int_negative(void)
{
    string *s = String()->new("temp is -10 degrees");
    int n = -10;
    int idx = String()->index_of(s, VAL_INT(n));
    ASSERT_EQ(idx, 8);
    String()->del(&s);
}

void test_index_of_string(void)
{
    string *s = String()->new("hello world");
    string *needle = String()->new("world");
    int idx = String()->index_of(s, VAL_STR(needle));
    ASSERT_EQ(idx, 6);
    String()->del(&s);
    String()->del(&needle);
}

void test_index_of_string_not_found(void)
{
    string *s = String()->new("hello world");
    string *needle = String()->new("foo");
    int idx = String()->index_of(s, VAL_STR(needle));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
    String()->del(&needle);
}

void test_index_of_empty_string(void)
{
    string *s = String()->new("");
    const char *needle = "test";
    int idx = String()->index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_index_of_null_string(void)
{
    const char *needle = "test";
    int idx = String()->index_of(NULL, VAL_PCHAR(needle));
    ASSERT_EQ(idx, -1);
}

void test_index_of_null_value(void)
{
    string *s = String()->new("hello world");
    int idx = String()->index_of(s, VAL_PCHAR(NULL));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

// ============================================================================
// Test Functions for String()->last_index_of
// ============================================================================

void test_last_index_of_pchar_single(void)
{
    string *s = String()->new("hello world");
    const char *needle = "world";
    int idx = String()->last_index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, 6);
    String()->del(&s);
}

void test_last_index_of_pchar_multiple(void)
{
    string *s = String()->new("hello hello hello");
    const char *needle = "hello";
    int idx = String()->last_index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, 12);  // Last "hello" starts at index 12
    String()->del(&s);
}

void test_last_index_of_pchar_not_found(void)
{
    string *s = String()->new("hello world");
    const char *needle = "foo";
    int idx = String()->last_index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_last_index_of_pchar_at_start(void)
{
    string *s = String()->new("hello");
    const char *needle = "hello";
    int idx = String()->last_index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, 0);  // Only occurrence is at start
    String()->del(&s);
}

void test_last_index_of_pchar_at_end(void)
{
    string *s = String()->new("world world");
    const char *needle = "world";
    int idx = String()->last_index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, 6);  // Last "world" at index 6
    String()->del(&s);
}

void test_last_index_of_char_single(void)
{
    string *s = String()->new("hello");
    char c = 'e';
    int idx = String()->last_index_of(s, VAL_CHAR(c));
    ASSERT_EQ(idx, 1);
    String()->del(&s);
}

void test_last_index_of_char_multiple(void)
{
    string *s = String()->new("hello world");
    char c = 'l';
    int idx = String()->last_index_of(s, VAL_CHAR(c));
    ASSERT_EQ(idx, 9);  // Last 'l' is at index 9 in "world"
    String()->del(&s);
}

void test_last_index_of_char_not_found(void)
{
    string *s = String()->new("hello world");
    char c = 'z';
    int idx = String()->last_index_of(s, VAL_CHAR(c));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_last_index_of_int_single(void)
{
    string *s = String()->new("value is 42");
    int n = 42;
    int idx = String()->last_index_of(s, VAL_INT(n));
    ASSERT_EQ(idx, 9);
    String()->del(&s);
}

void test_last_index_of_int_multiple(void)
{
    string *s = String()->new("42 plus 42 equals 84");
    int n = 42;
    int idx = String()->last_index_of(s, VAL_INT(n));
    ASSERT_EQ(idx, 8);  // Last "42" at index 8
    String()->del(&s);
}

void test_last_index_of_int_not_found(void)
{
    string *s = String()->new("value is 42");
    int n = 99;
    int idx = String()->last_index_of(s, VAL_INT(n));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_last_index_of_string(void)
{
    string *s = String()->new("test test test");
    string *needle = String()->new("test");
    int idx = String()->last_index_of(s, VAL_STR(needle));
    ASSERT_EQ(idx, 10);  // Last "test" at index 10
    String()->del(&s);
    String()->del(&needle);
}

void test_last_index_of_empty_string(void)
{
    string *s = String()->new("");
    const char *needle = "test";
    int idx = String()->last_index_of(s, VAL_PCHAR(needle));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_last_index_of_null_string(void)
{
    const char *needle = "test";
    int idx = String()->last_index_of(NULL, VAL_PCHAR(needle));
    ASSERT_EQ(idx, -1);
}

void test_last_index_of_null_value(void)
{
    string *s = String()->new("hello world");
    int idx = String()->last_index_of(s, VAL_PCHAR(NULL));
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

// ============================================================================
// Test Functions for String()->is_null
// ============================================================================

void test_is_null_with_null_ptr(void)
{
    ASSERT_EQ(String()->is_null(NULL), 1);
}

void test_is_null_with_valid_string(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->is_null(s), 0);
    String()->del(&s);
}

void test_is_null_with_empty_string(void)
{
    string *s = String()->new("");
    ASSERT_EQ(String()->is_null(s), 0);
    String()->del(&s);
}

// ============================================================================
// Test Functions for String()->is_alpha
// ============================================================================

void test_is_alpha_all_lower(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->is_alpha(s), 1);
    String()->del(&s);
}

void test_is_alpha_all_upper(void)
{
    string *s = String()->new("HELLO");
    ASSERT_EQ(String()->is_alpha(s), 1);
    String()->del(&s);
}

void test_is_alpha_mixed_case(void)
{
    string *s = String()->new("HeLLo");
    ASSERT_EQ(String()->is_alpha(s), 1);
    String()->del(&s);
}

void test_is_alpha_with_numbers(void)
{
    string *s = String()->new("hello123");
    ASSERT_EQ(String()->is_alpha(s), 0);
    String()->del(&s);
}

void test_is_alpha_with_space(void)
{
    string *s = String()->new("hello world");
    ASSERT_EQ(String()->is_alpha(s), 0);
    String()->del(&s);
}

void test_is_alpha_with_symbols(void)
{
    string *s = String()->new("hello!");
    ASSERT_EQ(String()->is_alpha(s), 0);
    String()->del(&s);
}

void test_is_alpha_empty(void)
{
    string *s = String()->new("");
    // Empty string has no non-alpha characters, but also no alpha characters
    // Implementation may return 1 (vacuously true) or 0
    int result = String()->is_alpha(s);
    ASSERT(result == 0 || result == 1);
    String()->del(&s);
}

void test_is_alpha_null(void)
{
    ASSERT_EQ(String()->is_alpha(NULL), 0);
}

// ============================================================================
// Test Functions for String()->is_alnum
// ============================================================================

void test_is_alnum_all_alpha(void)
{
    string *s = String()->new("HelloWorld");
    ASSERT_EQ(String()->is_alnum(s), 1);
    String()->del(&s);
}

void test_is_alnum_all_digits(void)
{
    string *s = String()->new("1234567890");
    ASSERT_EQ(String()->is_alnum(s), 1);
    String()->del(&s);
}

void test_is_alnum_mixed(void)
{
    string *s = String()->new("Hello123World456");
    ASSERT_EQ(String()->is_alnum(s), 1);
    String()->del(&s);
}

void test_is_alnum_with_space(void)
{
    string *s = String()->new("Hello World");
    ASSERT_EQ(String()->is_alnum(s), 0);
    String()->del(&s);
}

void test_is_alnum_with_symbols(void)
{
    string *s = String()->new("hello@world");
    ASSERT_EQ(String()->is_alnum(s), 0);
    String()->del(&s);
}

void test_is_alnum_with_underscore(void)
{
    string *s = String()->new("hello_world");
    ASSERT_EQ(String()->is_alnum(s), 0);
    String()->del(&s);
}

void test_is_alnum_empty(void)
{
    string *s = String()->new("");
    int result = String()->is_alnum(s);
    ASSERT(result == 0 || result == 1);
    String()->del(&s);
}

void test_is_alnum_null(void)
{
    ASSERT_EQ(String()->is_alnum(NULL), 0);
}

// ============================================================================
// Test Functions for String()->is_ascii
// ============================================================================

void test_is_ascii_basic(void)
{
    string *s = String()->new("Hello, World!");
    ASSERT_EQ(String()->is_ascii(s), 1);
    String()->del(&s);
}

void test_is_ascii_all_printable(void)
{
    string *s = String()->new("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    ASSERT_EQ(String()->is_ascii(s), 1);
    String()->del(&s);
}

void test_is_ascii_special_chars(void)
{
    string *s = String()->new("!@#$%^&*()_+-=[]{}|;':\",./<>?");
    ASSERT_EQ(String()->is_ascii(s), 1);
    String()->del(&s);
}

void test_is_ascii_with_newline(void)
{
    string *s = String()->new("hello\nworld");
    ASSERT_EQ(String()->is_ascii(s), 1);
    String()->del(&s);
}

void test_is_ascii_with_tab(void)
{
    string *s = String()->new("hello\tworld");
    ASSERT_EQ(String()->is_ascii(s), 1);
    String()->del(&s);
}

void test_is_ascii_with_high_byte(void)
{
    // Create string with a non-ASCII character (value > 127)
    char non_ascii[] = "hello\x80world";
    string *s = String()->new(non_ascii);
    ASSERT_EQ(String()->is_ascii(s), 0);
    String()->del(&s);
}

void test_is_ascii_with_utf8(void)
{
    // UTF-8 encoded character (e.g., é = 0xC3 0xA9)
    char utf8[] = "caf\xc3\xa9";
    string *s = String()->new(utf8);
    ASSERT_EQ(String()->is_ascii(s), 0);
    String()->del(&s);
}

void test_is_ascii_empty(void)
{
    string *s = String()->new("");
    int result = String()->is_ascii(s);
    ASSERT(result == 0 || result == 1);
    String()->del(&s);
}

void test_is_ascii_null(void)
{
    ASSERT_EQ(String()->is_ascii(NULL), 0);
}

// ============================================================================
// Test Functions for String()->is_lower
// ============================================================================

void test_is_lower_all_lower(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->is_lower(s), 1);
    String()->del(&s);
}

void test_is_lower_with_upper(void)
{
    string *s = String()->new("Hello");
    ASSERT_EQ(String()->is_lower(s), 0);
    String()->del(&s);
}

void test_is_lower_all_upper(void)
{
    string *s = String()->new("HELLO");
    ASSERT_EQ(String()->is_lower(s), 0);
    String()->del(&s);
}

void test_is_lower_with_numbers(void)
{
    string *s = String()->new("hello123");
    ASSERT_EQ(String()->is_lower(s), 0);
    String()->del(&s);
}

void test_is_lower_with_space(void)
{
    string *s = String()->new("hello world");
    ASSERT_EQ(String()->is_lower(s), 0);
    String()->del(&s);
}

void test_is_lower_with_symbols(void)
{
    string *s = String()->new("hello!");
    ASSERT_EQ(String()->is_lower(s), 0);
    String()->del(&s);
}

void test_is_lower_empty(void)
{
    string *s = String()->new("");
    int result = String()->is_lower(s);
    ASSERT(result == 0 || result == 1);
    String()->del(&s);
}

void test_is_lower_null(void)
{
    ASSERT_EQ(String()->is_lower(NULL), 0);
}

// ============================================================================
// Test Functions for String()->is_upper
// ============================================================================

void test_is_upper_all_upper(void)
{
    string *s = String()->new("HELLO");
    ASSERT_EQ(String()->is_upper(s), 1);
    String()->del(&s);
}

void test_is_upper_with_lower(void)
{
    string *s = String()->new("HELLo");
    ASSERT_EQ(String()->is_upper(s), 0);
    String()->del(&s);
}

void test_is_upper_all_lower(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->is_upper(s), 0);
    String()->del(&s);
}

void test_is_upper_with_numbers(void)
{
    string *s = String()->new("HELLO123");
    ASSERT_EQ(String()->is_upper(s), 0);
    String()->del(&s);
}

void test_is_upper_with_space(void)
{
    string *s = String()->new("HELLO WORLD");
    ASSERT_EQ(String()->is_upper(s), 0);
    String()->del(&s);
}

void test_is_upper_with_symbols(void)
{
    string *s = String()->new("HELLO!");
    ASSERT_EQ(String()->is_upper(s), 0);
    String()->del(&s);
}

void test_is_upper_empty(void)
{
    string *s = String()->new("");
    int result = String()->is_upper(s);
    ASSERT(result == 0 || result == 1);
    String()->del(&s);
}

void test_is_upper_null(void)
{
    ASSERT_EQ(String()->is_upper(NULL), 0);
}

// ============================================================================
// Test Functions for String()->is_printable
// ============================================================================

void test_is_printable_basic(void)
{
    string *s = String()->new("Hello, World!");
    ASSERT_EQ(String()->is_printable(s), 1);
    String()->del(&s);
}

void test_is_printable_all_printable(void)
{
    string *s = String()->new("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    ASSERT_EQ(String()->is_printable(s), 1);
    String()->del(&s);
}

void test_is_printable_special_chars(void)
{
    string *s = String()->new("!@#$%^&*()_+-=[]{}|;':\",./<>?");
    ASSERT_EQ(String()->is_printable(s), 1);
    String()->del(&s);
}

void test_is_printable_with_space(void)
{
    string *s = String()->new("hello world");
    ASSERT_EQ(String()->is_printable(s), 1);
    String()->del(&s);
}

void test_is_printable_with_tab(void)
{
    string *s = String()->new("hello\tworld");
    ASSERT_EQ(String()->is_printable(s), 1);
    String()->del(&s);
}

void test_is_printable_with_newline(void)
{
    string *s = String()->new("hello\nworld");
    ASSERT_EQ(String()->is_printable(s), 1);
    String()->del(&s);
}

void test_is_printable_with_carriage_return(void)
{
    string *s = String()->new("hello\rworld");
    ASSERT_EQ(String()->is_printable(s), 1);
    String()->del(&s);
}

void test_is_printable_with_null_char(void)
{
    // String with embedded NUL (only first part will be stored)
    char str[] = "hello\0world";
    string *s = String()->new(str);
    // Should be printable since the string stops at \0
    ASSERT_EQ(String()->is_printable(s), 1);
    String()->del(&s);
}

void test_is_printable_with_bell(void)
{
    // Bell character (0x07) is not printable
    char str[] = "hello\x07world";
    string *s = String()->new(str);
    ASSERT_EQ(String()->is_printable(s), 0);
    String()->del(&s);
}

void test_is_printable_with_escape(void)
{
    // Escape character (0x1B) is not printable
    char str[] = "hello\x1Bworld";
    string *s = String()->new(str);
    ASSERT_EQ(String()->is_printable(s), 0);
    String()->del(&s);
}

void test_is_printable_with_del(void)
{
    // DEL character (0x7F) is not printable
    char str[] = "hello\x7Fworld";
    string *s = String()->new(str);
    ASSERT_EQ(String()->is_printable(s), 0);
    String()->del(&s);
}

void test_is_printable_empty(void)
{
    string *s = String()->new("");
    int result = String()->is_printable(s);
    ASSERT(result == 0 || result == 1);
    String()->del(&s);
}

void test_is_printable_null(void)
{
    ASSERT_EQ(String()->is_printable(NULL), 0);
}

// ============================================================================
// Test Functions for String()->is_title
// ============================================================================

void test_is_title_basic(void)
{
    string *s = String()->new("Hello World");
    ASSERT_EQ(String()->is_title(s), 1);
    String()->del(&s);
}

void test_is_title_all_lower(void)
{
    string *s = String()->new("hello world");
    ASSERT_EQ(String()->is_title(s), 1);
    String()->del(&s);
}

void test_is_title_all_upper(void)
{
    // "HELLO WORLD" - H is after separator (start), but E is uppercase after H
    string *s = String()->new("HELLO WORLD");
    ASSERT_EQ(String()->is_title(s), 0);
    String()->del(&s);
}

void test_is_title_mid_word_upper(void)
{
    // "HeLLo" - e is lowercase, L is uppercase mid-word = not title
    string *s = String()->new("HeLLo");
    ASSERT_EQ(String()->is_title(s), 0);
    String()->del(&s);
}

void test_is_title_camelCase(void)
{
    // camelCase has uppercase in middle of word
    string *s = String()->new("camelCase");
    ASSERT_EQ(String()->is_title(s), 0);
    String()->del(&s);
}

void test_is_title_with_numbers(void)
{
    // Numbers act as separators, so "Hello2World" is valid
    string *s = String()->new("Hello2World");
    ASSERT_EQ(String()->is_title(s), 1);
    String()->del(&s);
}

void test_is_title_with_comma(void)
{
    // Comma acts as separator
    string *s = String()->new("Hello,World");
    ASSERT_EQ(String()->is_title(s), 1);
    String()->del(&s);
}

void test_is_title_with_slash(void)
{
    // Slash acts as separator
    string *s = String()->new("Hello/World");
    ASSERT_EQ(String()->is_title(s), 1);
    String()->del(&s);
}

void test_is_title_with_tab(void)
{
    // Tab acts as separator
    string *s = String()->new("Hello\tWorld");
    ASSERT_EQ(String()->is_title(s), 1);
    String()->del(&s);
}

void test_is_title_single_word(void)
{
    string *s = String()->new("Hello");
    ASSERT_EQ(String()->is_title(s), 1);
    String()->del(&s);
}

void test_is_title_single_upper(void)
{
    string *s = String()->new("H");
    ASSERT_EQ(String()->is_title(s), 1);
    String()->del(&s);
}

void test_is_title_single_lower(void)
{
    string *s = String()->new("h");
    ASSERT_EQ(String()->is_title(s), 1);
    String()->del(&s);
}

void test_is_title_empty(void)
{
    string *s = String()->new("");
    int result = String()->is_title(s);
    ASSERT(result == 0 || result == 1);
    String()->del(&s);
}

void test_is_title_null(void)
{
    ASSERT_EQ(String()->is_title(NULL), 0);
}

// ============================================================================
// Test Functions for String()->is_space
// ============================================================================

void test_is_space_only_spaces(void)
{
    string *s = String()->new("     ");
    ASSERT_EQ(String()->is_space(s), 1);
    String()->del(&s);
}

void test_is_space_only_tabs(void)
{
    string *s = String()->new("\t\t\t");
    ASSERT_EQ(String()->is_space(s), 1);
    String()->del(&s);
}

void test_is_space_only_newlines(void)
{
    string *s = String()->new("\n\n\n");
    ASSERT_EQ(String()->is_space(s), 1);
    String()->del(&s);
}

void test_is_space_mixed_whitespace(void)
{
    string *s = String()->new(" \t\n\r\v\f ");
    ASSERT_EQ(String()->is_space(s), 1);
    String()->del(&s);
}

void test_is_space_with_text(void)
{
    string *s = String()->new("hello world");
    ASSERT_EQ(String()->is_space(s), 0);
    String()->del(&s);
}

void test_is_space_leading_space(void)
{
    string *s = String()->new("  hello");
    ASSERT_EQ(String()->is_space(s), 0);
    String()->del(&s);
}

void test_is_space_trailing_space(void)
{
    string *s = String()->new("hello  ");
    ASSERT_EQ(String()->is_space(s), 0);
    String()->del(&s);
}

void test_is_space_only_carriage_return(void)
{
    string *s = String()->new("\r\r\r");
    ASSERT_EQ(String()->is_space(s), 1);
    String()->del(&s);
}

void test_is_space_only_vertical_tab(void)
{
    string *s = String()->new("\v\v");
    ASSERT_EQ(String()->is_space(s), 1);
    String()->del(&s);
}

void test_is_space_only_form_feed(void)
{
    string *s = String()->new("\f\f");
    ASSERT_EQ(String()->is_space(s), 1);
    String()->del(&s);
}

void test_is_space_single_space(void)
{
    string *s = String()->new(" ");
    ASSERT_EQ(String()->is_space(s), 1);
    String()->del(&s);
}

void test_is_space_empty(void)
{
    string *s = String()->new("");
    int result = String()->is_space(s);
    ASSERT(result == 0 || result == 1);
    String()->del(&s);
}

void test_is_space_null(void)
{
    ASSERT_EQ(String()->is_space(NULL), 0);
}

// ============================================================================
// Test Functions for String()->is_empty
// ============================================================================

void test_is_empty_empty_string(void)
{
    string *s = String()->new("");
    ASSERT_EQ(String()->is_empty(s), 1);
    String()->del(&s);
}

void test_is_empty_non_empty(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->is_empty(s), 0);
    String()->del(&s);
}

void test_is_empty_single_char(void)
{
    string *s = String()->new("a");
    ASSERT_EQ(String()->is_empty(s), 0);
    String()->del(&s);
}

void test_is_empty_single_space(void)
{
    string *s = String()->new(" ");
    ASSERT_EQ(String()->is_empty(s), 0);
    String()->del(&s);
}

void test_is_empty_whitespace(void)
{
    string *s = String()->new("   \t\n");
    ASSERT_EQ(String()->is_empty(s), 0);
    String()->del(&s);
}

void test_is_empty_long_string(void)
{
    string *s = String()->new("this is a longer string");
    ASSERT_EQ(String()->is_empty(s), 0);
    String()->del(&s);
}

void test_is_empty_null(void)
{
    ASSERT_EQ(String()->is_empty(NULL), 0);
}

// ============================================================================
// Test Functions for String()->count
// ============================================================================

void test_count_pchar_single(void)
{
    string *s = String()->new("hello world");
    ASSERT_EQ(String()->count(s, VAL_PCHAR("world")), 1);
    String()->del(&s);
}

void test_count_pchar_multiple(void)
{
    string *s = String()->new("hello hello hello");
    ASSERT_EQ(String()->count(s, VAL_PCHAR("hello")), 3);
    String()->del(&s);
}

void test_count_pchar_none(void)
{
    string *s = String()->new("hello world");
    ASSERT_EQ(String()->count(s, VAL_PCHAR("foo")), 0);
    String()->del(&s);
}

void test_count_pchar_overlapping(void)
{
    // "aaa" searching for "aa" - depends on implementation
    string *s = String()->new("aaa");
    int result = String()->count(s, VAL_PCHAR("aa"));
    // Could be 1 or 2 depending on overlapping handling
    ASSERT(result >= 1);
    String()->del(&s);
}

void test_count_char_single(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->count(s, VAL_CHAR('e')), 1);
    String()->del(&s);
}

void test_count_char_multiple(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->count(s, VAL_CHAR('l')), 2);
    String()->del(&s);
}

void test_count_char_none(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->count(s, VAL_CHAR('z')), 0);
    String()->del(&s);
}

void test_count_int_single(void)
{
    string *s = String()->new("test 42 test");
    ASSERT_EQ(String()->count(s, VAL_INT(42)), 1);
    String()->del(&s);
}

void test_count_int_multiple(void)
{
    string *s = String()->new("42 is 42 and 42");
    ASSERT_EQ(String()->count(s, VAL_INT(42)), 3);
    String()->del(&s);
}

void test_count_int_none(void)
{
    string *s = String()->new("hello world");
    ASSERT_EQ(String()->count(s, VAL_INT(42)), 0);
    String()->del(&s);
}

void test_count_empty_string(void)
{
    string *s = String()->new("");
    ASSERT_EQ(String()->count(s, VAL_PCHAR("test")), 0);
    String()->del(&s);
}

void test_count_null_string(void)
{
    ASSERT_EQ(String()->count(NULL, VAL_PCHAR("test")), 0);
}

void test_count_null_value(void)
{
    string *s = String()->new("hello world");
    int result = String()->count(s, VAL_PCHAR(NULL));
    ASSERT(result == 0 || result == -1);
    String()->del(&s);
}

// ============================================================================
// Test Functions for String()->swap_case
// ============================================================================

void test_swap_case_all_lower(void)
{
    string *s = String()->new("hello");
    String()->swap_case(s);
    ASSERT(equals_string(s, "HELLO"));
    String()->del(&s);
}

void test_swap_case_all_upper(void)
{
    string *s = String()->new("HELLO");
    String()->swap_case(s);
    ASSERT(equals_string(s, "hello"));
    String()->del(&s);
}

void test_swap_case_mixed(void)
{
    string *s = String()->new("HeLLo WoRLd");
    String()->swap_case(s);
    ASSERT(equals_string(s, "hEllO wOrlD"));
    String()->del(&s);
}

void test_swap_case_with_numbers(void)
{
    string *s = String()->new("Hello123World");
    String()->swap_case(s);
    ASSERT(equals_string(s, "hELLO123wORLD"));
    String()->del(&s);
}

void test_swap_case_with_symbols(void)
{
    string *s = String()->new("Hello, World!");
    String()->swap_case(s);
    ASSERT(equals_string(s, "hELLO, wORLD!"));
    String()->del(&s);
}

void test_swap_case_empty(void)
{
    string *s = String()->new("");
    String()->swap_case(s);
    ASSERT_EQ(String()->len(s), 0);
    String()->del(&s);
}

void test_swap_case_single_lower(void)
{
    string *s = String()->new("a");
    String()->swap_case(s);
    ASSERT(equals_string(s, "A"));
    String()->del(&s);
}

void test_swap_case_single_upper(void)
{
    string *s = String()->new("A");
    String()->swap_case(s);
    ASSERT(equals_string(s, "a"));
    String()->del(&s);
}

void test_swap_case_double_swap(void)
{
    string *s = String()->new("Hello World");
    String()->swap_case(s);
    String()->swap_case(s);
    ASSERT(equals_string(s, "Hello World"));
    String()->del(&s);
}

void test_swap_case_null(void)
{
    String()->swap_case(NULL);
}

// ============================================================================
// Test Functions for String()->change
// ============================================================================

void test_change_basic(void)
{
    string *s = String()->new("hello");
    String()->change(s, "world");
    ASSERT(equals_string(s, "world"));
    ASSERT_EQ(String()->len(s), 5);
    String()->del(&s);
}

void test_change_to_longer(void)
{
    string *s = String()->new("hi");
    String()->change(s, "hello world");
    ASSERT(equals_string(s, "hello world"));
    ASSERT_EQ(String()->len(s), 11);
    String()->del(&s);
}

void test_change_to_shorter(void)
{
    string *s = String()->new("hello world");
    String()->change(s, "hi");
    ASSERT(equals_string(s, "hi"));
    ASSERT_EQ(String()->len(s), 2);
    String()->del(&s);
}

void test_change_to_empty(void)
{
    string *s = String()->new("hello");
    String()->change(s, "");
    ASSERT(equals_string(s, ""));
    ASSERT_EQ(String()->len(s), 0);
    String()->del(&s);
}

void test_change_from_empty(void)
{
    string *s = String()->new("");
    String()->change(s, "hello");
    ASSERT(equals_string(s, "hello"));
    ASSERT_EQ(String()->len(s), 5);
    String()->del(&s);
}

void test_change_to_same(void)
{
    string *s = String()->new("hello");
    String()->change(s, "hello");
    ASSERT(equals_string(s, "hello"));
    ASSERT_EQ(String()->len(s), 5);
    String()->del(&s);
}

void test_change_with_null_value(void)
{
    string *s = String()->new("hello");
    String()->change(s, NULL);
    // Should clear the string
    ASSERT_EQ(String()->len(s), 0);
    String()->del(&s);
}

void test_change_null_string(void)
{
    String()->change(NULL, "hello");
}

void test_change_multiple_times(void)
{
    string *s = String()->new("first");
    String()->change(s, "second");
    String()->change(s, "third");
    ASSERT(equals_string(s, "third"));
    String()->del(&s);
}

// ============================================================================
// Test Functions for String()->compare
// ============================================================================

void test_compare_pchar_equal(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->compare(s, VAL_PCHAR("hello")), 1);
    String()->del(&s);
}

void test_compare_pchar_not_equal(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->compare(s, VAL_PCHAR("world")), 0);
    String()->del(&s);
}

void test_compare_pchar_different_length(void)
{
    string *s = String()->new("hello");
    ASSERT_EQ(String()->compare(s, VAL_PCHAR("hello world")), 0);
    String()->del(&s);
}

void test_compare_pchar_substring(void)
{
    string *s = String()->new("hello world");
    ASSERT_EQ(String()->compare(s, VAL_PCHAR("hello")), 0);
    String()->del(&s);
}

void test_compare_pchar_empty(void)
{
    string *s = String()->new("");
    ASSERT_EQ(String()->compare(s, VAL_PCHAR("")), 1);
    String()->del(&s);
}

void test_compare_pchar_case_sensitive(void)
{
    string *s = String()->new("Hello");
    ASSERT_EQ(String()->compare(s, VAL_PCHAR("hello")), 0);
    String()->del(&s);
}

void test_compare_char_single(void)
{
    string *s = String()->new("a");
    ASSERT_EQ(String()->compare(s, VAL_CHAR('a')), 1);
    String()->del(&s);
}

void test_compare_char_not_equal(void)
{
    string *s = String()->new("a");
    ASSERT_EQ(String()->compare(s, VAL_CHAR('b')), 0);
    String()->del(&s);
}

void test_compare_int_equal(void)
{
    string *s = String()->new("42");
    ASSERT_EQ(String()->compare(s, VAL_INT(42)), 1);
    String()->del(&s);
}

void test_compare_int_not_equal(void)
{
    string *s = String()->new("42");
    ASSERT_EQ(String()->compare(s, VAL_INT(123)), 0);
    String()->del(&s);
}

void test_compare_int_negative(void)
{
    string *s = String()->new("-42");
    ASSERT_EQ(String()->compare(s, VAL_INT(-42)), 1);
    String()->del(&s);
}

void test_compare_string_type(void)
{
    string *s1 = String()->new("hello");
    string *s2 = String()->new("hello");
    ASSERT_EQ(String()->compare(s1, VAL_STR(s2)), 1);
    String()->del(&s1);
    String()->del(&s2);
}

void test_compare_null_string(void)
{
    ASSERT_EQ(String()->compare(NULL, VAL_PCHAR("hello")), 0);
}

void test_compare_null_value(void)
{
    string *s = String()->new("hello");
    int result = String()->compare(s, VAL_PCHAR(NULL));
    ASSERT(result == 0 || result == -1);
    String()->del(&s);
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_large_append(void)
{
    string *s = String()->new("");
    char chunk[101];
    memset(chunk, 'x', 100);
    chunk[100] = '\0';
    const char *chunk_ptr = chunk;
    
    for (int i = 0; i < 100; i++)
        String()->append(s, VAL_PCHAR(chunk_ptr));
    
    ASSERT_EQ(String()->len(s), 10000);
    String()->del(&s);
}

void test_int_min_value(void)
{
    string *s = String()->new("");
    int n = -2147483648;  // INT_MIN
    String()->append(s, VAL_INT(n));
    ASSERT_NOT_NULL(s);
    String()->del(&s);
}

void test_int_max_value(void)
{
    string *s = String()->new("");
    int n = 2147483647;  // INT_MAX
    String()->append(s, VAL_INT(n));
    ASSERT_NOT_NULL(s);
    String()->del(&s);
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main(int argc, char **argv)
{
    // Check for verbose flag
    if (argc > 1 && strcmp(argv[1], "-v") == 0)
        set_verbose(1);
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->new tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->new");
    
    TEST("new: basic string", test_new_basic());
    TEST("new: empty string", test_new_empty());
    TEST("new: long string", test_new_long_string());
    TEST_NULL_SAFE("new: NULL input", test_new_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->len tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->len");
    
    TEST("len: basic length", test_len_basic());
    TEST("len: empty string", test_len_empty());
    TEST_NULL_SAFE("len: NULL string", test_len_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->del tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->del");
    
    TEST("del: basic delete", test_del_basic());
    TEST_NULL_SAFE("del: NULL pointer", test_del_null_ptr());
    TEST_NULL_SAFE("del: pointer to NULL", test_del_ptr_to_null());
    TEST_NULL_SAFE("del: double free", test_del_double_free());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->write tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->write");
    
    TEST("write: basic write", test_write_basic());
    TEST_NULL_SAFE("write: NULL string", test_write_null());
    TEST("write: invalid fd", test_write_invalid_fd());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->append tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->append");
    
    TEST("append: pchar", test_append_pchar());
    TEST("append: string", test_append_string());
    TEST("append: char", test_append_char());
    TEST("append: int", test_append_int());
    TEST("append: int negative", test_append_int_negative());
    TEST("append: int zero", test_append_int_zero());
    TEST("append: llong", test_append_llong());
    TEST("append: llong negative", test_append_llong_negative());
    TEST("append: empty pchar", test_append_empty_pchar());
    TEST("append: multiple", test_append_multiple());
    TEST_NULL_SAFE("append: NULL string", test_append_null_string());
    TEST_NULL_SAFE("append: NULL value", test_append_null_value());
    
    // ─────────────────────────────────────────────────────────────────────
    // String() struct tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String() struct");
    
    TEST("struct: not null", test_string_struct_not_null());
    TEST("struct: functions not null", test_string_struct_functions_not_null());
    TEST("struct: singleton pattern", test_string_struct_singleton());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->clone tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->clone");
    
    TEST("clone: basic", test_clone_basic());
    TEST("clone: empty string", test_clone_empty());
    TEST_NULL_SAFE("clone: NULL input", test_clone_null());
    TEST("clone: independence", test_clone_independence());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->to_lower tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->to_lower");
    
    TEST("to_lower: basic", test_to_lower_basic());
    TEST("to_lower: mixed case", test_to_lower_mixed());
    TEST("to_lower: already lowercase", test_to_lower_already_lower());
    TEST("to_lower: with numbers", test_to_lower_with_numbers());
    TEST("to_lower: empty string", test_to_lower_empty());
    TEST_NULL_SAFE("to_lower: NULL input", test_to_lower_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->to_upper tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->to_upper");
    
    TEST("to_upper: basic", test_to_upper_basic());
    TEST("to_upper: mixed case", test_to_upper_mixed());
    TEST("to_upper: already uppercase", test_to_upper_already_upper());
    TEST("to_upper: with numbers", test_to_upper_with_numbers());
    TEST("to_upper: empty string", test_to_upper_empty());
    TEST_NULL_SAFE("to_upper: NULL input", test_to_upper_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->index_of tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->index_of");
    
    TEST("index_of: pchar found", test_index_of_pchar_found());
    TEST("index_of: pchar not found", test_index_of_pchar_not_found());
    TEST("index_of: pchar at start", test_index_of_pchar_at_start());
    TEST("index_of: pchar at end", test_index_of_pchar_at_end());
    TEST("index_of: char found", test_index_of_char_found());
    TEST("index_of: char not found", test_index_of_char_not_found());
    TEST("index_of: char first", test_index_of_char_first());
    TEST("index_of: int found", test_index_of_int_found());
    TEST("index_of: int not found", test_index_of_int_not_found());
    TEST("index_of: int negative", test_index_of_int_negative());
    TEST("index_of: string found", test_index_of_string());
    TEST("index_of: string not found", test_index_of_string_not_found());
    TEST("index_of: empty string", test_index_of_empty_string());
    TEST_NULL_SAFE("index_of: NULL string", test_index_of_null_string());
    TEST_NULL_SAFE("index_of: NULL value", test_index_of_null_value());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->last_index_of tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->last_index_of");
    
    TEST("last_index_of: pchar single", test_last_index_of_pchar_single());
    TEST("last_index_of: pchar multiple", test_last_index_of_pchar_multiple());
    TEST("last_index_of: pchar not found", test_last_index_of_pchar_not_found());
    TEST("last_index_of: pchar at start", test_last_index_of_pchar_at_start());
    TEST("last_index_of: pchar at end", test_last_index_of_pchar_at_end());
    TEST("last_index_of: char single", test_last_index_of_char_single());
    TEST("last_index_of: char multiple", test_last_index_of_char_multiple());
    TEST("last_index_of: char not found", test_last_index_of_char_not_found());
    TEST("last_index_of: int single", test_last_index_of_int_single());
    TEST("last_index_of: int multiple", test_last_index_of_int_multiple());
    TEST("last_index_of: int not found", test_last_index_of_int_not_found());
    TEST("last_index_of: string", test_last_index_of_string());
    TEST("last_index_of: empty string", test_last_index_of_empty_string());
    TEST_NULL_SAFE("last_index_of: NULL string", test_last_index_of_null_string());
    TEST_NULL_SAFE("last_index_of: NULL value", test_last_index_of_null_value());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->is_null tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->is_null");
    
    TEST("is_null: NULL pointer", test_is_null_with_null_ptr());
    TEST("is_null: valid string", test_is_null_with_valid_string());
    TEST("is_null: empty string", test_is_null_with_empty_string());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->is_alpha tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->is_alpha");
    
    TEST("is_alpha: all lower", test_is_alpha_all_lower());
    TEST("is_alpha: all upper", test_is_alpha_all_upper());
    TEST("is_alpha: mixed case", test_is_alpha_mixed_case());
    TEST("is_alpha: with numbers", test_is_alpha_with_numbers());
    TEST("is_alpha: with space", test_is_alpha_with_space());
    TEST("is_alpha: with symbols", test_is_alpha_with_symbols());
    TEST("is_alpha: empty string", test_is_alpha_empty());
    TEST_NULL_SAFE("is_alpha: NULL input", test_is_alpha_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->is_alnum tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->is_alnum");
    
    TEST("is_alnum: all alpha", test_is_alnum_all_alpha());
    TEST("is_alnum: all digits", test_is_alnum_all_digits());
    TEST("is_alnum: mixed", test_is_alnum_mixed());
    TEST("is_alnum: with space", test_is_alnum_with_space());
    TEST("is_alnum: with symbols", test_is_alnum_with_symbols());
    TEST("is_alnum: with underscore", test_is_alnum_with_underscore());
    TEST("is_alnum: empty string", test_is_alnum_empty());
    TEST_NULL_SAFE("is_alnum: NULL input", test_is_alnum_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->is_ascii tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->is_ascii");
    
    TEST("is_ascii: basic", test_is_ascii_basic());
    TEST("is_ascii: all printable", test_is_ascii_all_printable());
    TEST("is_ascii: special chars", test_is_ascii_special_chars());
    TEST("is_ascii: with newline", test_is_ascii_with_newline());
    TEST("is_ascii: with tab", test_is_ascii_with_tab());
    TEST("is_ascii: with high byte", test_is_ascii_with_high_byte());
    TEST("is_ascii: with utf8", test_is_ascii_with_utf8());
    TEST("is_ascii: empty string", test_is_ascii_empty());
    TEST_NULL_SAFE("is_ascii: NULL input", test_is_ascii_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->is_lower tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->is_lower");
    
    TEST("is_lower: all lower", test_is_lower_all_lower());
    TEST("is_lower: with upper", test_is_lower_with_upper());
    TEST("is_lower: all upper", test_is_lower_all_upper());
    TEST("is_lower: with numbers", test_is_lower_with_numbers());
    TEST("is_lower: with space", test_is_lower_with_space());
    TEST("is_lower: with symbols", test_is_lower_with_symbols());
    TEST("is_lower: empty string", test_is_lower_empty());
    TEST_NULL_SAFE("is_lower: NULL input", test_is_lower_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->is_upper tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->is_upper");
    
    TEST("is_upper: all upper", test_is_upper_all_upper());
    TEST("is_upper: with lower", test_is_upper_with_lower());
    TEST("is_upper: all lower", test_is_upper_all_lower());
    TEST("is_upper: with numbers", test_is_upper_with_numbers());
    TEST("is_upper: with space", test_is_upper_with_space());
    TEST("is_upper: with symbols", test_is_upper_with_symbols());
    TEST("is_upper: empty string", test_is_upper_empty());
    TEST_NULL_SAFE("is_upper: NULL input", test_is_upper_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->is_printable tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->is_printable");
    
    TEST("is_printable: basic", test_is_printable_basic());
    TEST("is_printable: all printable", test_is_printable_all_printable());
    TEST("is_printable: special chars", test_is_printable_special_chars());
    TEST("is_printable: with space", test_is_printable_with_space());
    TEST("is_printable: with tab", test_is_printable_with_tab());
    TEST("is_printable: with newline", test_is_printable_with_newline());
    TEST("is_printable: with carriage return", test_is_printable_with_carriage_return());
    TEST("is_printable: with null char", test_is_printable_with_null_char());
    TEST("is_printable: with bell", test_is_printable_with_bell());
    TEST("is_printable: with escape", test_is_printable_with_escape());
    TEST("is_printable: with DEL", test_is_printable_with_del());
    TEST("is_printable: empty string", test_is_printable_empty());
    TEST_NULL_SAFE("is_printable: NULL input", test_is_printable_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->is_title tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->is_title");
    
    TEST("is_title: basic", test_is_title_basic());
    TEST("is_title: all lower", test_is_title_all_lower());
    TEST("is_title: all upper", test_is_title_all_upper());
    TEST("is_title: mid-word upper", test_is_title_mid_word_upper());
    TEST("is_title: camelCase", test_is_title_camelCase());
    TEST("is_title: with numbers", test_is_title_with_numbers());
    TEST("is_title: with comma", test_is_title_with_comma());
    TEST("is_title: with slash", test_is_title_with_slash());
    TEST("is_title: with tab", test_is_title_with_tab());
    TEST("is_title: single word", test_is_title_single_word());
    TEST("is_title: single upper", test_is_title_single_upper());
    TEST("is_title: single lower", test_is_title_single_lower());
    TEST("is_title: empty string", test_is_title_empty());
    TEST_NULL_SAFE("is_title: NULL input", test_is_title_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->is_space tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->is_space");
    
    TEST("is_space: only spaces", test_is_space_only_spaces());
    TEST("is_space: only tabs", test_is_space_only_tabs());
    TEST("is_space: only newlines", test_is_space_only_newlines());
    TEST("is_space: mixed whitespace", test_is_space_mixed_whitespace());
    TEST("is_space: with text", test_is_space_with_text());
    TEST("is_space: leading space", test_is_space_leading_space());
    TEST("is_space: trailing space", test_is_space_trailing_space());
    TEST("is_space: carriage return", test_is_space_only_carriage_return());
    TEST("is_space: vertical tab", test_is_space_only_vertical_tab());
    TEST("is_space: form feed", test_is_space_only_form_feed());
    TEST("is_space: single space", test_is_space_single_space());
    TEST("is_space: empty string", test_is_space_empty());
    TEST_NULL_SAFE("is_space: NULL input", test_is_space_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->is_empty tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->is_empty");
    
    TEST("is_empty: empty string", test_is_empty_empty_string());
    TEST("is_empty: non-empty", test_is_empty_non_empty());
    TEST("is_empty: single char", test_is_empty_single_char());
    TEST("is_empty: single space", test_is_empty_single_space());
    TEST("is_empty: whitespace", test_is_empty_whitespace());
    TEST("is_empty: long string", test_is_empty_long_string());
    TEST_NULL_SAFE("is_empty: NULL input", test_is_empty_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->count tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->count");
    
    TEST("count: pchar single", test_count_pchar_single());
    TEST("count: pchar multiple", test_count_pchar_multiple());
    TEST("count: pchar none", test_count_pchar_none());
    TEST("count: pchar overlapping", test_count_pchar_overlapping());
    TEST("count: char single", test_count_char_single());
    TEST("count: char multiple", test_count_char_multiple());
    TEST("count: char none", test_count_char_none());
    TEST("count: int single", test_count_int_single());
    TEST("count: int multiple", test_count_int_multiple());
    TEST("count: int none", test_count_int_none());
    TEST("count: empty string", test_count_empty_string());
    TEST_NULL_SAFE("count: NULL string", test_count_null_string());
    TEST_NULL_SAFE("count: NULL value", test_count_null_value());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->swap_case tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->swap_case");
    
    TEST("swap_case: all lower", test_swap_case_all_lower());
    TEST("swap_case: all upper", test_swap_case_all_upper());
    TEST("swap_case: mixed", test_swap_case_mixed());
    TEST("swap_case: with numbers", test_swap_case_with_numbers());
    TEST("swap_case: with symbols", test_swap_case_with_symbols());
    TEST("swap_case: empty string", test_swap_case_empty());
    TEST("swap_case: single lower", test_swap_case_single_lower());
    TEST("swap_case: single upper", test_swap_case_single_upper());
    TEST("swap_case: double swap", test_swap_case_double_swap());
    TEST_NULL_SAFE("swap_case: NULL input", test_swap_case_null());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->change tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->change");
    
    TEST("change: basic", test_change_basic());
    TEST("change: to longer", test_change_to_longer());
    TEST("change: to shorter", test_change_to_shorter());
    TEST("change: to empty", test_change_to_empty());
    TEST("change: from empty", test_change_from_empty());
    TEST("change: to same", test_change_to_same());
    TEST_NULL_SAFE("change: NULL value", test_change_with_null_value());
    TEST_NULL_SAFE("change: NULL string", test_change_null_string());
    TEST("change: multiple times", test_change_multiple_times());
    
    // ─────────────────────────────────────────────────────────────────────
    // String()->compare tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("String()->compare");
    
    TEST("compare: pchar equal", test_compare_pchar_equal());
    TEST("compare: pchar not equal", test_compare_pchar_not_equal());
    TEST("compare: pchar different length", test_compare_pchar_different_length());
    TEST("compare: pchar substring", test_compare_pchar_substring());
    TEST("compare: pchar empty", test_compare_pchar_empty());
    TEST("compare: pchar case sensitive", test_compare_pchar_case_sensitive());
    TEST("compare: char single", test_compare_char_single());
    TEST("compare: char not equal", test_compare_char_not_equal());
    TEST("compare: int equal", test_compare_int_equal());
    TEST("compare: int not equal", test_compare_int_not_equal());
    TEST("compare: int negative", test_compare_int_negative());
    TEST("compare: string type", test_compare_string_type());
    TEST_NULL_SAFE("compare: NULL string", test_compare_null_string());
    TEST_NULL_SAFE("compare: NULL value", test_compare_null_value());
    
    // ─────────────────────────────────────────────────────────────────────
    // Edge case tests
    // ─────────────────────────────────────────────────────────────────────
    print_suite_header("Edge Cases");
    
    TEST("edge: large append", test_large_append());
    TEST("edge: INT_MIN", test_int_min_value());
    TEST("edge: INT_MAX", test_int_max_value());
    
    // ─────────────────────────────────────────────────────────────────────
    // Final Results
    // ─────────────────────────────────────────────────────────────────────
    print_final_score();
    
    return get_exit_code();
}
