#include <Parser.h>
#include <String.h>
#include <Char.h>
#include <stdio.h>
#include "test.h"

#define print_state(f, p) printf(f ": %zu %zu %s\n", p.from, p.to, Parser_errorToString(&p))

int main()
{
    TEST_HEADER;

    String text = String("one two -111\n 3 4 \n56 7 hello\n  \nx4\n");
    Parser parser = Parser_fromString(text);

    Parser_chompUntil(&Char_isDigit, &parser);
    Parser_discard(&parser);
    Parser_chompWhile(&Char_isDigit, &parser);
    String ones = Parser_getChomped(&parser);
    printf("Parser: *%.*s*\n", (unsigned int)ones.length, ones.data);

    parser = Parser_fromString(text);

    Parser_chompUntil(&Char_isSigned, &parser);
    Parser_discard(&parser);
    int64_t num1 = Parser_toI64(&parser);

    Parser_chompUntil(&Char_isDigit, &parser);
    Parser_discard(&parser);
    uint64_t num2 = Parser_toU64(&parser);
    printf("Parser num1: %"PRIi64"\n", num1);
    printf("Parser num2: %"PRIi64"\n", num2);

    char four[] = {'1', '2', 'c', 'd'};
    String str = String(four);

    parser = Parser_fromString(str);
    Parser_chompChar('1', &parser);
    print_state("chompChar(a)", parser);
    Parser_chompChar('2', &parser);
    print_state("chompChar(b)", parser);
    Parser_chompChar('d', &parser);
    print_state("chompChar(d)", parser);

    parser = Parser_fromString(str);
    Parser_chompAtMost(2, &parser);
    print_state("chompAtMost(2)", parser);
    Parser_chompAtMost(2, &parser);
    print_state("chompAtMost(2)", parser);

    parser = Parser_fromString(str);
    Parser_chompAtLeast(2, &parser);
    print_state("chompAtLeast(2)", parser);
    Parser_chompAtLeast(2, &parser);
    print_state("chompAtLeast(2)", parser);

    parser = Parser_fromString(str);
    uint64_t i = Parser_toU64(&parser);
    printf("Parsed int: %" PRIu64 "\n", i);
    print_state("toU64()", parser);
    i = Parser_toU64(&parser);
    printf("Parsed int: %" PRIu64 "\n", i);
    print_state("toU64()", parser);


    return 0;
}

