#pragma once

#include "types.hpp"

namespace BitOperations
{
  inline int trailingZeros(uint64_t mask);
  inline int leadingZeros(uint64_t mask);

  #if defined(__GNUC__) || defined(__clang__)
    #ifdef ARCH_X86_64
      inline int trailingZeros(uint64_t mask)
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
      inline int leadingZeros(uint64_t mask)
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
