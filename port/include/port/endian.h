#pragma once
/**
 * port/endian.h
 * Byte-swap utilities for reading GameCube (big-endian) data on x86.
 *
 * The decompiled game logic operates on native-endian values at runtime.
 * Endianness only matters at the I/O boundary: when loading binary data
 * files (arc, bmd, brk, stage files, etc.) that were authored on PPC.
 *
 * Usage:
 *   u32 val = read_be32(ptr);   // safe on any host endianness
 *   write_be32(ptr, val);
 *
 * The beXX_t<T> wrapper types can be embedded in structs that directly
 * map onto on-disk data layouts (like Ship of Harkinian's approach).
 */

#include <cstdint>
#include <cstring>
#if defined(_MSC_VER)
#  include <stdlib.h>  // _byteswap_ushort / _byteswap_ulong / _byteswap_uint64
#endif

// -----------------------------------------------------------------------
// Compile-time host endianness (set by CMakeLists.txt)
// -----------------------------------------------------------------------
#ifndef TP_LITTLE_ENDIAN
#  if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#    define TP_LITTLE_ENDIAN 0
#  else
#    define TP_LITTLE_ENDIAN 1
#  endif
#endif

// -----------------------------------------------------------------------
// Swap primitives
// -----------------------------------------------------------------------
namespace tp::endian {

inline uint16_t swap16(uint16_t v) {
#if defined(_MSC_VER)
    return _byteswap_ushort(v);
#elif defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap16(v);
#else
    return (uint16_t)((v >> 8) | (v << 8));
#endif
}

inline uint32_t swap32(uint32_t v) {
#if defined(_MSC_VER)
    return _byteswap_ulong(v);
#elif defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32(v);
#else
    return ((v & 0xFF000000u) >> 24) | ((v & 0x00FF0000u) >> 8) |
           ((v & 0x0000FF00u) << 8)  | ((v & 0x000000FFu) << 24);
#endif
}

inline uint64_t swap64(uint64_t v) {
#if defined(_MSC_VER)
    return _byteswap_uint64(v);
#elif defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap64(v);
#else
    return ((v & 0xFF00000000000000ull) >> 56) |
           ((v & 0x00FF000000000000ull) >> 40) |
           ((v & 0x0000FF0000000000ull) >> 24) |
           ((v & 0x000000FF00000000ull) >> 8)  |
           ((v & 0x00000000FF000000ull) << 8)  |
           ((v & 0x0000000000FF0000ull) << 24) |
           ((v & 0x000000000000FF00ull) << 40) |
           ((v & 0x00000000000000FFull) << 56);
#endif
}

// -----------------------------------------------------------------------
// Read/write helpers — byte-swap only when host != big-endian
// -----------------------------------------------------------------------
inline uint8_t  read_be8 (const void* p) { uint8_t  v; memcpy(&v, p, 1); return v; }
inline uint16_t read_be16(const void* p) { uint16_t v; memcpy(&v, p, 2); return TP_LITTLE_ENDIAN ? swap16(v) : v; }
inline uint32_t read_be32(const void* p) { uint32_t v; memcpy(&v, p, 4); return TP_LITTLE_ENDIAN ? swap32(v) : v; }
inline uint64_t read_be64(const void* p) { uint64_t v; memcpy(&v, p, 8); return TP_LITTLE_ENDIAN ? swap64(v) : v; }

inline float read_bef32(const void* p) {
    uint32_t raw = read_be32(p);
    float f;
    memcpy(&f, &raw, 4);
    return f;
}

inline void write_be8 (void* p, uint8_t  v) { memcpy(p, &v, 1); }
inline void write_be16(void* p, uint16_t v) { if (TP_LITTLE_ENDIAN) v = swap16(v); memcpy(p, &v, 2); }
inline void write_be32(void* p, uint32_t v) { if (TP_LITTLE_ENDIAN) v = swap32(v); memcpy(p, &v, 4); }
inline void write_be64(void* p, uint64_t v) { if (TP_LITTLE_ENDIAN) v = swap64(v); memcpy(p, &v, 8); }

inline void write_bef32(void* p, float f) {
    uint32_t raw;
    memcpy(&raw, &f, 4);
    write_be32(p, raw);
}

// -----------------------------------------------------------------------
// Wrapper type for structs that map directly onto big-endian binary data.
//
// Usage:
//   struct ArcHeader {
//       be_t<uint32_t> magic;
//       be_t<uint32_t> fileSize;
//   };
//   ArcHeader* hdr = reinterpret_cast<ArcHeader*>(rawBuffer);
//   uint32_t sz = hdr->fileSize;  // auto-swapped
// -----------------------------------------------------------------------
template<typename T>
struct be_t {
    static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8,
                  "be_t only supports 1/2/4/8-byte types");
    uint8_t raw[sizeof(T)];

    be_t() = default;
    be_t(T val) { *this = val; }

    T get() const {
        if constexpr (sizeof(T) == 1) { return static_cast<T>(raw[0]); }
        else if constexpr (sizeof(T) == 2) { uint16_t v; memcpy(&v, raw, 2); return static_cast<T>(TP_LITTLE_ENDIAN ? swap16(v) : v); }
        else if constexpr (sizeof(T) == 4) { uint32_t v; memcpy(&v, raw, 4); return static_cast<T>(TP_LITTLE_ENDIAN ? swap32(v) : v); }
        else { uint64_t v; memcpy(&v, raw, 8); return static_cast<T>(TP_LITTLE_ENDIAN ? swap64(v) : v); }
    }

    void set(T val) {
        if constexpr (sizeof(T) == 1) { raw[0] = static_cast<uint8_t>(val); }
        else if constexpr (sizeof(T) == 2) { uint16_t v = static_cast<uint16_t>(val); if (TP_LITTLE_ENDIAN) v = swap16(v); memcpy(raw, &v, 2); }
        else if constexpr (sizeof(T) == 4) { uint32_t v = static_cast<uint32_t>(val); if (TP_LITTLE_ENDIAN) v = swap32(v); memcpy(raw, &v, 4); }
        else { uint64_t v = static_cast<uint64_t>(val); if (TP_LITTLE_ENDIAN) v = swap64(v); memcpy(raw, &v, 8); }
    }

    operator T() const { return get(); }
    be_t& operator=(T val) { set(val); return *this; }
};

using be_u8  = be_t<uint8_t>;
using be_u16 = be_t<uint16_t>;
using be_u32 = be_t<uint32_t>;
using be_u64 = be_t<uint64_t>;
using be_s8  = be_t<int8_t>;
using be_s16 = be_t<int16_t>;
using be_s32 = be_t<int32_t>;
using be_s64 = be_t<int64_t>;
using be_f32 = be_t<float>;

} // namespace tp::endian
