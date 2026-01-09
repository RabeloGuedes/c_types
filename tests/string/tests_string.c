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
    String()->append(s, TYPE_PCHAR, (void *)&suffix);
    ASSERT_EQ(String()->len(s), 11);
    ASSERT(equals_string(s, "hello world"));
    String()->del(&s);
}

void test_append_string(void)
{
    string *s1 = String()->new("hello");
    string *s2 = String()->new(" world");
    String()->append(s1, TYPE_STRING, s2);
    ASSERT_EQ(String()->len(s1), 11);
    ASSERT(equals_string(s1, "hello world"));
    String()->del(&s1);
    String()->del(&s2);
}

void test_append_char(void)
{
    string *s = String()->new("hell");
    char c = 'o';
    String()->append(s, TYPE_CHAR, &c);
    ASSERT_EQ(String()->len(s), 5);
    ASSERT(equals_string(s, "hello"));
    String()->del(&s);
}

void test_append_int(void)
{
    string *s = String()->new("num: ");
    int n = 42;
    String()->append(s, TYPE_INT, &n);
    ASSERT_EQ(String()->len(s), 7);
    ASSERT(equals_string(s, "num: 42"));
    String()->del(&s);
}

void test_append_int_negative(void)
{
    string *s = String()->new("num: ");
    int n = -42;
    String()->append(s, TYPE_INT, &n);
    ASSERT_EQ(String()->len(s), 8);
    ASSERT(equals_string(s, "num: -42"));
    String()->del(&s);
}

void test_append_llong(void)
{
    string *s = String()->new("big: ");
    long long n = 9223372036854775807LL;
    String()->append(s, TYPE_LLONG, &n);
    ASSERT_NOT_NULL(s);
    ASSERT(equals_string(s, "big: 9223372036854775807"));
    String()->del(&s);
}

void test_append_llong_negative(void)
{
    string *s = String()->new("neg: ");
    long long n = -9223372036854775807LL;
    String()->append(s, TYPE_LLONG, &n);
    ASSERT_NOT_NULL(s);
    ASSERT(equals_string(s, "neg: -9223372036854775807"));
    String()->del(&s);
}

void test_append_null_string(void)
{
    const char *suffix = " world";
    String()->append(NULL, TYPE_PCHAR, (void *)&suffix);
}

void test_append_null_value(void)
{
    string *s = String()->new("hello");
    String()->append(s, TYPE_PCHAR, NULL);
    String()->del(&s);
}

void test_append_empty_pchar(void)
{
    string *s = String()->new("hello");
    const char *empty = "";
    String()->append(s, TYPE_PCHAR, (void *)&empty);
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
    
    String()->append(s, TYPE_PCHAR, (void *)&hello);
    String()->append(s, TYPE_PCHAR, (void *)&space);
    String()->append(s, TYPE_PCHAR, (void *)&world);
    
    ASSERT_EQ(String()->len(s), 11);
    ASSERT(equals_string(s, "hello world"));
    String()->del(&s);
}

void test_append_int_zero(void)
{
    string *s = String()->new("zero: ");
    int n = 0;
    String()->append(s, TYPE_INT, &n);
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
    String()->append(s, TYPE_PCHAR, (void *)&suffix);
    
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
    int idx = String()->index_of(s, TYPE_PCHAR, (void *)&needle);
    ASSERT_EQ(idx, 6);
    String()->del(&s);
}

void test_index_of_pchar_not_found(void)
{
    string *s = String()->new("hello world");
    const char *needle = "foo";
    int idx = String()->index_of(s, TYPE_PCHAR, (void *)&needle);
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_index_of_pchar_at_start(void)
{
    string *s = String()->new("hello world");
    const char *needle = "hello";
    int idx = String()->index_of(s, TYPE_PCHAR, (void *)&needle);
    ASSERT_EQ(idx, 0);
    String()->del(&s);
}

void test_index_of_pchar_at_end(void)
{
    string *s = String()->new("hello world");
    const char *needle = "ld";
    int idx = String()->index_of(s, TYPE_PCHAR, (void *)&needle);
    ASSERT_EQ(idx, 9);
    String()->del(&s);
}

void test_index_of_char_found(void)
{
    string *s = String()->new("hello world");
    char c = 'w';
    int idx = String()->index_of(s, TYPE_CHAR, &c);
    ASSERT_EQ(idx, 6);
    String()->del(&s);
}

void test_index_of_char_not_found(void)
{
    string *s = String()->new("hello world");
    char c = 'z';
    int idx = String()->index_of(s, TYPE_CHAR, &c);
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_index_of_char_first(void)
{
    string *s = String()->new("hello world");
    char c = 'h';
    int idx = String()->index_of(s, TYPE_CHAR, &c);
    ASSERT_EQ(idx, 0);
    String()->del(&s);
}

void test_index_of_int_found(void)
{
    string *s = String()->new("value is 42 here");
    int n = 42;
    int idx = String()->index_of(s, TYPE_INT, &n);
    ASSERT_EQ(idx, 9);
    String()->del(&s);
}

void test_index_of_int_not_found(void)
{
    string *s = String()->new("value is 42 here");
    int n = 99;
    int idx = String()->index_of(s, TYPE_INT, &n);
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_index_of_int_negative(void)
{
    string *s = String()->new("temp is -10 degrees");
    int n = -10;
    int idx = String()->index_of(s, TYPE_INT, &n);
    ASSERT_EQ(idx, 8);
    String()->del(&s);
}

void test_index_of_string(void)
{
    string *s = String()->new("hello world");
    string *needle = String()->new("world");
    int idx = String()->index_of(s, TYPE_STRING, needle);
    ASSERT_EQ(idx, 6);
    String()->del(&s);
    String()->del(&needle);
}

void test_index_of_string_not_found(void)
{
    string *s = String()->new("hello world");
    string *needle = String()->new("foo");
    int idx = String()->index_of(s, TYPE_STRING, needle);
    ASSERT_EQ(idx, -1);
    String()->del(&s);
    String()->del(&needle);
}

void test_index_of_empty_string(void)
{
    string *s = String()->new("");
    const char *needle = "test";
    int idx = String()->index_of(s, TYPE_PCHAR, (void *)&needle);
    ASSERT_EQ(idx, -1);
    String()->del(&s);
}

void test_index_of_null_string(void)
{
    const char *needle = "test";
    int idx = String()->index_of(NULL, TYPE_PCHAR, (void *)&needle);
    ASSERT_EQ(idx, -1);
}

void test_index_of_null_value(void)
{
    string *s = String()->new("hello world");
    int idx = String()->index_of(s, TYPE_PCHAR, NULL);
    ASSERT_EQ(idx, -1);
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
        String()->append(s, TYPE_PCHAR, (void *)&chunk_ptr);
    
    ASSERT_EQ(String()->len(s), 10000);
    String()->del(&s);
}

void test_int_min_value(void)
{
    string *s = String()->new("");
    int n = -2147483648;  // INT_MIN
    String()->append(s, TYPE_INT, &n);
    ASSERT_NOT_NULL(s);
    String()->del(&s);
}

void test_int_max_value(void)
{
    string *s = String()->new("");
    int n = 2147483647;  // INT_MAX
    String()->append(s, TYPE_INT, &n);
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
