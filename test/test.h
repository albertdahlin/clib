#define TEST_HEADER \
    printf("\n*** TEST " __FILE__ " ***\n")

#define debugger asm("int $3");

#define TEST_RUN(F) printf("\n%s\n", #F); if (F) return 1;

#define EXPECT(expr, fmt, ...) \
    if (expr) { \
        printf("%4.d: %s -> OK\n", __LINE__, #expr); \
    } else { \
        fprintf(stderr, "\e[91m%4.d: %s -> " fmt "\e[0m\n", __LINE__, #expr, ##__VA_ARGS__); \
        return 1; \
    }

