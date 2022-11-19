#pragma once
#include <stdbool.h>

static inline bool Char_isDigit(char c)
{
    return c >= 0x30 && c < 0x3A;
}

static inline bool Char_isSigned(char c)
{
    return (c >= 0x30 && c < 0x3A) || c == '-';
}


static inline bool Char_isNewLine(char c)
{
    return c == '\n';
}

static inline bool Char_isWhiteSpace(char c)
{
    return c == ' ' || c == '\n' || c == '\t' || c == '\r' ;
}

static inline bool Char_isAlpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
