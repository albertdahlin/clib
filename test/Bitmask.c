#include <Bitmask.h>
#include <stdio.h>
#include <inttypes.h>
#include "test.h"

int main()
{
    TEST_HEADER;

    uint64_t mask64 = 0;

    printf("Bitmask_get:\n");
    EXPECT(Bitmask_get64(0x00, 1) == false, "");
    EXPECT(Bitmask_get64(0x01, 0) == true, "");
    EXPECT(Bitmask_get64(0x02, 0) == false, "");
    EXPECT(Bitmask_get64(0x02, 1) == true, "");

    printf("Bitmask_set:\n");
    EXPECT(Bitmask_get64(mask64, 1) == false, "");
    mask64 = Bitmask_set64(mask64, 1);
    EXPECT(Bitmask_get64(mask64, 0) == false, "");
    EXPECT(Bitmask_get64(mask64, 1) == true, "");
    EXPECT(Bitmask_get64(mask64, 2) == false, "");

    mask64 = 0;
    mask64 = Bitmask_set64(mask64, 0);
    mask64 = Bitmask_set64(mask64, 1);
    mask64 = Bitmask_set64(mask64, 2);
    mask64 = Bitmask_set64(mask64, 3);
    EXPECT(mask64 == 0x0F, "%lx", mask64);

    printf("Bitmask_clear:\n");
    mask64 = -1; // ALL 1
    EXPECT(Bitmask_get64(mask64, 10) == true, "");
    mask64 = Bitmask_clear64(mask64, 10);
    EXPECT(Bitmask_get64(mask64, 10) == false, "");

    printf("Bitmask_toggle:\n");
    mask64 = 0;
    mask64 = Bitmask_toggle64(mask64, 2);
    EXPECT(Bitmask_get64(mask64, 2) == true, "");
    mask64 = Bitmask_toggle64(mask64, 2);
    EXPECT(Bitmask_get64(mask64, 2) == false, "");

    return 0;
}
