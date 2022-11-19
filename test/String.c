#include <String.h>
#include <stdio.h>
#include <inttypes.h>
#include "test.h"


int main()
{
    TEST_HEADER;
    String lit = String_fromLit("one");

    EXPECT(lit.length == 3, "%zu", lit.length);

    char ptr[] = {'a', 'b', 'c' };
    String fromPtr = String_fromPtr(ptr, 3);
    EXPECT(fromPtr.length == 3, "%zu", fromPtr.length);

    String_Buffer contents = String_fromFile(__FILE__);
    String thisFile = String_fromBuffer(contents);
    EXPECT(String_startsWith(String_fromLit("#include"), thisFile) == true, "false");
    EXPECT(String_startsWith(lit, thisFile) == false, "true");

    String_free(contents);

    return 0;
}
