#pragma once

#include <cstdint>
#include <cstddef>

#if defined(__GNUC__) || defined(__clang__)
  #ifdef __i386
    #define ARCH_X86
  #endif
  #ifdef __amd64
    #define ARCH_X86_64
  #endif
#endif

namespace CountZeros
{
  inline int trailingZeros(const uint64_t mask);
  inline int leadingZeros(const uint64_t mask);

  inline size_t findAndClearTrailingOne(uint64_t & mask)
  {
    size_t ret = trailingZeros(mask);
    mask &= mask -1;
    return ret;
  }

  #if defined(__GNUC__) || defined(__clang__)
    #ifdef ARCH_X86_64
      inline int trailingZeros(const uint64_t mask)
      {
        uint64_t Ret;
        __asm__
        (
            "bsfq %[mask], %[Ret]"
            :[Ret] "=r" (Ret)
            :[mask] "mr" (mask)
        );
        return (int)Ret;
      }
      inline int leadingZeros(const uint64_t mask)
      {
        uint64_t Ret;
        __asm__
        (
            "bsrq %[mask], %[Ret]"
            :[Ret] "=r" (Ret)
            :[mask] "mr" (mask)
        );
        return (int)Ret;
      }
    #endif
  #endif
}
