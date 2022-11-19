#include <stdio.h>
#include <inttypes.h>
#include "test.h"
#include "Number.h"

int main()
{
    TEST_HEADER;

    uint64_t a = 11;
    uint64_t b = 12;

    printf("%" PRIu64 "\n", min(a, b));
    printf("%d\n", max(100, min(1, -12)));

    return 0;
}
