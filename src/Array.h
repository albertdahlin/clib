#pragma once
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>


typedef struct {
    size_t length;
    size_t capacity;
    size_t elementSize;
} Array_Header;


void *Array_alloc(size_t elementSize, size_t capacity)
{
    Array_Header *header = malloc(sizeof(Array_Header) + elementSize * capacity);
    assert(header != NULL);

    header->length = 0;
    header->capacity = capacity;
    header->elementSize = elementSize;

    return &header[1];
}


void Array_free(void *array)
{
    if (array == NULL) {
        return;
    }

    Array_Header *header = array;
    header--;
    free(header);
}


void Array_truncate(void *arr)
{
    Array_Header *header = arr;
    header--;
    header->length = 0;
}


void *Array_ensureCapacity(void *arr)
{
    Array_Header *header = arr;
    header--;

    if (header->length == header->capacity) {
        header = realloc(
            header,
            header->elementSize * header->capacity * 2 + sizeof(Array_Header)
        );
        assert(header != NULL);
        header->capacity *= 2;
    }

    return &header[1];
}


static inline size_t Array_length(void *array)
{
    Array_Header *header = array;
    header--;

    return header->length;
}


static inline size_t Array_pushIdx(void *array)
{
    Array_Header *header = array;
    header--;
    size_t len = header->length;
    header->length++;

    return len;
}


static inline size_t Array_popIdx(void *array)
{
    Array_Header *header = array;
    header--;
    assert(header->length > 0);
    header->length--;

    return header->length;
}


#define Array_push(arr, item) \
    arr = Array_ensureCapacity(arr); \
    arr[Array_pushIdx(arr)] = (item)


#define Array_pop(arr) arr[Array_popIdx(arr)]


#define Array_for(arr, idx) \
    for (size_t idx = 0; idx < Array_length(arr); idx++)


typedef int (*Array_function_compare)(const void*, const void*);

/*** ARRAY SORT ***/

static inline void Array_qsort(void *array, Array_function_compare compare)
{
    Array_Header *header = array;
    header--;

    qsort(array, header->length, header->elementSize, compare);
}


#define Array_template_qsortAsc(T, N) \
static inline void _Array_qsort_##T(T *array, int_fast32_t first, int_fast32_t last) \
{ \
    int_fast32_t left, right, pivot; \
    T temp; \
    if (first >= last) { \
        return; \
    } \
    pivot = last; \
    left = first; \
    right = last; \
    while (left < right) { \
        while (array[left] <= array[pivot] && left < last) { \
            left++; \
        } \
        while (array[right] > array[pivot]) { \
            right--; \
        } \
        if (left > right) { \
            continue; \
        } \
        temp = array[left]; \
        array[left] = array[right]; \
        array[right] = temp; \
    } \
    temp = array[pivot]; \
    array[pivot] = array[right]; \
    array[right] = temp; \
    _Array_qsort_##T(array, first, right - 1); \
    _Array_qsort_##T(array, right + 1, last); \
} \
void N(T *array) \
{ \
    Array_Header *header = (Array_Header*)array; \
    header--; \
    _Array_qsort_##T(array, 0, header->length - 1); \
}

Array_template_qsortAsc(int64_t, Array_qsort_i64);
Array_template_qsortAsc(int32_t, Array_qsort_i32);
Array_template_qsortAsc(int16_t, Array_qsort_i16);
Array_template_qsortAsc(int8_t, Array_qsort_i8);
Array_template_qsortAsc(uint64_t, Array_qsort_u64);
Array_template_qsortAsc(uint32_t, Array_qsort_u32);
Array_template_qsortAsc(uint16_t, Array_qsort_u16);
Array_template_qsortAsc(uint8_t, Array_qsort_u8);

static inline void _Array_qsort_index(
    uint32_t *array,
    int_fast32_t first,
    int_fast32_t last,
    size_t size,
    void *data,
    Array_function_compare compare
) {
   int_fast32_t i, j, pivot;
   uint32_t temp;

   if (first < last) {
      pivot = first;
      i = first;
      j = last;

      while (i < j) {
         while (compare(data + size*array[i], data + size*array[pivot]) < 1 && i < last)
            i++;
         while (compare(data + size*array[j], data + size*array[pivot]) > 0)
            j--;
         if (i < j) {
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
         }
      }

      temp = array[pivot];
      array[pivot] = array[j];
      array[j] = temp;
      _Array_qsort_index(array, first, j - 1, size, data, compare);
      _Array_qsort_index(array, j + 1, last, size, data, compare);
   }
}

void Array_index_sort(uint32_t *array, void *data, Array_function_compare compare)
{
    Array_Header *indexHeader = (Array_Header*)array;
    indexHeader--;

    Array_Header *dataHeader = (Array_Header*)data;
    dataHeader--;

    assert(dataHeader->length >= indexHeader->length);

    _Array_qsort_index(array, 0, indexHeader->length - 1, dataHeader->elementSize, data, compare);
}


uint32_t *Array_index_createSorted(void *data, Array_function_compare compare)
{
    Array_Header *dataHeader = (Array_Header*)data;
    dataHeader--;

    uint32_t *array = Array_alloc(sizeof(uint32_t), dataHeader->length);

    for (uint32_t i = 0; i < dataHeader->length; i++) {
        array[i] = i;
    }

    Array_Header *iheader = (Array_Header*)array;
    iheader--;
    iheader->length = dataHeader->length;

    _Array_qsort_index(array, 0, dataHeader->length - 1, dataHeader->elementSize, data, compare);

    return array;
}
