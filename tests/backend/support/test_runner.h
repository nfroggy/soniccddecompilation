#ifndef BACKEND_TEST_RUNNER_H
#define BACKEND_TEST_RUNNER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct test_context {
    int failures;
} test_context;

static void test_fail(test_context *ctx, const char *file, int line,
                      const char *expr) {
    ++ctx->failures;
    fprintf(stderr, "%s:%d: assertion failed: %s\n", file, line, expr);
}

#define TEST_ASSERT_TRUE(ctx, expr)                                            \
    do {                                                                       \
        if (!(expr)) {                                                         \
            test_fail((ctx), __FILE__, __LINE__, #expr);                       \
        }                                                                      \
    } while (0)

#define TEST_ASSERT_EQ_INT(ctx, expected, actual)                              \
    do {                                                                       \
        int test_expected_ = (int)(expected);                                   \
        int test_actual_ = (int)(actual);                                       \
        if (test_expected_ != test_actual_) {                                   \
            ++(ctx)->failures;                                                 \
            fprintf(stderr,                                                    \
                    "%s:%d: assertion failed: expected %d, got %d\n",          \
                    __FILE__, __LINE__, test_expected_, test_actual_);          \
        }                                                                      \
    } while (0)

#define TEST_ASSERT_NOT_NULL(ctx, value)                                       \
    do {                                                                       \
        if ((value) == 0) {                                                     \
            test_fail((ctx), __FILE__, __LINE__, #value " != 0");              \
        }                                                                      \
    } while (0)

#define TEST_MAIN_BEGIN                                                        \
    int main(void) {                                                           \
        test_context ctx = {0}

#define TEST_MAIN_END                                                          \
        if (ctx.failures != 0) {                                               \
            fprintf(stderr, "%d assertion(s) failed\n", ctx.failures);         \
            return EXIT_FAILURE;                                               \
        }                                                                      \
        return EXIT_SUCCESS;                                                   \
    }

#endif
