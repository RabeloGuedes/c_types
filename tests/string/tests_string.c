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
    String()->del(&s);
}

void test_append_string(void)
{
    string *s1 = String()->new("hello");
    string *s2 = String()->new(" world");
    String()->append(s1, TYPE_STRING, s2);
    ASSERT_EQ(String()->len(s1), 11);
    String()->del(&s1);
    String()->del(&s2);
}

void test_append_char(void)
{
    string *s = String()->new("hell");
    char c = 'o';
    String()->append(s, TYPE_CHAR, &c);
    ASSERT_EQ(String()->len(s), 5);
    String()->del(&s);
}

void test_append_int(void)
{
    string *s = String()->new("num: ");
    int n = 42;
    String()->append(s, TYPE_INT, &n);
    ASSERT_EQ(String()->len(s), 7);
    String()->del(&s);
}

void test_append_int_negative(void)
{
    string *s = String()->new("num: ");
    int n = -42;
    String()->append(s, TYPE_INT, &n);
    ASSERT_EQ(String()->len(s), 8);  // "num: -42"
    String()->del(&s);
}

void test_append_llong(void)
{
    string *s = String()->new("big: ");
    long long n = 9223372036854775807LL;
    String()->append(s, TYPE_LLONG, &n);
    ASSERT_NOT_NULL(s);
    String()->del(&s);
}

void test_append_llong_negative(void)
{
    string *s = String()->new("neg: ");
    long long n = -9223372036854775807LL;
    String()->append(s, TYPE_LLONG, &n);
    ASSERT_NOT_NULL(s);
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
    String()->del(&s);
}

void test_append_int_zero(void)
{
    string *s = String()->new("zero: ");
    int n = 0;
    String()->append(s, TYPE_INT, &n);
    ASSERT_EQ(String()->len(s), 7);  // "zero: 0"
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
}

void test_string_struct_singleton(void)
{
    str_funcs *funcs1 = String();
    str_funcs *funcs2 = String();
    ASSERT_EQ(funcs1, funcs2);  // Should return same static struct
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
