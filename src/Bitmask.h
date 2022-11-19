#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>


/**
 * 64-bit masks
 */
static inline uint64_t Bitmask_make64(size_t idx)
{
    uint64_t mask = 1;
    mask = mask << idx;

    return mask;
}

static inline uint64_t Bitmask_set64(uint64_t bits, size_t idx)
{
    return bits |= Bitmask_make64(idx);
}

static inline uint64_t Bitmask_clear64(uint64_t bits, size_t idx)
{
    uint64_t mask = Bitmask_make64(idx);

    return bits &= (~mask);
}

static inline uint64_t Bitmask_toggle64(uint64_t bits, size_t idx)
{

    return bits ^= Bitmask_make64(idx);
}


static inline bool Bitmask_get64(uint64_t bits, size_t idx)
{
    return (bool)(bits & Bitmask_make64(idx));
}


/**
 * 32-bit masks
 */
static inline uint32_t Bitmask_make32(size_t idx)
{
    uint32_t mask = 1;
    mask = mask << idx;

    return mask;
}

static inline uint32_t Bitmask_set32(uint32_t bits, size_t idx)
{
    return bits |= Bitmask_make32(idx);
}

static inline uint32_t Bitmask_clear32(uint32_t bits, size_t idx)
{
    uint32_t mask = Bitmask_make32(idx);

    return bits &= (~mask);
}

static inline bool Bitmask_get32(uint64_t bits, size_t idx)
{
    return (bool)(bits & Bitmask_make32(idx));
}


/**
 * Bitmask array
 */
static inline void Bitmask_set(uint64_t *bits, size_t idx)
{
    bits[idx / 64] |= Bitmask_make64(idx % 64);
}


static inline void Bitmask_clear(uint64_t *bits, size_t idx)
{
    uint64_t mask = Bitmask_make64(idx % 64);
    bits[idx / 64] &= (~mask);
}

static inline bool Bitmask_get(uint64_t *bits, size_t idx)
{
    return (bool)(bits[idx / 64] & Bitmask_make64(idx % 64));
}
