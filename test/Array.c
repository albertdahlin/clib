#include <Array.h>
#include <stdio.h>
#include <inttypes.h>
#include "test.h"

typedef struct {
    int64_t one;
    uint64_t two;
} TestStruct;

static inline TestStruct testar(uint64_t a, uint64_t b)
{
    return (TestStruct){a, b};
}


int TestStruct_compareAsc(const void *ptrA, const void *ptrB)
{
    TestStruct *a = (TestStruct*)ptrA;
    TestStruct *b = (TestStruct*)ptrB;

    return a->one - b->one;
}

int TestStruct_compareDesc(const void *ptrA, const void *ptrB)
{
    TestStruct *a = (TestStruct*)ptrA;
    TestStruct *b = (TestStruct*)ptrB;

    return b->one - a->one;
}

int array_sort_i32()
{
    int32_t *array = Array_alloc(sizeof(int32_t), 8);

    Array_push(array, 0);

    for (int32_t i = 1; i < 5; i++) {
        Array_push(array, i);
        Array_push(array, i * -1);
    }

    Array_qsort_i32(array);

    int32_t prev = -10;
    Array_for(array, i) {
        EXPECT(array[i] > prev, "%d", array[i]);
    }

    Array_free(array);

    return 0;
}

int array_sort_u32()
{
    uint32_t *array = Array_alloc(sizeof(uint32_t), 8);

    for (uint32_t i = 1; i < 10; i++) {
        Array_push(array, i);
    }

    Array_qsort_u32(array);
    uint32_t prev = 0;

    Array_for(array, i) {
        EXPECT(array[i] > prev, "%d", array[i]);
        prev = array[i];
    }

    Array_free(array);

    return 0;
}

int test_Array_qsort_u8()
{
    uint8_t *alphabet = Array_alloc(sizeof(uint8_t), 26);

    for (uint8_t letter = 'Z'; letter >= 'A'; letter--) {
        Array_push(alphabet, letter);
    }

    Array_qsort_u8(alphabet);
    EXPECT(alphabet[0] == 'A', "%c", alphabet[0]);
    EXPECT(alphabet[25] == 'Z', "%c", alphabet[25]);

    Array_free(alphabet);

    return 0;
}

int test_Array_index_sort()
{
    TestStruct *array = Array_alloc(sizeof(TestStruct), 8);

    for (uint64_t i = 0; i <= 5; i++) {
        Array_push(array, testar(10 * (i + 1), 2*i));
        Array_push(array, testar(-10 * (i + 1), 2*i+1));
    }

    uint32_t *sorted = Array_alloc(sizeof(uint32_t), 5);

    for (uint32_t i = 0; i < 5; i++) {
        Array_push(sorted, i);
    }
    Array_index_sort(sorted, array, TestStruct_compareAsc);

    int64_t prev = -200;
    Array_for(sorted, i) {
        size_t idx = sorted[i];
        EXPECT(array[idx].one > prev, "%zu: %lu > %lu", idx, array[idx].one, prev);
        prev = array[idx].one;
    }

    Array_index_sort(sorted, array, TestStruct_compareDesc);
    prev = 200;
    Array_for(sorted, i) {
        size_t idx = sorted[i];
        EXPECT(array[idx].one < prev, "%zu: %lu > %lu", idx, array[idx].one, prev);
        prev = array[idx].one;
    }

    Array_free(array);
    Array_free(sorted);

    return 0;
}

int main()
{
    TEST_HEADER;

    printf("Array push/pop\n");
    TestStruct *arr = Array_alloc(sizeof(TestStruct), 2);

    for (uint64_t i = 1; i <= 100; i++) {
        Array_push(arr, testar(i * 10, 10));
    }

    EXPECT(Array_length(arr) == 100, "Wrong length %zu", Array_length(arr));

    TestStruct poppedValue = Array_pop(arr);
    EXPECT(poppedValue.one == 1000, "%ld", poppedValue.one);
    EXPECT(Array_length(arr) == 99, "Wrong length %zu", Array_length(arr));

    uint64_t sum = 0;
    Array_for(arr, idx) {
        sum += arr[idx].two;
    }

    EXPECT(sum == 990, "%ld", sum);

    printf("Sort ASC\n");

    Array_truncate(arr);

    for (int64_t i = 0; i < 6; i++) {
        Array_push(arr, testar(((i % 2) * 2 - 1) * i, 10));
    }

    Array_qsort(arr, TestStruct_compareAsc);
    int64_t prev = -6;
    TestStruct current;

    Array_for(arr, i) {
        current = arr[i];
        EXPECT(current.one > prev, "current %ld, prev %ld", current.one, prev);
        prev = current.one;
    }

    printf("Sort DESC\n");
    Array_qsort(arr, TestStruct_compareDesc);

    prev = 6;

    Array_for(arr, i) {
        current = arr[i];
        EXPECT(current.one < prev, "current %ld, prev %ld", current.one, prev);
        prev = current.one;
    }

    Array_free(arr);

    TEST_RUN(array_sort_i32());
    TEST_RUN(array_sort_u32());
    TEST_RUN(test_Array_qsort_u8());
    TEST_RUN(test_Array_index_sort());

    return 0;
}

