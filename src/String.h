#pragma once
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    char *data;
    size_t length;
} String;

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String_Buffer;


#define String_fromLit(cstr) (String){cstr, sizeof(cstr) - 1}

String String_fromPtr(char *ptr, size_t len)
{
    return (String){ ptr, len };
}

String String_fromBuffer(String_Buffer buf)
{
    return (String){ buf.data, buf.length };
}


static inline bool String_startsWith(String needle, String haystack)
{
    if (needle.length > haystack.length) {
        return false;
    }

    for (size_t i = 0; i < needle.length; i++) {
        if (haystack.data[i] != needle.data[i]) {
            return false;
        }
    }

    return true;
}

static inline bool String_isEqual(String a, String b)
{
    if (a.length != b.length) {
        return false;
    }

    for (size_t i = 0; i < a.length; i++) {
        if (a.data[i] != b.data[i]) {
            return false;
        }
    }

    return true;
}

void String_free(String_Buffer buf)
{
    if (buf.data == NULL) {
        return;
    }

    free(buf.data);
}

String_Buffer String_fromFile(const char *name)
{
    String_Buffer buffer = {0};

    FILE *fd = fopen(name, "r");

    if (fd == NULL) {
        return buffer;
    }

    fseek(fd, 0, SEEK_END);
    buffer.capacity = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    if (buffer.capacity == 0) {
        return buffer;
    }

    buffer.data = malloc(buffer.capacity);
    assert(buffer.data != NULL);

    buffer.length = fread(buffer.data, sizeof(char), buffer.capacity, fd);

    fclose(fd);

    return buffer;
}
