#include <Heap.h>
#include <Array.h>
#include <stdio.h>
#include <inttypes.h>
#include "test.h"

typedef struct {
    uint64_t one;
    uint64_t two;
} TestItem;


TestItem TestItem_make(uint64_t a, uint64_t b)
{
    return (TestItem){ a, b };
}


int TestItem_compareAsc(void *pa, void *pb)
{
    TestItem *a = (TestItem*)pa;
    TestItem *b = (TestItem*)pb;

    return a->one - b->one;
}

int TestItem_comparePtrAsc(void *pa, void *pb)
{
    TestItem *a = *(TestItem**)pa;
    TestItem *b = *(TestItem**)pb;

    return a->one - b->one;
}

int TestItem_compareDesc(void *pa, void *pb)
{
    TestItem *a = (TestItem*)pa;
    TestItem *b = (TestItem*)pb;

    return b->one - a->one;
}

int Int64_compareAsc(void *pa, void *pb)
{
    int64_t a = *(int64_t*)pa;
    int64_t b = *(int64_t*)pb;

    return a - b;
}

void test_HeapPerformance()
{
    int64_t *heap = Heap_create(
        sizeof(int64_t),
        100000,
        (Heap_function_compare)Int64_compareAsc
    );

    for (int64_t i = 0; i < 100; i++) {
        Heap_push(heap, i);
    }

    int64_t j;

    for (int64_t i = 0; i < 1e7; i++) {
        j = Heap_pop(heap);
        Heap_push(heap, j + 1);
    }
    /*

    for (uint64_t i = 0; i < 1e7; i++) {
        j = Heap_pop_uint64(heap);
        Heap_push_uint64(heap, j + 1);
    }
    */

    printf("j=%ld\n", j);

    Heap_free(heap);

}

int heap_with_pointers()
{
    TestItem *testArray = Array_alloc(
        sizeof(TestItem),
        16
    );

    Array_push(testArray, TestItem_make(30, 1));
    Array_push(testArray, TestItem_make(20, 1));
    Array_push(testArray, TestItem_make(10, 1));
    Array_push(testArray, TestItem_make(100, 1));
    Array_push(testArray, TestItem_make(200, 1));

    TestItem **ptrHeap = Heap_create(
        sizeof(TestItem*),
        16,
        TestItem_comparePtrAsc
    );

    Array_for(testArray, i) {
        Heap_push(ptrHeap, &testArray[i]);
    }

    EXPECT(ptrHeap[0]->one == 10, "%ld", ptrHeap[0]->one);
    TestItem *item = Heap_pop(ptrHeap);
    EXPECT(item->one == 10, "%ld", item->one);
    item = Heap_pop(ptrHeap);
    EXPECT(item->one == 20, "%ld", item->one);

    Heap_free(ptrHeap);
    Array_free(testArray);

    return 0;
}

int heap_idx_math()
{
    size_t idx = 30;

    idx = Heap_childLeftIdx(idx);
    idx = Heap_childLeftIdx(idx);
    idx = Heap_parentIdx(idx);
    idx = Heap_parentIdx(idx);
    EXPECT(idx == 30, "%zu", idx);

    idx = Heap_childRightIdx(idx);
    idx = Heap_childRightIdx(idx);
    idx = Heap_parentIdx(idx);
    idx = Heap_parentIdx(idx);
    EXPECT(idx == 30, "%zu", idx);

    return 0;
}

int min_heap_with_struct()
{
    TestItem *minHeap = Heap_create(
        sizeof(TestItem),
        2,
        TestItem_compareAsc
    );

    Heap_push(minHeap, TestItem_make(30, 1));
    Heap_push(minHeap, TestItem_make(20, 1));
    Heap_push(minHeap, TestItem_make(10, 1));
    Heap_push(minHeap, TestItem_make(100, 1));
    Heap_push(minHeap, TestItem_make(200, 1));

    EXPECT(minHeap[0].one == 10, "%lu", minHeap[0].one);
    TestItem poppedItem = Heap_pop(minHeap);
    EXPECT(poppedItem.one == 10, "%lu", poppedItem.one);

    poppedItem = Heap_pop(minHeap);
    EXPECT(poppedItem.one == 20, "%lu", poppedItem.one);

    Heap_free(minHeap);

    return 0;
}

int max_heap_with_struct()
{
    TestItem *maxHeap = Heap_create(
        sizeof(TestItem),
        2,
        (Heap_function_compare)TestItem_compareDesc
    );

    Heap_push(maxHeap, TestItem_make(30, 1));
    Heap_push(maxHeap, TestItem_make(20, 1));
    Heap_push(maxHeap, TestItem_make(200, 1));
    Heap_push(maxHeap, TestItem_make(10, 1));
    Heap_push(maxHeap, TestItem_make(100, 1));

    EXPECT(maxHeap[0].one == 200, "%lu", maxHeap[0].one);

    TestItem poppedItem = Heap_pop(maxHeap);
    EXPECT(poppedItem.one == 200, "%lu", poppedItem.one);

    poppedItem = Heap_pop(maxHeap);
    EXPECT(poppedItem.one == 100, "%lu", poppedItem.one);

    Heap_free(maxHeap);

    return 0;
}

int main()
{
    TEST_HEADER;

    TEST_RUN(heap_idx_math());
    TEST_RUN(min_heap_with_struct());
    TEST_RUN(max_heap_with_struct());
    TEST_RUN(heap_with_pointers());

    //test_HeapPerformance();

    return 0;
}

