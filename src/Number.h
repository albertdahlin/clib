#pragma once
#include <inttypes.h>


#define min(a, b) \
    _Generic((a), \
        uint64_t: min_uint64_t(a, b), \
        uint32_t: min_uint32_t(a, b), \
        uint16_t: min_uint16_t(a, b), \
        uint8_t: min_uint8_t(a, b), \
        int64_t: min_int64_t(a, b), \
        int32_t: min_int32_t(a, b), \
        int16_t: min_int16_t(a, b), \
        int8_t: min_int8_t(a, b) \
    )

#define max(a, b) \
    _Generic((a), \
        uint64_t: max_uint64_t(a, b), \
        uint32_t: max_uint32_t(a, b), \
        uint16_t: max_uint16_t(a, b), \
        uint8_t: max_uint8_t(a, b), \
        int64_t: max_int64_t(a, b), \
        int32_t: max_int32_t(a, b), \
        int16_t: max_int16_t(a, b), \
        int8_t: max_int8_t(a, b) \
    )

#define clamp(a, b, c) \
    _Generic((a), \
        uint64_t: clamp_uint64_t(a, b, c), \
        uint32_t: clamp_uint32_t(a, b, c), \
        uint16_t: clamp_uint16_t(a, b, c), \
        uint8_t: clamp_uint8_t(a, b, c), \
        int64_t: clamp_int64_t(a, b, c), \
        int32_t: clamp_int32_t(a, b, c), \
        int16_t: clamp_int16_t(a, b, c), \
        int8_t: clamp_int8_t(a, b, c) \
    )

#define template_min(T) \
    static inline T min_##T(T a, T b) \
    { \
        return a < b ? a : b; \
    }

#define template_max(T) \
    static inline T max_##T(T a, T b) \
    { \
        return a > b ? a : b; \
    }

#define template_clamp(T) \
    static inline T clamp_##T(T low, T high, T val) \
    { \
        return max(low, min(high, val)); \
    }

template_min(uint64_t);
template_min(uint32_t);
template_min(uint16_t);
template_min(uint8_t);
template_min(int64_t);
template_min(int32_t);
template_min(int16_t);
template_min(int8_t);

template_max(uint64_t);
template_max(uint32_t);
template_max(uint16_t);
template_max(uint8_t);
template_max(int64_t);
template_max(int32_t);
template_max(int16_t);
template_max(int8_t);

template_clamp(uint64_t);
template_clamp(uint32_t);
template_clamp(uint16_t);
template_clamp(uint8_t);
template_clamp(int64_t);
template_clamp(int32_t);
template_clamp(int16_t);
template_clamp(int8_t);
