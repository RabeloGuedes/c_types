#ifndef TEST_FRAMEWORK_H
# define TEST_FRAMEWORK_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <setjmp.h>
# include <unistd.h>
# include <sys/wait.h>

// Colors
# define RED     "\033[0;31m"
# define GREEN   "\033[0;32m"
# define YELLOW  "\033[0;33m"
# define BLUE    "\033[0;34m"
# define RESET   "\033[0m"
# define BOLD    "\033[1m"

// Test result structure
typedef struct s_test_result {
    int     passed;
    int     failed;
    int     total;
}   t_test_result;

// Global test state
static t_test_result    g_results = {0, 0, 0};
static int              g_verbose = 0;
static jmp_buf          g_jump_buffer;
static volatile int     g_test_failed = 0;

// Signal handler for segfaults during tests
static void segfault_handler(int sig)
{
    (void)sig;
    g_test_failed = 1;
    longjmp(g_jump_buffer, 1);
}

// Run a test that might segfault (expects NO crash)
# define TEST(name, code) do { \
    g_results.total++; \
    g_test_failed = 0; \
    signal(SIGSEGV, segfault_handler); \
    signal(SIGBUS, segfault_handler); \
    signal(SIGABRT, segfault_handler); \
    if (setjmp(g_jump_buffer) == 0) { \
        code; \
        if (!g_test_failed) { \
            g_results.passed++; \
            if (g_verbose) \
                printf(GREEN "  ✓ " RESET "%s\n", name); \
        } else { \
            g_results.failed++; \
            if (g_verbose) \
                printf(RED "  ✗ " RESET "%s (assertion failed)\n", name); \
        } \
    } else { \
        g_results.failed++; \
        if (g_verbose) \
            printf(RED "  ✗ " RESET "%s (crashed unexpectedly)\n", name); \
    } \
    signal(SIGSEGV, SIG_DFL); \
    signal(SIGBUS, SIG_DFL); \
    signal(SIGABRT, SIG_DFL); \
} while(0)

// Run a test that SHOULD handle NULL without crashing
# define TEST_NULL_SAFE(name, code) do { \
    g_results.total++; \
    g_test_failed = 0; \
    pid_t pid = fork(); \
    if (pid == 0) { \
        code; \
        exit(0); \
    } else { \
        int status; \
        waitpid(pid, &status, 0); \
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) { \
            g_results.passed++; \
            if (g_verbose) \
                printf(GREEN "  ✓ " RESET "%s (NULL handled safely)\n", name); \
        } else { \
            g_results.failed++; \
            if (g_verbose) \
                printf(RED "  ✗ " RESET "%s (crashed on NULL)\n", name); \
        } \
    } \
} while(0)

// Assertion macros
# define ASSERT(cond) do { if (!(cond)) { g_test_failed = 1; return; } } while(0)
# define ASSERT_EQ(a, b) ASSERT((a) == (b))
# define ASSERT_NE(a, b) ASSERT((a) != (b))
# define ASSERT_NULL(a) ASSERT((a) == NULL)
# define ASSERT_NOT_NULL(a) ASSERT((a) != NULL)
# define ASSERT_STR_EQ(a, b) ASSERT(strcmp((a), (b)) == 0)

// Progress bar (used in print_final_score)
__attribute__((unused))
static void print_progress_bar(int current, int total, int width)
{
    float   ratio = (float)current / total;
    int     filled = (int)(ratio * width);
    int     i;

    printf("\r  [");
    for (i = 0; i < filled; i++)
        printf("█");
    for (i = filled; i < width; i++)
        printf("░");
    printf("] %3d%% (%d/%d)", (int)(ratio * 100), current, total);
    fflush(stdout);
}

// Final score display
static void print_final_score(void)
{
    float   percent = g_results.total > 0 
        ? (float)g_results.passed / g_results.total * 100 : 0;
    
    printf("\n\n" BOLD "═══════════════════════════════════════════\n" RESET);
    printf(BOLD "                 TEST RESULTS\n" RESET);
    printf(BOLD "═══════════════════════════════════════════\n\n" RESET);
    
    // Score bar
    int width = 30;
    int filled = (int)(percent / 100 * width);
    int i;
    
    printf("  Score: [");
    for (i = 0; i < filled; i++)
        printf(GREEN "█" RESET);
    for (i = filled; i < width; i++)
        printf(RED "░" RESET);
    printf("] ");
    
    if (percent >= 80)
        printf(GREEN);
    else if (percent >= 50)
        printf(YELLOW);
    else
        printf(RED);
    
    printf("%.1f%%\n\n" RESET, percent);
    
    printf("  " GREEN "Passed: %d" RESET "\n", g_results.passed);
    printf("  " RED "Failed: %d" RESET "\n", g_results.failed);
    printf("  " BLUE "Total:  %d" RESET "\n", g_results.total);
    printf(BOLD "\n═══════════════════════════════════════════\n" RESET);
    
    if (g_results.failed == 0)
        printf(GREEN BOLD "\n  ✓ All tests passed!\n\n" RESET);
    else
        printf(RED BOLD "\n  ✗ Some tests failed.\n\n" RESET);
}

// Test suite header
static void print_suite_header(const char *name)
{
    printf(BOLD BLUE "\n▸ %s\n" RESET, name);
    printf("───────────────────────────────────────────\n");
}

// Set verbose mode
static void set_verbose(int v)
{
    g_verbose = v;
}

// Get exit code based on results
static int get_exit_code(void)
{
    return (g_results.failed > 0) ? 1 : 0;
}

#endif
