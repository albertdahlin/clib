#pragma once
#include <String.h>
#include <Char.h>
#include <inttypes.h>

typedef enum Parser_Err {
    PARSER_OK,
    PARSER_EXPECTING_CHAR,
    PARSER_EXPECTING_INT,
    PARSER_AT_LEAST
} Parser_Error;


typedef struct {
    char *data;
    size_t length;
    size_t from;
    size_t to;
    uint32_t row;
    uint32_t col;
    Parser_Error error;
} Parser;


typedef bool (*char_isGood_fn) (char);

#define Parser_errorHelp(T) case T: return #T

const char *Parser_errorToString(Parser *parser)
{
    switch (parser->error) {
        Parser_errorHelp(PARSER_OK);
        Parser_errorHelp(PARSER_EXPECTING_CHAR);
        Parser_errorHelp(PARSER_EXPECTING_INT);
        Parser_errorHelp(PARSER_AT_LEAST);
    }

    return "";
}


Parser Parser_fromString(String str)
{
    Parser parser = {
        .data = str.data,
        .length = str.length,
        .from = 0,
        .to = 0,
        .row = 1,
        .col = 1,
        .error = PARSER_OK
    };

    return parser;
}

static inline bool Parser_isDone(Parser *parser)
{
    return parser->error > 0 || parser->from >= parser->length;
}


static inline void Parser_discard(Parser *parser)
{
    if (Parser_isDone(parser)) {
        return;
    }
    parser->from = parser->to;
}

void Parser_chompWhile(char_isGood_fn isGood, Parser *parser)
{
    if (Parser_isDone(parser)) {
        return;
    }
    while (parser->to < parser->length && isGood(parser->data[parser->to])) {
        parser->to++;
    }
}

static inline void Parser_skipWhile(char_isGood_fn isGood, Parser *parser)
{
    Parser_chompWhile(isGood, parser);
    Parser_discard(parser);
}

void Parser_chompChar(char character, Parser *parser)
{
    if (Parser_isDone(parser)) {
        return;
    }
    if (parser->to < parser->length && parser->data[parser->to] == character) {
        parser->to++;
    } else {
        parser->error = PARSER_EXPECTING_CHAR;
    }
}

static inline void Parser_skipChar(char character, Parser *parser)
{
    Parser_chompChar(character, parser);
    Parser_discard(parser);
}


void Parser_chompUntil(char_isGood_fn isGood, Parser *parser)
{
    if (Parser_isDone(parser)) {
        return;
    }
    while (parser->to < parser->length && !isGood(parser->data[parser->to])) {
        parser->to++;
    }
}

static inline void Parser_skipUntil(char_isGood_fn isGood, Parser *parser)
{
    Parser_chompUntil(isGood, parser);
    Parser_discard(parser);
}

static inline char Parser_current(Parser *parser)
{
    return parser->data[parser->from];
}

static inline char *Parser_ptr(Parser *parser)
{
    return &parser->data[parser->from];
}

void Parser_chompAtLeast(size_t n, Parser *parser)
{
    if (Parser_isDone(parser)) {
        return;
    }
    while (parser->to < parser->length && n > 0) {
        parser->to++;
        n--;
    }

    if (n != 0) {
        parser->error = PARSER_AT_LEAST;
    }
}

static inline void Parser_skipAtLeast(size_t n, Parser *parser)
{
    Parser_chompAtLeast(n, parser);
    Parser_discard(parser);
}


void Parser_chompAtMost(size_t n, Parser *parser)
{
    if (Parser_isDone(parser)) {
        return;
    }
    while (parser->to < parser->length && n > 0) {
        parser->to++;
        n--;
    }
}


String Parser_getChomped(Parser *parser) {
    String str = {0};
    str.data = parser->data + parser->from;
    str.length = parser->to - parser->from;

    parser->from = parser->to;

    return str;
}

int64_t Parser_toI64(Parser *parser)
{
    if (Parser_isDone(parser)) {
        return 0;
    }
    int64_t result = 0;
    char c;
    size_t offset = parser->from;
    int64_t sign = 1;

    if (parser->data[offset] == '-') {
        sign = -1;
        offset++;
    }

    while (offset < parser->length) {
        c = parser->data[offset];
        if (Char_isDigit(c)) {
            result = result * 10 + (int64_t)(c & 0xF);
            offset++;
        } else {
            break;
        }
    }

    parser->from = offset;
    parser->to = offset;

    result *= sign;

    return result;
}

uint64_t Parser_toU64(Parser *parser)
{
    if (Parser_isDone(parser)) {
        return 0;
    }
    uint64_t result = 0;
    char c;
    size_t offset = parser->from;
    parser->error = PARSER_EXPECTING_INT;

    while (offset < parser->length) {
        c = parser->data[offset];
        if (Char_isDigit(c)) {
            parser->error = PARSER_OK;
            result = result * 10 + (uint64_t)(c & 0xF);
            offset++;
        } else {
            break;
        }
    }

    parser->from = offset;
    parser->to = offset;

    return result;
}
